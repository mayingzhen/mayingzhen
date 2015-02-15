#include "Material.h"
#include "ShaderProgram.h"
#include "MaterialData.h"

namespace ma
{

	static const char* ShadowDepth = "ShadowDepth";
	static const char* Shading = "Shading";

	void SubMaterial::InitWithSubMatData(const SubMaterialData& subMatData)
	{
		SetShadingTechnqiue(subMatData.GetShaderName(), subMatData.GetShaderMacro());

		m_pShadingTech->SetRenderState(subMatData.GetRenderState());

		for (UINT i = 0; i < subMatData.GetParameterCount(); ++i)
		{
			const Parameter& matParam = subMatData.GetParameterByIndex(i);
			m_pShadingTech->SetParameter(matParam.GetName(), matParam.GetValue());
		}	
	}

	void SubMaterial::SetShadingTechnqiue(Technique* pTech)
	{
		m_pShadingTech = pTech;
	}

	void SubMaterial::SetShadingTechnqiue(const char* pShaderName, const char* pDefine)
	{
		m_pShaderMarco = pDefine;
		m_pShadingTech = CreateTechnique(Shading, pShaderName, pShaderName, pDefine);
	}

	Technique* SubMaterial::GetShadowDepthTechnqiue()
	{
		if (m_pShadowDepthTech == NULL)
		{
			m_pShadowDepthTech = CreateTechnique(ShadowDepth, ShadowDepth, ShadowDepth, m_pShaderMarco.c_str());
			m_pShadowDepthTech->GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;
		
			if (GetDeviceCapabilities()->GetDepthTextureSupported())
			{
				m_pShadowDepthTech->GetRenderState().m_bColorWrite = false;
			}
		}

		return m_pShadowDepthTech.get();
	}

	Technique* SubMaterial::GetShadingTechnqiue()
	{
		return m_pShadingTech.get();
	}


	RefPtr<SubMaterial> SubMaterial::Clone()
	{
		SubMaterial* pClonMaterial = new SubMaterial();

		if (m_pShadingTech)
			pClonMaterial->m_pShadingTech = m_pShadingTech->Clone();

		if (m_pShadowDepthTech)
		{
			pClonMaterial->m_pShadowDepthTech = m_pShadowDepthTech->Clone();
		}

		pClonMaterial->m_pShaderMarco = m_pShaderMarco;

		return pClonMaterial;
	}


	RefPtr<SubMaterial> CreateSubMaterial()
	{
		SubMaterial* pMaterial = new SubMaterial();

		return pMaterial;
	}
}
