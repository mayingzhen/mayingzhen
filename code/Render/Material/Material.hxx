#include "Material.h"
#include "ShaderProgram.h"
#include "MaterialData.h"

namespace ma
{
	Material::Material()
	{

	}

	Material::Material(ref_ptr<MaterialData> pMatData)
	{
		m_pMatData = pMatData;

		if (pMatData)
		{
			Technique* pTech = LoadTechnique(pMatData->GetShaderName(),pMatData->GetShaderMacro());

			for (UINT i = 0; i < pMatData->GetParameterNumber(); ++i)
			{
				MaterialParameter* pParmMat = pMatData->GetParameterByIndex(i);

				MaterialParameter* pParmTech = pTech->GetParameter( pParmMat->GetName() );
				if (pParmTech)
				{
					pParmTech->setParameter(pParmMat);
				}
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

	Technique* Material::LoadTechnique(const std::string& sShaderName,const std::string& sMatFlag)
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
				std::string strShaderDefine = sMatFlag + ";HWPCF";
				pTech = AddTechnique("ShadowDepth","ShadowDepth",strShaderDefine.c_str());
				pTech->GetRenderState().m_bColorWrite = false;
			}
			else
			{
				pTech = AddTechnique("ShadowDepth","ShadowDepth",sMatFlag.c_str());
			}
			pTech->GetRenderState().m_fDepthBias = 0.002f;
			pTech->GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT; 
		}

		if ( GetRenderSetting()->m_bDefferLight )
		{
			AddTechnique("Gbuffer","Gbuffer",sMatFlag.c_str());
		}

		return AddTechnique("Shading",sShaderName.c_str(),strShadingDefine.c_str());
	}

	Technique*	Material::AddTechnique(const char* pTechName,const char* pShadrName, const char* pDefine)
	{
		Technique* pTechnique = new Technique(pTechName,pShadrName,pShadrName,pDefine);
		m_arrTechnique.push_back(pTechnique);
		return pTechnique;
	}
}
