
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
#include "MetalShaderProgram.h"

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
        memset(m_pPreVB,0,sizeof(m_pPreVB));
        memset(m_nPreVBOffset,0,sizeof(m_nPreVBOffset));
        m_preDS = 0;
        m_prePipeline = 0;
        memset(m_preTexture,0,sizeof(m_preTexture));
        memset(m_preSampler,0,sizeof(m_preSampler));
    }

	void MetalRenderCommand::End()
	{
        [m_encoder endEncoding];
	}

	void MetalRenderCommand::SetIndexBuffer(IndexBuffer* pIB)
	{
        m_pCurIB = pIB;
	}

	void MetalRenderCommand::SetVertexBuffer(int index, VertexBuffer* pVB, uint32_t nOffSet)
	{
        if (m_pPreVB[index] == pVB && m_nPreVBOffset[index] == nOffSet)
            return;
        
        m_pPreVB[index] = pVB;
        m_nPreVBOffset[index] = nOffSet;
        
        MetalVertexBuffer* pMetalVertexBuffer = (MetalVertexBuffer*)(pVB);
        id<MTLBuffer> vb = pMetalVertexBuffer->GetMetalVertexBuffer();
        
        [m_encoder setVertexBuffer:vb offset:nOffSet atIndex:index];
	}
    
    void MetalRenderCommand::SetTechnique(Technique* pTech)
    {
        MetalTechnique* pVulkanTech = (MetalTechnique*)(pTech);
        
        MetalShaderProgram* pShader = (MetalShaderProgram*)pTech->GetShaderProgram();
        
        MetalDepthStencilStateObject* pDSState = (MetalDepthStencilStateObject*)(pShader->GetShaderCreateInfo().m_pDSState.get());
        if (pDSState && pDSState->m_pMetalDSState == nil)
        {
            pDSState->RT_StreamComplete();
        }
        
        if (pDSState && m_preDS != pDSState->m_pMetalDSState)
        {
            [m_encoder setDepthStencilState:pDSState->m_pMetalDSState];
            
            m_preDS = pDSState->m_pMetalDSState;
        }
        
        MetalRasterizerStateObject* pRSState = (MetalRasterizerStateObject*)(pShader->GetShaderCreateInfo().m_pRSState.get());
        if (pRSState)
        {
            MTLCullMode eCull = MetalMapping::get(pRSState->m_eCullMode);
            [m_encoder setCullMode:eCull];
            [m_encoder setFrontFacingWinding:MTLWindingCounterClockwise];
        }
        
        if (m_prePipeline != pVulkanTech->m_pPipline->m_pipelineState)
        {
            [m_encoder setRenderPipelineState:pVulkanTech->m_pPipline->m_pipelineState];
            
            m_prePipeline = pVulkanTech->m_pPipline->m_pipelineState;
        }
        
        for (uint32_t iType = 0; iType < ShaderType_Number; ++iType)
        {
            ShaderType eShader = (ShaderType)iType;
            for (uint32_t j = 0; j < pTech->GetConstBufferCount(eShader); ++j)
            {
                ConstantBuffer* pCB = pTech->GetConstBufferByIndex(eShader, j);
                MetalConstantBuffer* pMlCB = (MetalConstantBuffer*)pCB;
                pMlCB->Apply(m_encoder,iType != VS);
            }
    
            MetalTechnique* pMetalTech = (MetalTechnique*)(pTech);
            for (uint32_t j = 0; j < pMetalTech->GetSamplerCount(eShader); ++j)
            {
                Uniform* pUniform = pTech->GetSamplerByIndex(eShader,j);
                MetalSamplerStateObject* pMetalSampler = (MetalSamplerStateObject*)pMetalTech->GetActiveSampler(pUniform);
                if (pMetalSampler == NULL)
                {
                    continue;
                }
                
                pMetalSampler->RT_StreamComplete();
                
                uint32_t nIndex = pUniform->GetIndex();
            
                MetalTexture* pMetalTexure = (MetalTexture*)(pMetalSampler->GetTexture());
            
                if (m_preTexture[eShader][nIndex] != pMetalTexure->GetNative())
                {
                    if (eShader == VS)
                    {
                        [m_encoder setVertexTexture:pMetalTexure->GetNative() atIndex:nIndex];
                    }
                    else if (eShader == PS)
                    {
                        [m_encoder setFragmentTexture:pMetalTexure->GetNative() atIndex:nIndex];
                    }
                    
                    m_preTexture[eShader][nIndex] = pMetalTexure->GetNative();
                }
         
                if (m_preSampler[eShader][nIndex] != pMetalSampler->m_pImpl)
                {
                    if (eShader == VS)
                    {
                        [m_encoder setVertexSamplerState:pMetalSampler->m_pImpl atIndex:nIndex];
                    }
                    else if (eShader == PS)
                    {
                        [m_encoder setFragmentSamplerState:pMetalSampler->m_pImpl atIndex:nIndex];
                    }
                
                    m_preSampler[eShader][nIndex] = pMetalSampler->m_pImpl;
                }
            }
        }
    }

    void MetalRenderCommand::SetViewPort(const Rectangle& viewPort)
    {
        MTLViewport mtlvp =
            { viewPort.offsetX(), //originX
              viewPort.offsetY(), //origninY
              viewPort.width(), // width
              viewPort.height(),// height
              0.0, // znear
              1.0,// zfar
            };
        [m_encoder setViewport: mtlvp];
    }

    void MetalRenderCommand::SetScissor(const Rectangle& viewPort)
    {
        MTLScissorRect scissorRect =
        {
            viewPort.offsetX(),
            viewPort.offsetY(),
            viewPort.width(),
            viewPort.height(),
        };
        [m_encoder setScissorRect:scissorRect];
    }

    void MetalRenderCommand::SetDepthBounds(float fDepthNear, float fDepthFar)
    {
        
    }

    void MetalRenderCommand::DrawIndex(uint32_t nIndexStart, uint32_t nIndexCount, uint32_t nVertexStart, uint32_t nInstanceCount)
    {
        PRIMITIVE_TYPE ePrType = PRIM_TRIANGLELIST;
        MTLPrimitiveType ePrimitiveType = MetalMapping::GetPrimitiveType(ePrType);
        
        MetalIndexBuffer* pMetalIndexBuffer = (MetalIndexBuffer*)(m_pCurIB);
        id<MTLBuffer> ib = pMetalIndexBuffer->GetMetalIndexBuffer();
        
        MTLIndexType ibType = pMetalIndexBuffer->GetIndexType() == INDEX_TYPE_U16 ? MTLIndexTypeUInt16 : MTLIndexTypeUInt32;
    
        [m_encoder drawIndexedPrimitives:ePrimitiveType indexCount:nIndexCount indexType:ibType indexBuffer:ib indexBufferOffset:nIndexStart instanceCount:nInstanceCount];
    }

    void MetalRenderCommand::Draw(uint32_t nVertexStart, uint32_t nVertexCount, uint32_t nInstanceCount)
    {
        
    }

    void MetalComputeCommad::Begin()
    {

    }

    void MetalComputeCommad::End()
    {

    }

    void MetalComputeCommad::SetTechnique(Technique* pTech)
    {

    }

	void MetalComputeCommad::SetStorgeBuffer(VertexBuffer* pBuffer)
    {

    }

	void MetalComputeCommad::Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
    {

    }
}



