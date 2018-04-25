#include "MetalRenderPass.h"
#include "MetalTexture.h"
#include "MetalRenderDevice.h"
#include "MetalRenderCommand.h"

#include <thread>

namespace ma
{

	MetalRenderPass::MetalRenderPass()
	{
        uint32_t numThreads = std::thread::hardware_concurrency();
        m_arrRenderCommand.resize(numThreads * RL_Count);
        for (uint32_t i = 0; i < m_arrRenderCommand.size(); ++i)
        {
            m_arrRenderCommand[i] = new MetalRenderCommand();
            m_arrRenderCommand[i]->m_pRenderPass = this;
        }
	}

	MetalRenderPass::~MetalRenderPass()
	{

	}

	void MetalRenderPass::Begine()
	{
        MetalRenderDevice* pMetalRender = (MetalRenderDevice*)GetRenderDevice();
        
        m_parallelRCE = [pMetalRender->m_command_buffer parallelRenderCommandEncoderWithDescriptor: m_pass_desc];
	}

	void MetalRenderPass::End()
	{
        [m_parallelRCE endEncoding];
	}

	void MetalRenderPass::Create()
	{
        m_pass_desc = [MTLRenderPassDescriptor new];
        
        for (uint32_t i = 0; i < MAX_RENDERTARGETS; ++i)
        {
            if (m_arrColor[i].m_pTexture == nullptr)
				continue;

            MetalTexture* pMlTexture = (MetalTexture*)m_arrColor[i].m_pTexture.get();

            m_pass_desc.colorAttachments[i].texture = pMlTexture->GetNative();
            m_pass_desc.colorAttachments[i].resolveTexture = nil;
            m_pass_desc.colorAttachments[i].loadAction = MTLLoadActionClear;
            m_pass_desc.colorAttachments[i].storeAction = MTLStoreActionStore;
            //m_pass_desc.colorAttachments[i].clearColor =  MTLClearColor(0,0,0,0);
        }

		if (m_pDepthStencil)
		{
            MetalTexture* pMlTexture = (MetalTexture*)m_pDepthStencil.get();
            
            m_pass_desc.depthAttachment.texture = pMlTexture->GetNative();
            m_pass_desc.depthAttachment.loadAction = MTLLoadActionClear;
            m_pass_desc.depthAttachment.storeAction = MTLStoreActionStore;
            m_pass_desc.depthAttachment.clearDepth = 1.0;

            //m_pass_desc.stencilAttachment.texture = pMlTexture->GetNative();
            //m_pass_desc.stencilAttachment.loadAction = MTLLoadActionClear;
            //m_pass_desc.stencilAttachment.storeAction = MTLStoreActionStore;
            //m_pass_desc.stencilAttachment.clearStencil = 0;
        }
	}
    
    RenderCommand* MetalRenderPass::GetThreadCommand(uint32_t nIndex, RenderListType eRLType)
    {
		uint32_t numThreads = std::thread::hardware_concurrency();
        uint32_t nAt = eRLType * numThreads + nIndex;
        return m_arrRenderCommand[nAt].get();
    }
}

