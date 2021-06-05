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
        m_activeSamplers[pUniform] = pSampler;
    }

    void MetalTechnique::RT_SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer)
    {
        m_activeStorageBuffers[pUniform] = pBuffer;
    }

    SamplerState* MetalTechnique::GetActiveSampler(Uniform* pUniform)
    {
        auto it = m_activeSamplers.find(pUniform);
        if (it == m_activeSamplers.end())
            return nullptr;
        
        return it->second.get();
    }

    HardwareBuffer* MetalTechnique::GetActiveStorageBuffer(Uniform* pUniform)
    {
        auto it = m_activeStorageBuffers.find(pUniform);
        if (it == m_activeStorageBuffers.end())
            return nullptr;
    
        return it->second.get();
    }

	void MetalTechnique::RT_StreamComplete()
	{
        MetalShaderProgram* pShader = (MetalShaderProgram*)this->GetShaderProgram();
        
        MetalRasterizerStateObject* rs = (MetalRasterizerStateObject*)pShader->GetShaderCreateInfo().m_pRSState.get();
        
        MetalBlendStateObject* bs = (MetalBlendStateObject*)pShader->GetShaderCreateInfo().m_pBlendState.get();
        
        MetalDepthStencilStateObject* ds = (MetalDepthStencilStateObject*)pShader->GetShaderCreateInfo().m_pDSState.get();
        
        if (rs)
        {
            rs->RT_StreamComplete();
        }
        
        if (bs)
        {
            bs->RT_StreamComplete();
        }
        
        if (ds)
        {
            ds->RT_StreamComplete();
        }
        
        m_pPipline = CreateMetalPipeline(this);

		for (uint32_t iType = 0; iType < ShaderType_Number; ++iType)
		{
            ShaderType eType = (ShaderType)iType;
            
			for (uint32_t j = 0; j < pShader->GetConstBufferCount(eType); ++j)
			{
				ConstantBuffer* pShaderCS = pShader->GetConstBufferByIndex(eType,j);
                RefPtr<MetalConstantBuffer> pConstantBuffer = CloneConstBuffer(pShaderCS);
				pConstantBuffer->SetParent(this);
				this->AddConstBuffer(eType,pConstantBuffer.get());
			}

            for (uint32_t j = 0; j < pShader->GetSamplerCount(eType); ++j)
            {
                Uniform* pUniform = pShader->GetSamplerByIndex(eType,j);

                RefPtr<Uniform> pUniformCopy = CreateUniform(pUniform->GetName());
                pUniformCopy->SetTechnique(this);
                pUniformCopy->SetShaderType(eType);
                pUniformCopy->SetIndex(pUniform->GetIndex());
                pUniformCopy->SetMethodBinding(pUniform->GetMethodBinding());

                this->AddSampler(eType,pUniformCopy.get());
            }

            SceneContext sc;
            BindUniform(nullptr, &sc, eType);
        
            for (uint32_t j = 0; j < this->GetSamplerCount(eType); ++j)
            {
                uint32_t nIndex = this->GetSamplerByIndex(eType,j)->GetIndex();
                //MetalSamplerStateObject* pSampler = (MetalSamplerStateObject*)this->GetActiveSampler((ShaderType)shader,nIndex);
               // if (pSampler == NULL)
               //     continue;
            
                //pSampler->RT_StreamComplete();
            }
        }

		return;
	}
}



