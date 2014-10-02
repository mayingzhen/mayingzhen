#include "Material.h"
#include "ShaderProgram.h"
#include "MaterialData.h"

namespace ma
{

	void Material::InitWithSubMatData(const SubMaterialData& subMatData)
	{
		Technique* pTech = LoadTechnique(subMatData.m_strShaderName.c_str(),subMatData.m_strShaderMacro.c_str());

		pTech->SetRenderState(subMatData.m_renderState);

		for (UINT i = 0; i < subMatData.m_arrParameters.size(); ++i)
		{
			const MaterialParameter& matParam = subMatData.m_arrParameters[i];

			MaterialParameter* pParmTech = pTech->GetParameter( matParam.GetName() );
			ASSERT(pParmTech);
			if (pParmTech)
			{
				pParmTech->SetValue( matParam.GetValue() );
			}
		}	
	}

	Technique* Material::GetTechnqiue(const char* pTechName)
	{
		ASSERT(pTechName);
		if (pTechName == NULL)
			return NULL;

		for (UINT i = 0; i < m_arrTechnique.size(); ++i)
		{
			Technique* pTech = m_arrTechnique[i];
			ASSERT(pTech);
			if (pTech == NULL)
				continue;

			if ( strcmp(pTechName,pTech->GetTechName()) == 0 )
			{
				return pTech;
			}
		}

		return NULL;
	}

	void Material::AddTechnique(Technique* pTechnique)
	{
		m_arrTechnique.push_back(pTechnique);
	}

	Technique* Material::LoadTechnique(const char* sShaderName,const char* sMatFlag)
	{
		std::string strShadingDefine = sMatFlag;
		if ( GetRenderSetting()->m_bDefferLight )
		{
			strShadingDefine += ";DeferredLight;";
		}

		if ( GetRenderSetting()->m_bShadow)
		{
			Technique* pTech = NULL;
			if ( GetRenderSetting()->m_bHWShadowMap )
			{
				strShadingDefine += ";HWPCF";
				pTech = AddTechnique("ShadowDepth","ShadowDepth",strShadingDefine.c_str());
				pTech->GetRenderState().m_bColorWrite = false;
			}
			else
			{
				pTech = AddTechnique("ShadowDepth","ShadowDepth",sMatFlag);
			}
			pTech->GetRenderState().m_fDepthBias = 0.002f;
			pTech->GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT; 
		}

		if ( GetRenderSetting()->m_bDefferLight )
		{
			AddTechnique("Gbuffer","Gbuffer",sMatFlag);
		}

		return AddTechnique("Shading",sShaderName,strShadingDefine.c_str());
	}

	Technique*	Material::AddTechnique(const char* pTechName,const char* pShadrName, const char* pDefine)
	{
		Technique* pTechnique = new Technique(pTechName,pShadrName,pShadrName,pDefine);
		m_arrTechnique.push_back(pTechnique);
		return pTechnique;
	}
}
