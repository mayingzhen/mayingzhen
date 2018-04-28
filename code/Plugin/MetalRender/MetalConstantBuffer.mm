#include "MetalConstantBuffer.h"
#include "MetalRenderDevice.h"

namespace ma
{
    std::vector< MetalConstantBuffer* > g_mapConstantBufferPool;

	MetalConstantBuffer::MetalConstantBuffer()
	{
        m_nCurFrame = 0;
        
        g_mapConstantBufferPool.push_back(this);
	}

	MetalConstantBuffer::~MetalConstantBuffer()
	{
		Release();
        
        auto it = std::find(g_mapConstantBufferPool.begin(),g_mapConstantBufferPool.end(),this);
        g_mapConstantBufferPool.erase(it);
	}

	void MetalConstantBuffer::Release()
	{
	}

	bool MetalConstantBuffer::Create()
	{
		Release();

		// Round up to next 16 bytes
		uint32_t nSize = GetSize() + 15;
		nSize &= 0xfffffff0;

        for (uint i = 0; i < 3; ++i)
        {
            m_pMetalBuffer[i] = [GetMetalDevive() newBufferWithLength:nSize options:0];
            m_pMetalBuffer[i].label = [NSString stringWithFormat:@"ConstantBuffer %d_frmae%d",GetBound(),i];
        }

		return true;
	}

	void MetalConstantBuffer::SetParameter(unsigned offset, unsigned size, const void* data)
	{
		if (offset + size > GetSize())
			return; // Would overflow the buffer
        
        if (!m_bApply)
        {
            for (uint32_t i = 0; i < 3; ++i)
            {
                void* shadowData = [m_pMetalBuffer[i] contents];
                uint8_t* dest = (uint8_t*)shadowData;
                memcpy(dest + offset, data, size);
            }
        }
        else
        {
            void* shadowData = [m_pMetalBuffer[m_nCurFrame] contents];
            uint8_t* dest = (uint8_t*)shadowData;
            memcpy(dest + offset, data, size);
        }
        
	}

	void MetalConstantBuffer::Apply(id<MTLRenderCommandEncoder> renderEncoder,bool ps)
	{
        m_bApply = true;
        
		if (m_pMetalBuffer[m_nCurFrame])
		{
            if (ps)
            {
                [renderEncoder setFragmentBuffer:m_pMetalBuffer[m_nCurFrame] offset:0 atIndex:GetBound()];
            }
            else
            {
                [renderEncoder setVertexBuffer:m_pMetalBuffer[m_nCurFrame] offset:0 atIndex:GetBound()];
            }
            
		}
	}
    
    void MetalConstantBuffer::UpdateFrame()
    {
        m_nCurFrame = (m_nCurFrame + 1) % 3;
    }

    void MetalConstantBuffer::OnEndFrame()
    {
        auto it  = g_mapConstantBufferPool.begin();
        for (; it != g_mapConstantBufferPool.end(); ++it)
        {
            (*it)->UpdateFrame();
        }
    }
    
	void MetalConstantBuffer::Clear()
	{
		g_mapConstantBufferPool.clear();
	}

	RefPtr<MetalConstantBuffer> CreateConstantBuffer(const char* pszName, uint32_t nIndex, uint32_t size)
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
			RefPtr<MetalConstantBuffer> newConstantBuffer(new MetalConstantBuffer());
            newConstantBuffer->SetSize(size);
            newConstantBuffer->SetBound(nIndex);
            newConstantBuffer->Create();
			return newConstantBuffer;
		}
	}

}
