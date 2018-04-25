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

		uint32_t nUniformCount = src->GetUniformCount();
		for (uint32_t i = 0; i < nUniformCount; ++i)
		{
			Uniform* pUniform = src->GetUniformByIndex(i);

			RefPtr<Uniform> pUniformCopy = pConstantBuffer->AddUniform(pUniform->GetName());
			pUniformCopy->SetIndex(pUniform->GetIndex());
			pUniformCopy->SetSize(pUniform->GetSize());
			pUniformCopy->SetOffset(pUniform->GetOffset());
		}

		return pConstantBuffer;
	}

    void MetalTechnique::RT_SetSampler(ma::Uniform *pUniform, ma::SamplerState *pSampler)
    {
        
    }
    
	void MetalTechnique::RT_StreamComplete()
	{
        MetalRasterizerStateObject* rs = (MetalRasterizerStateObject*)this->GetRasterizerState();

        MetalBlendStateObject* bs = (MetalBlendStateObject*)this->GetBlendState();
        
        MetalDepthStencilStateObject* ds = (MetalDepthStencilStateObject*)this->GetDepthStencilState();
        
        rs->RT_StreamComplete();
        
        bs->RT_StreamComplete();
        
        ds->RT_StreamComplete();
        
        MetalShaderProgram* pShader = (MetalShaderProgram*)this->GetShaderProgram();
        
        m_pPipline = CreateMetalPipeline(this);

		for (uint32_t i = 0; i < ShaderType_Number; ++i)
		{
			for (uint32_t j = 0; j < pShader->GetConstBufferCount((ShaderType)i); ++j)
			{
				ConstantBuffer* pShaderCS = pShader->GetConstBufferByIndex((ShaderType)i,j);
                RefPtr<MetalConstantBuffer> pConstantBuffer = CloneConstBuffer(pShaderCS);
				pConstantBuffer->SetParent(this);
				this->AddConstBuffer((ShaderType)i,pConstantBuffer.get());
			}
		}

		for (uint32_t i = 0; i < pShader->GetSamplerCount(); ++i)
		{
			Uniform* pUniform = pShader->GetSamplerByIndex(i);

			RefPtr<Uniform> pUniformCopy = CreateUniform(pUniform->GetName());
			pUniformCopy->SetTechnique(this);
			pUniformCopy->SetIndex(pUniform->GetIndex());

			this->AddSampler(pUniformCopy.get());
		}

		BindUniform(nullptr);
        
        for (uint32_t i = 0; i < this->GetSamplerCount(); ++i)
        {
            uint32_t nIndex = this->GetSamplerByIndex(i)->GetIndex();
            MetalSamplerStateObject* pSampler = (MetalSamplerStateObject*)this->GetActiveSampler(nIndex);
            if (pSampler == NULL)
                continue;
            
            pSampler->RT_StreamComplete();
        }

		return;
	}
}



