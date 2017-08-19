#include "MetalConstantBuffer.h"
#include "MetalRenderDevice.h"

namespace ma
{
    std::map<std::string, RefPtr<ConstantBuffer> > g_mapConstantBufferPool;

	ConstantBuffer::ConstantBuffer() 
	{
		//m_pMetalBuffer = nil;
		m_bDirty = false;
        //m_shadowData = NULL;
        m_nIndex = 0;
        m_nCurFrame = 0;
	}

	ConstantBuffer::~ConstantBuffer()
	{
		Release();
	}

	void ConstantBuffer::Release()
	{
		//if (m_pMetalBuffer)
		//{
			//SAFE_RELEASE(m_pMetalBuffer);
		//}

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
		size += 15;
		size &= 0xfffffff0;

		m_bDirty = false;
		//m_shadowData.resize(size);// = new unsigned char[size_];
		//memset(&m_shadowData[0], 0, size);

        for (uint i = 0; i < 3; ++i)
        {
            m_pMetalBuffer[i] = [GetMetalDevive() newBufferWithLength:size options:0];
            m_pMetalBuffer[i].label = [NSString stringWithFormat:@"ConstantBuffer %d_frmae%d",nIndex,i];
        }

        m_nSize = size;

		return true;
	}

	void ConstantBuffer::SetParameter(unsigned offset, unsigned size, const void* data)
	{
		if (offset + size > m_nSize)
			return; // Would overflow the buffer

        void* shadowData = [m_pMetalBuffer[m_nCurFrame] contents];
        BYTE* dest = (BYTE*)shadowData;
		memcpy(dest + offset, data, size);
		m_bDirty = true;
	}

	void ConstantBuffer::Apply(id<MTLRenderCommandEncoder> renderEncoder,bool ps)
	{
		if (m_bDirty && m_pMetalBuffer)
		{
            if (ps)
            {
                [renderEncoder setFragmentBuffer:m_pMetalBuffer[m_nCurFrame] offset:0 atIndex:m_nIndex];
            }
            else
            {
                [renderEncoder setVertexBuffer:m_pMetalBuffer[m_nCurFrame] offset:0 atIndex:m_nIndex];
            }
            
			m_bDirty = false;
		}
	}
    
    void ConstantBuffer::UpdateFrame()
    {
         m_nCurFrame = (m_nCurFrame + 1) % 3;
    }

    void ConstantBuffer::OnEndFrame()
    {
        auto it  = g_mapConstantBufferPool.begin();
        for (; it != g_mapConstantBufferPool.end(); ++it)
        {
            it->second->UpdateFrame();
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
		//auto it = g_mapConstantBufferPool.find(pszName);
		//if (it != g_mapConstantBufferPool.end())
		//{
		//	return it->second.get();
		//}
		//else
		{
			RefPtr<ConstantBuffer> newConstantBuffer(new ConstantBuffer());
			newConstantBuffer->SetSize(size,nIndex);
			g_mapConstantBufferPool[pszName] = newConstantBuffer;
			return newConstantBuffer;
		}
	}

}
