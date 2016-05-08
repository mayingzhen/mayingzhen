
#include "D3D11ConstantBuffer.h"


namespace ma
{

	ConstantBuffer::ConstantBuffer() 
	{
		m_pD3D11Buffer = NULL;
		m_bDirty = false;
	}

	ConstantBuffer::~ConstantBuffer()
	{
		Release();
	}

	void ConstantBuffer::Release()
	{
		if (m_pD3D11Buffer)
		{
			SAFE_RELEASE(m_pD3D11Buffer);
		}

		m_shadowData.clear();
	}

	bool ConstantBuffer::SetSize(unsigned size)
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

	void ConstantBuffer::SetParameter(unsigned offset, unsigned size, const void* data)
	{
		if (offset + size > m_shadowData.size())
			return; // Would overflow the buffer

		memcpy(&m_shadowData[offset], data, size);
		m_bDirty = true;
	}

	void ConstantBuffer::SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data)
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

	void ConstantBuffer::Apply()
	{
		if (m_bDirty && m_pD3D11Buffer)
		{
			GetD3D11DxDeviveContext()->UpdateSubresource(m_pD3D11Buffer, 0, 0, &m_shadowData[0], 0, 0);
			m_bDirty = false;
		}
	}

}
