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
        MetalRasterizerStateObject* rs = (MetalRasterizerStateObject*)this->GetRasterizerState();

        MetalBlendStateObject* bs = (MetalBlendStateObject*)this->GetBlendState();
        
        MetalDepthStencilStateObject* ds = (MetalDepthStencilStateObject*)this->GetDepthStencilState();
        
        rs->RT_StreamComplete();
        
        bs->RT_StreamComplete();
        
        ds->RT_StreamComplete();
        
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
}



