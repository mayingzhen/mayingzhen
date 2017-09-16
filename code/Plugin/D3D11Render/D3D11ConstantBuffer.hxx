
#include "D3D11ConstantBuffer.h"


namespace ma
{
	std::map<unsigned, RefPtr<D3D11ConstantBuffer> > g_mapConstantBufferPool;

	D3D11ConstantBuffer::D3D11ConstantBuffer() 
	{
		m_pD3D11Buffer = NULL;
		m_bDirty = false;
	}

	D3D11ConstantBuffer::~D3D11ConstantBuffer()
	{
		Release();
	}

	void D3D11ConstantBuffer::Release()
	{
		if (m_pD3D11Buffer)
		{
			SAFE_RELEASE(m_pD3D11Buffer);
		}

		m_shadowData.clear();
	}

	bool D3D11ConstantBuffer::SetSize(unsigned size)
	{
		Release();

		if (!size)
		{
			LogError("Can not create zero-sized constant buffer");
			return false;
		}

		// Round up to next 16 bytes
		size += 15;
		size &= 0xfffffff0;

		m_bDirty = false;
		m_shadowData.resize(size);// = new unsigned char[size_];
		memset(&m_shadowData[0], 0, size);

		//if (graphics_)
		{
			D3D11_BUFFER_DESC bufferDesc;
			memset(&bufferDesc, 0, sizeof bufferDesc);

			bufferDesc.ByteWidth = size;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;

			GetD3D11DxDevive()->CreateBuffer(&bufferDesc, 0, &m_pD3D11Buffer);
			ASSERT(m_pD3D11Buffer);
			if (!m_pD3D11Buffer)
			{
				LogError("Failed to create constant buffer");
				return false;
			}
		}

		return true;
	}

	void D3D11ConstantBuffer::SetParameter(unsigned offset, unsigned size, const void* data)
	{
		if (offset + size > m_shadowData.size())
			return; // Would overflow the buffer

		memcpy(&m_shadowData[offset], data, size);
		m_bDirty = true;
	}

	void D3D11ConstantBuffer::SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data)
	{
		if (offset + rows * 4 * sizeof(float) > m_shadowData.size())
			return; // Would overflow the buffer

		float* dest = (float*)&m_shadowData[offset];
		const float* src = (const float*)data;

		while (rows--)
		{
			*dest++ = *src++;
			*dest++ = *src++;
			*dest++ = *src++;
			++dest; // Skip over the w coordinate
		}

		m_bDirty = true;
	}

	void D3D11ConstantBuffer::Apply()
	{
		if (m_bDirty && m_pD3D11Buffer)
		{
			GetD3D11DxDeviveContext()->UpdateSubresource(m_pD3D11Buffer, 0, 0, &m_shadowData[0], 0, 0);
			m_bDirty = false;
		}
	}

	void D3D11ConstantBuffer::Clear()
	{
		g_mapConstantBufferPool.clear();
	}

	RefPtr<D3D11ConstantBuffer> CreateConstantBuffer(ShaderType type, unsigned index, unsigned size)
	{
		// Ensure that different shader types and index slots get unique buffers, even if the size is same
		unsigned key = type | (index << 1) | (size << 4);
		map<unsigned, RefPtr<D3D11ConstantBuffer> >::iterator i = g_mapConstantBufferPool.find(key);
		if (i != g_mapConstantBufferPool.end())
		{
			return i->second.get();
		}
		else
		{
			RefPtr<D3D11ConstantBuffer> newConstantBuffer(new D3D11ConstantBuffer());
			newConstantBuffer->SetSize(size);
			g_mapConstantBufferPool[key] = newConstantBuffer;
			return newConstantBuffer;
		}
	}

}
