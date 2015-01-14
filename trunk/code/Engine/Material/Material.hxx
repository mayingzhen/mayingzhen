#include "Material.h"
#include "ShaderProgram.h"
#include "MaterialData.h"

namespace ma
{

	static const char* ShadowDepth = "ShadowDepth";
	static const char* Gbuffer = "GBuffer";
	static const char* Shading = "Shading";

	void SubMaterial::InitWithSubMatData(const SubMaterialData& subMatData)
	{
		Technique* pTech = LoadTechnique(subMatData.GetShaderName(),subMatData.GetShaderMacro());

		pTech->SetRenderState(subMatData.GetRenderState());

		for (UINT i = 0; i < subMatData.GetParameterCount(); ++i)
		{
			const MaterialParameter& matParam = subMatData.GetParameterByIndex(i);
			
			pTech->SetParameter(matParam.GetName(),matParam.GetValue());
		}	
	}

	Technique*	SubMaterial::GetTechnqiueByIndex(UINT index)
	{
		ASSERT(index >=0 && index < m_arrTechnique.size());
		if (index < 0 || index >= m_arrTechnique.size() )
			return NULL;

		return m_arrTechnique[index].get();
	}

	Technique* SubMaterial::GetShadowDepthTechnqiue()
	{
		return GetTechnqiueByName(ShadowDepth);
	}

	Technique* SubMaterial::GetGbufferTechnqiue()
	{
		return GetTechnqiueByName(Gbuffer);
	}

	Technique* SubMaterial::GetShadingTechnqiue()
	{
		return GetTechnqiueByName(Shading);
	}

	Technique* SubMaterial::GetTechnqiueByName(const char* pTechName)
	{
		ASSERT(pTechName);
		if (pTechName == NULL)
			return NULL;

		for (UINT i = 0; i < m_arrTechnique.size(); ++i)
		{
			Technique* pTech = m_arrTechnique[i].get();
			ASSERT(pTech);
			if (pTech == NULL)
				continue;

			if (strcmp(pTechName,pTech->GetTechName()) == 0 )
			{
				return pTech;
			}
		}

		return NULL;
	}

	void SubMaterial::AddTechnique(Technique* pTechnique)
	{
		m_arrTechnique.push_back(pTechnique);
	}

	Technique* SubMaterial::LoadTechnique(const char* sShaderName,const char* sMatFlag)
	{
		std::string strShadingDefine = sMatFlag;
		if ( GetRenderSetting()->m_bDefferLight )
		{
			strShadingDefine += ";DeferredLight;";
		}

		//if ( GetRenderShadowCSM()->GetEnabled() )
		{
			Technique* pTech = NULL;
			if ( GetDeviceCapabilities()->GetDepthTextureSupported() )
			{
				strShadingDefine += ";HWPCF";
				pTech = AddTechnique(ShadowDepth,ShadowDepth,strShadingDefine.c_str());
				pTech->GetRenderState().m_bColorWrite = false;
			}
			else
			{
				pTech = AddTechnique(ShadowDepth,ShadowDepth,sMatFlag);
			}
			pTech->GetRenderState().m_fDepthBias = 0.002f;
			pTech->GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT; 
		}

		if ( GetRenderSetting()->m_bDefferLight )
		{
			if (GetDeviceCapabilities()->GetDepthTextureSupported())
			{
				std::string strTemp = sMatFlag;
				strTemp += ";HWDEPTH";
				AddTechnique(Gbuffer,Gbuffer,strTemp.c_str());
			}
			else
			{
				AddTechnique(Gbuffer,Gbuffer,sMatFlag);
			}
		}

		return AddTechnique(Shading,sShaderName,strShadingDefine.c_str());
	}

	Technique*	SubMaterial::AddTechnique(const char* pTechName,const char* pShadrName, const char* pDefine)
	{
		RefPtr<Technique> pTechnique = CreateTechnique(pTechName,pShadrName,pShadrName,pDefine);
		m_arrTechnique.push_back(pTechnique.get());
		return pTechnique.get();
	}

	RefPtr<SubMaterial> SubMaterial::Clone()
	{
		SubMaterial* pClonMaterial = new SubMaterial();

		for (uint32 i = 0; i < m_arrTechnique.size(); ++i)
		{
			pClonMaterial->AddTechnique( m_arrTechnique[i]->Clone().get() );
		}

		return pClonMaterial;
	}

	void SubMaterial::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);
		

		sl.EndSection();
	}

	RefPtr<SubMaterial> CreateSubMaterial()
	{
		SubMaterial* pMaterial = new SubMaterial();

		return pMaterial;
	}
}
