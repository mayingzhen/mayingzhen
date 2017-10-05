#include "MetalRenderCommand.h"
#include "MetalIndexBuffer.h"
#include "MetalVertexBuffer.h"
#include "MetalMapping.h"
#include "MetalRenderPass.h"
#include "MetalTechniqueh.h"
#include "MetalRenderState.h"
#include "MetalConstantBuffer.h"
#include "MetalPipeline.h"
#include "MetalSamplerState.h"
#include "MetalTexture.h"

namespace ma
{

	MetalRenderCommand::MetalRenderCommand()
	{

	}

	MetalRenderCommand::~MetalRenderCommand()
	{

	}

	void MetalRenderCommand::Begin()
	{
        ASSERT(m_pRenderPass);
        if (m_pRenderPass == NULL)
            return;
        
        m_encoder = [m_pRenderPass->m_parallelRCE renderCommandEncoder];
	
        m_pCurIB = NULL;
        m_pPreVB = NULL;
        m_preDS = 0;
        m_prePipeline = 0;
        for(UINT i = 0; i < 16; ++i)
        {
            m_preTexture[i] = 0;
            m_preSampler[i] = 0;
        }
    }

	void MetalRenderCommand::End()
	{
        [m_encoder endEncoding];
	}

	void MetalRenderCommand::SetIndexBuffer(IndexBuffer* pIB)
	{
        m_pCurIB = pIB;
	}

	void MetalRenderCommand::SetVertexBuffer(int index, VertexBuffer* pVB)
	{
        if (m_pPreVB == pVB)
            return;
        
        m_pPreVB = pVB;
        
        MetalVertexBuffer* pMetalVertexBuffer = (MetalVertexBuffer*)(pVB);
        id<MTLBuffer> vb = pMetalVertexBuffer->GetMetalVertexBuffer();
        
        [m_encoder setVertexBuffer:vb offset:0 atIndex:0];
	}
    
    void MetalRenderCommand::SetTechnique(Technique* pTech)
    {
        MetalTechnique* pVulkanTech = (MetalTechnique*)(pTech);
        
        MetalDepthStencilStateObject* pDSState = (MetalDepthStencilStateObject*)pTech->GetDepthStencilState();
        if (pDSState->m_pMetalDSState == nil)
        {
            pDSState->RT_StreamComplete();
        }
        
        if (m_preDS != pDSState->m_pMetalDSState)
        {
            [m_encoder setDepthStencilState:pDSState->m_pMetalDSState];
            
            m_preDS = pDSState->m_pMetalDSState;
        }
        
        MetalRasterizerStateObject* pRSState = (MetalRasterizerStateObject*)pTech->GetRasterizerState();
        MTLCullMode eCull = MetalMapping::get(pRSState->m_eCullMode);
        [m_encoder setCullMode:eCull];
        [m_encoder setFrontFacingWinding:MTLWindingCounterClockwise];
        
        if (m_prePipeline != pVulkanTech->m_pPipline->m_pipelineState)
        {
            [m_encoder setRenderPipelineState:pVulkanTech->m_pPipline->m_pipelineState];
            
            m_prePipeline = pVulkanTech->m_pPipline->m_pipelineState;
        }
        
        for (UINT i = 0; i < ShaderType_Number; ++i)
        {
            for (UINT j = 0; j < pTech->GetConstBufferCount((ShaderType)i); ++j)
            {
                ConstantBuffer* pCB = pTech->GetConstBufferByIndex((ShaderType)i,j);
                MetalConstantBuffer* pMlCB = (MetalConstantBuffer*)pCB;
                pMlCB->Apply(m_encoder,i != VS);
            }
        }
        
        for (uint32 i = 0; i < pTech->GetSamplerCount(); ++i)
        {
            uint32 nIndex = pTech->GetSamplerByIndex(i)->GetIndex();
            MetalSamplerStateObject* pMetalSampler = (MetalSamplerStateObject*)pTech->m_arrSampler[nIndex];
            if (pMetalSampler == NULL)
            {
                continue;
            }
            
            MetalTexture* pMetalTexure = (MetalTexture*)(pMetalSampler->GetTexture());
            
            if (m_preTexture[nIndex] != pMetalTexure->GetNative())
            {
                [m_encoder setFragmentTexture:pMetalTexure->GetNative() atIndex:nIndex];
                
                m_preTexture[nIndex] = pMetalTexure->GetNative();
            }
         
            if (m_preSampler[nIndex] != pMetalSampler->m_pImpl)
            {
                [m_encoder setFragmentSamplerState:pMetalSampler->m_pImpl atIndex:nIndex];
                
                m_preSampler[nIndex] = pMetalSampler->m_pImpl;
            }
        }
    }
    
    void MetalRenderCommand::DrawIndex(UINT nIndexStart, UINT nIndexCount, PRIMITIVE_TYPE ePrType)
    {
        MTLPrimitiveType ePrimitiveType = MetalMapping::GetPrimitiveType(ePrType);
        
        MetalIndexBuffer* pMetalIndexBuffer = (MetalIndexBuffer*)(m_pCurIB);
        id<MTLBuffer> ib = pMetalIndexBuffer->GetMetalIndexBuffer();
        
        MTLIndexType ibType = pMetalIndexBuffer->GetIndexType() == INDEX_TYPE_U16 ? MTLIndexTypeUInt16 : MTLIndexTypeUInt32;
        
        [m_encoder drawIndexedPrimitives:ePrimitiveType indexCount:nIndexCount indexType:ibType indexBuffer:ib indexBufferOffset:nIndexStart];
    }
}



