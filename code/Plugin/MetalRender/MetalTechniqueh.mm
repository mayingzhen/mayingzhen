#include "MetalTechniqueh.h"
#include "MetalConstantBuffer.h"
#include "MetalRenderState.h"
#include "MetalShaderProgram.h"
#include "MetalRenderCommand.h"
#include "MetalSamplerState.h"
#include "MetalMapping.h"
#include "MetalTexture.h"
#include "MetalPipeline.h"

#include "../../Engine/Material/ParameterManager.h"

namespace ma
{

	MetalTechnique::MetalTechnique()
	{
	}

	MetalTechnique::~MetalTechnique()
	{
	}

    RefPtr<MetalConstantBuffer> CloneConstBuffer(ConstantBuffer* src)
	{
        RefPtr<MetalConstantBuffer> pConstantBuffer = new MetalConstantBuffer();
        pConstantBuffer->SetBound(src->GetBound());
        pConstantBuffer->SetSize(src->GetSize());
		pConstantBuffer->SetName(src->GetName());
        pConstantBuffer->Create();

		uint32 nUniformCount = src->GetUniformCount();
		for (UINT i = 0; i < nUniformCount; ++i)
		{
			Uniform* pUniform = src->GetUniformByIndex(i);

			RefPtr<Uniform> pUniformCopy = pConstantBuffer->AddUniform(pUniform->GetName());
			pUniformCopy->SetIndex(pUniform->GetIndex());
			pUniformCopy->SetSize(pUniform->GetSize());
			pUniformCopy->SetOffset(pUniform->GetOffset());

			GetParameterManager()->UseDefaultBing(pUniformCopy.get());
		}

		return pConstantBuffer;
	}

	void MetalTechnique::RT_StreamComplete()
	{
        /*
        MetalRasterizerStateObject* rs = (MetalRasterizerStateObject*)this->GetRasterizerState();

        MetalBlendStateObject* bs = (MetalBlendStateObject*)this->GetBlendState();
        
        MetalDepthStencilStateObject* ds = (MetalDepthStencilStateObject*)this->GetDepthStencilState();
        
        rs->RT_StreamComplete();
        
        bs->RT_StreamComplete();
        
        ds->RT_StreamComplete();
         */
        
        MetalShaderProgram* pShader = (MetalShaderProgram*)this->GetShaderProgram();
        
        m_pPipline = CreateMetalPipeline(this);

		for (UINT i = 0; i < ShaderType_Number; ++i)
		{
			for (UINT j = 0; j < pShader->GetConstBufferCount((ShaderType)i); ++j)
			{
				ConstantBuffer* pShaderCS = pShader->GetConstBufferByIndex((ShaderType)i,j);
                RefPtr<MetalConstantBuffer> pConstantBuffer = CloneConstBuffer(pShaderCS);
				pConstantBuffer->SetParent(this);
				this->AddConstBuffer((ShaderType)i,pConstantBuffer.get());
			}
		}

		for (uint32 i = 0; i < pShader->GetSamplerCount(); ++i)
		{
			Uniform* pUniform = pShader->GetSamplerByIndex(i);

			RefPtr<Uniform> pUniformCopy = CreateUniform(pUniform->GetName());
			pUniformCopy->SetTechnique(this);
			pUniformCopy->SetIndex(pUniform->GetIndex());

			this->AddSampler(pUniformCopy.get());

			GetParameterManager()->UseDefaultBing(pUniformCopy.get());
		}

		BindUniform();
        
        for (uint32 i = 0; i < this->GetSamplerCount(); ++i)
        {
            uint32 nIndex = this->GetSamplerByIndex(i)->GetIndex();
            MetalSamplerStateObject* pSampler = (MetalSamplerStateObject*)m_arrSampler[nIndex];
            if (pSampler == NULL)
                continue;
            
            pSampler->RT_StreamComplete();
        }

		return;
	}

	void MetalTechnique::CommitChanges(RenderCommand* pCmd)
	{
		MetalRenderCommand* pMlCmd = (MetalRenderCommand*)pCmd;
        
        MetalDepthStencilStateObject* pDSState = (MetalDepthStencilStateObject*)this->GetDepthStencilState();
        if (pDSState->m_pMetalDSState == nil)
        {
            pDSState->RT_StreamComplete();
        }
        [pMlCmd->m_encoder setDepthStencilState:pDSState->m_pMetalDSState];

        MetalRasterizerStateObject* pRSState = (MetalRasterizerStateObject*)this->GetRasterizerState();
        MTLCullMode eCull = MetalMapping::get(pRSState->m_eCullMode);
        [pMlCmd->m_encoder setCullMode:eCull];
        [pMlCmd->m_encoder setFrontFacingWinding:MTLWindingCounterClockwise];
  
        [pMlCmd->m_encoder setRenderPipelineState:m_pPipline->m_pipelineState];
        
        for (UINT i = 0; i < ShaderType_Number; ++i)
        {
            for (UINT j = 0; j < this->GetConstBufferCount((ShaderType)i); ++j)
            {
                ConstantBuffer* pCB = this->GetConstBufferByIndex((ShaderType)i,j);
                MetalConstantBuffer* pMlCB = (MetalConstantBuffer*)pCB;
                pMlCB->Apply(pMlCmd->m_encoder,i != VS);
            }
        }
        
        for (uint32 i = 0; i < this->GetSamplerCount(); ++i)
        {
            uint32 nIndex = this->GetSamplerByIndex(i)->GetIndex();
            MetalSamplerStateObject* pMetalSampler = (MetalSamplerStateObject*)m_arrSampler[nIndex];
            if (pMetalSampler == NULL)
            {
                continue;
            }
            
            MetalTexture* pMetalTexure = (MetalTexture*)(pMetalSampler->GetTexture());
            
            [pMlCmd->m_encoder setFragmentTexture:pMetalTexure->GetNative() atIndex:nIndex];
            
            [pMlCmd->m_encoder setFragmentSamplerState:pMetalSampler->m_pImpl atIndex:nIndex];
        }
	}
}



