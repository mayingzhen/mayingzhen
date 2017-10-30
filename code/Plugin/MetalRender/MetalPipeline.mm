#include "MetalPipeline.h"
#include "MetalShaderProgram.h"
#include "MetalVertexDeclaration.h"
#include "MetalRenderDevice.h"
#include "MetalRenderPass.h"
#include "MetalRenderState.h"
#include "MetalTexture.h"
#include "MetalTechniqueh.h"

namespace ma
{
	MetalPipeline::MetalPipeline()
	{

	}

	MetalPipeline::~MetalPipeline()
	{

	}

    void MetalPipeline::RT_StreamComplete(MetalTechnique* pTech)
    {
        MetalShaderProgram* pShader = (MetalShaderProgram*)pTech->GetShaderProgram();
        
        MetalVertexDeclaration* pMetalVertexDec = (MetalVertexDeclaration*)( pShader->GetVertexDeclaration() );
        ASSERT(pMetalVertexDec);
        if (pMetalVertexDec)
        {
            if (pMetalVertexDec->m_descriptor == nil)
            {
                pMetalVertexDec->RT_StreamComplete();
            }
        }
        
        MetalRenderPass* pRenderPass = (MetalRenderPass*)pTech->GetRenderPass();
        
        //MetalBlendStateObject* pBlendState = (MetalBlendStateObject*)pTech->GetBlendState();
        
        //  create a reusable pipeline state
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"MyPipeline";
        [pipelineStateDescriptor setSampleCount: 1];
        [pipelineStateDescriptor setVertexFunction:pShader->m_pVertexShader];
        [pipelineStateDescriptor setFragmentFunction:pShader->m_pPiexelShader];
        [pipelineStateDescriptor setVertexDescriptor:pMetalVertexDec->m_descriptor];
        
        for (UINT i = 0 ; i < MAX_RENDERTARGETS; ++i)
        {
            Texture* pRT = pRenderPass->m_arrColor[i].get();
            if (pRT == NULL)
            {
                pipelineStateDescriptor.colorAttachments[i].pixelFormat = MTLPixelFormatInvalid;
                continue;
            }
            
            MetalTexture* pMetalRT = (MetalTexture*)(pRT);
            pipelineStateDescriptor.colorAttachments[i].pixelFormat = pMetalRT->m_descFormat;
        }
        
        if (pRenderPass->m_pDepthStencil)
        {
            MetalTexture* pMetalRT = (MetalTexture*)(pRenderPass->m_pDepthStencil.get());
            
            pipelineStateDescriptor.depthAttachmentPixelFormat = pMetalRT->m_descFormat;
        }
        
        
        bool blending = false;
        if (blending)
        {
            //Enable Blending
            pipelineStateDescriptor.colorAttachments[0].blendingEnabled = YES;
            pipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
            pipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
            pipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorOne;
            pipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorOne;
            pipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
            pipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
        }
        
        NSError* ns_error = nil;
        m_pipelineState = [GetMetalDevive() newRenderPipelineStateWithDescriptor:pipelineStateDescriptor options:MTLPipelineOptionArgumentInfo reflection:&m_pReflection error:&ns_error];
        if (m_pipelineState == nil && ns_error != nil)
        {
            LogError("newRenderPipeline error: %s", ns_error.localizedDescription.UTF8String);
        }
        
        pShader->ParseUniform(m_pReflection);
    }
    
    
    struct InfoKey
    {
        ShaderProgram* m_pShader;
        
        const BlendState* m_pBlendState;
        
        const DepthStencilState* m_pDSState;
        
        const RasterizerState* m_pRSState;
        
        bool operator < (const InfoKey& other) const
        {
            if (m_pShader != other.m_pShader)
                return m_pShader < other.m_pShader;
            
            // 			if (m_pBlendState != other.m_pBlendState)
            // 				return m_pBlendState < other.m_pBlendState;
            //
            // 			if (m_pDSState != other.m_pDSState)
            // 				return m_pDSState < other.m_pDSState;
            // 
            // 			if (m_pRSState != other.m_pRSState)
            // 				return m_pRSState < other.m_pRSState;
            
            return false;
        }
    };
    
    std::map< InfoKey, RefPtr<MetalPipeline> > g_mapPiplinePool;
    
    RefPtr<MetalPipeline> CreateMetalPipeline(MetalTechnique* pTech)
    {
        InfoKey key;
        key.m_pShader = pTech->GetShaderProgram();
        key.m_pBlendState = pTech->GetBlendState();
        key.m_pDSState = pTech->GetDepthStencilState();
        auto it = g_mapPiplinePool.find(key);
        if (it != g_mapPiplinePool.end())
        {
            return it->second;
        }
        else
        {
            RefPtr<MetalPipeline> pPP = new MetalPipeline();
            pPP->RT_StreamComplete(pTech);
            
            g_mapPiplinePool.insert(std::make_pair(key, pPP));
            
            return pPP;
        }
    }
    
}

