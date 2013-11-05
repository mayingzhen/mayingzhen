#include "Material.h"
#include "ShaderProgram.h"


namespace ma
{
	IMPL_OBJECT(Material,Resource);

	Material::Material() 
	{
		//m_pEffect = NULL;
		m_pDiffuse = NULL;
		m_pBumpmap = NULL;
		m_pCustom = NULL;
	}

	Material::~Material()
	{
		//SAFE_DELETE(m_pEffect);
	}

// 	Effect*	Material::CreateEffect(const char* pszEffectName)
// 	{
// 		m_pEffect = new Effect(pszEffectName);
// 		return m_pEffect;
// 	}

	MaterialParameter* Material::GetParameter(const char* name) 
	{
		ASSERT(name);

		// Search for an existing parameter with this name.
		MaterialParameter* param;
		for (size_t i = 0, count = m_parameters.size(); i < count; ++i)
		{
			param = m_parameters[i];
			ASSERT(param);
			if (strcmp(param->getName(), name) == 0)
			{
				return param;
			}
		}

		//ASSERT(false);
		return NULL;
	}

	MaterialParameter*	Material::AddParameter(const char* name)
	{
		MaterialParameter* pParame = GetParameter(name);
		if (pParame)
			return pParame;

		pParame = new MaterialParameter(name);
		m_parameters.push_back(pParame);
		
		return pParame;
	}


	void Material::SaveToFile(const char* pszPath)
	{
		std::string strSavePath = pszPath ? pszPath : m_sResPath;

		XMLOutputArchive ar;
		bool bLoadOK = ar.Open(strSavePath.c_str());
		if (!bLoadOK)
		{
			ASSERT(false && "Fail to save to file");
			return ;
		}

		Serialize(ar);

		ar.Close();
	}

	bool Material::LoadFileToMemeory()
	{
		return true;
	}

	bool Material::CreateFromMemeory()
	{
		std::string strPath = FileSystem::getFullPath(m_sResPath.c_str());

		XMLInputArchive ar;
		bool bLoadOK = ar.Open(strPath.c_str());
		if (!bLoadOK)
		{
			ASSERT(false && "Fail to Load to file");
			return  false;
		}

		Serialize(ar);

		ar.Close();

		return true;
	}

	Technique*	Material::GetTechnqiue(const char* pTechName)
	{
		ASSERT(pTechName);
		if (pTechName == NULL)
			return NULL;

		for (Uint i = 0; i < m_arrTechnique.size(); ++i)
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
		m_strMaterialFlag = sMatFlag;

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

	void Material::SetDiffuse(const char* pDiffPath, Wrap eWrap, FilterOptions eFilter)
	{
		Texture* pTexture = LoadResourceASync<Texture>(pDiffPath);
		SetDiffuse(pTexture,eWrap,eFilter);

// 		if (m_pDiffuse == NULL)
// 		{
// 			m_pDiffuse = new SamplerState();
// 		}
// 
// 		m_pDiffuse->SetTexture(pDiffPath);
// 		m_pDiffuse->SetWrapMode(eWrap);
// 		m_pDiffuse->SetFilterMode(eFilter);
// 
// 		m_pEffect->GetParameter("u_texture")->setSampler(m_pDiffuse);
	}

	void Material::SetDiffuse(Texture* pTexture, Wrap eWrap, FilterOptions eFilter)
	{
		if (m_pDiffuse == NULL)
		{
			m_pDiffuse = new SamplerState();
		}

		m_pDiffuse->SetTexture(pTexture);
		m_pDiffuse->SetWrapMode(eWrap);
		m_pDiffuse->SetFilterMode(eFilter);

		//m_pEffect->GetParameter("u_texture")->setSampler(m_pDiffuse);
		Technique* pTech = GetTechnqiue("Shading");
		pTech->GetParameter("u_texture")->setSampler(m_pDiffuse);
	}


	void Material::Serialize(Serializer& sl, const char* pszLable/* = "Material"*/)
	{
		sl.BeginSection(pszLable);
		
		if (sl.IsReading())
		{
			// shader
			std::string strShader;

			sl.Serialize(strShader,"Shader");
			sl.Serialize(m_strMaterialFlag,"MaterialFlag");

			Technique* pTech = LoadTechnique(strShader,m_strMaterialFlag);
	
			// Texture
			SAFE_DELETE(m_pDiffuse);
			m_pDiffuse = new SamplerState();
			m_pDiffuse->Serialize(sl,"Diffuse");
		
			pTech->GetParameter("u_texture")->setSampler(m_pDiffuse);

			// Param
			UINT nSize = 0;
			sl.Serialize(nSize,"arrParamSize");
			for (UINT nCnt = 0; nCnt < nSize; ++nCnt)
			{
				std::string sParamName;
				sl.Serialize(sParamName,"ParameterName");

				MaterialParameter* pParam = new MaterialParameter( sParamName.c_str() );//m_pEffect->AddParameter(sParamName.c_str());
				m_parameters.push_back(pParam);

				pParam->Serialize(sl,"ParamValue");
			}
		}
		else
		{	
			std::string strShader;

			sl.Serialize(strShader,"Shader");
			sl.Serialize(m_strMaterialFlag,"MaterialFlag");
			
			m_pDiffuse->Serialize(sl,"Diffuse");

			UINT nSize = m_parameters.size();
			sl.Serialize(nSize,"arrParamSize");
			for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
			{
				MaterialParameter* pParam = m_parameters[nCnt];

				std::string sParamName = pParam->getName();
				sl.Serialize(sParamName,"ParameterName");

				pParam->Serialize(sl,"ParamValue");
			}
	
		}


		sl.EndSection();
	}
}
