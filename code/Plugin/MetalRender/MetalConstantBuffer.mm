#include "MetalConstantBuffer.h"
#include "MetalRenderDevice.h"

namespace ma
{
    std::map<std::string, RefPtr<ConstantBuffer> > g_mapConstantBufferPool;

	ConstantBuffer::ConstantBuffer() 
	{
		m_pMetalBuffer = nil;
		m_bDirty = false;
        m_shadowData = NULL;
        m_nIndex = 0;
	}

	ConstantBuffer::~ConstantBuffer()
	{
		Release();
	}

	void ConstantBuffer::Release()
	{
		if (m_pMetalBuffer)
		{
			//SAFE_RELEASE(m_pMetalBuffer);
		}

		//m_shadowData.clear();
	}

	bool ConstantBuffer::SetSize(UINT size,UINT nIndex)
	{
		Release();
        
        m_nIndex = nIndex;

		if (!size)
		{
			LogError("Can not create zero-sized constant buffer");
			return false;
		}

		// Round up to next 16 bytes
		//size += 15;
		//size &= 0xfffffff0;

		m_bDirty = false;
		//m_shadowData.resize(size);// = new unsigned char[size_];
		//memset(&m_shadowData[0], 0, size);

        m_pMetalBuffer = [GetMetalDevive() newBufferWithLength:size options:0];
        m_pMetalBuffer.label = [NSString stringWithFormat:@"ConstantBuffer"];
        
        m_shadowData = [m_pMetalBuffer contents];
        m_nSize = size;

		return true;
	}

	void ConstantBuffer::SetParameter(unsigned offset, unsigned size, const void* data)
	{
		if (offset + size > m_nSize)
			return; // Would overflow the buffer

        BYTE* dest = (BYTE*)m_shadowData;
		memcpy(dest + offset, data, size);
		m_bDirty = true;
	}

	void ConstantBuffer::SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data)
	{
		if (offset + rows * 4 * sizeof(float) > m_nSize)
			return; // Would overflow the buffer

		float* dest = (float*)m_shadowData;
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

	void ConstantBuffer::Apply(id<MTLRenderCommandEncoder> renderEncoder,bool ps)
	{
		if (m_bDirty && m_pMetalBuffer)
		{
            if (ps)
            {
                [renderEncoder setFragmentBuffer:m_pMetalBuffer offset:0 atIndex:m_nIndex];
            }
            else
            {
                [renderEncoder setVertexBuffer:m_pMetalBuffer offset:0 atIndex:m_nIndex];
            }
            
			m_bDirty = false;
		}
	}

	void ConstantBuffer::Clear()
	{
		g_mapConstantBufferPool.clear();
	}

	RefPtr<ConstantBuffer> CreateConstantBuffer(const char* pszName, UINT nIndex, UINT size)
	{
		// Ensure that different shader types and index slots get unique buffers, even if the size is same
		//unsigned key = type | (index << 1) | (size << 4);
		auto it = g_mapConstantBufferPool.find(pszName);
		if (it != g_mapConstantBufferPool.end())
		{
			return it->second.get();
		}
		else
		{
			RefPtr<ConstantBuffer> newConstantBuffer(new ConstantBuffer());
			newConstantBuffer->SetSize(size,nIndex);
			g_mapConstantBufferPool[pszName] = newConstantBuffer;
			return newConstantBuffer;
		}
	}

}
