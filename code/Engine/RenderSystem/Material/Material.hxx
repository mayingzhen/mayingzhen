#include "Material.h"
#include "ShaderProgram.h"


namespace ma
{

	Material::Material() 
	{
		m_pEffect = NULL;
		m_pDiffuse = NULL;
		m_pBumpmap = NULL;
		m_pCustom = NULL;
	}

	Material::~Material()
	{
		//SAFE_DELETE(m_pShaderProgram);
	}


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

	void Material::LoadEffect(const std::string& sShaderName,const std::string& sMatFlag)
	{
		m_strMaterialFlag = sMatFlag;

		std::string strShaderDefine = sMatFlag;

		if ( GetRenderSystem()->IsDefferLight() )
		{
			strShaderDefine += ";DeferredLight;";
		}

		SAFE_DELETE(m_pEffect);
		m_pEffect = new Effect(sShaderName.c_str());
		m_pEffect->AddTechnique("Shading",sShaderName.c_str(),strShaderDefine.c_str());

		if ( GetRenderSystem()->IsShadow() )
		{
			//m_pEffect->AddTechnique("ShadowDepth","ShadowDepth",strShaderDefine.c_str());
		}

		if ( GetRenderSystem()->IsDefferLight() )
		{
			m_pEffect->AddTechnique("Gbuffer","Gbuffer",strShaderDefine.c_str());
		}
	}

	void Material::SetDiffuse(const char* pDiffPath, Wrap eWrap, FilterOptions eFilter)
	{
		if (m_pDiffuse == NULL)
		{
			m_pDiffuse = new SamplerState();
		}

		m_pDiffuse->SetTexture(pDiffPath);
		m_pDiffuse->SetWrapMode(eWrap);
		m_pDiffuse->SetFilterMode(eFilter);

		m_pEffect->GetParameter("u_texture")->setSampler(m_pDiffuse);
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

			LoadEffect(strShader,m_strMaterialFlag);
	
			// Texture
			SAFE_DELETE(m_pDiffuse);
			m_pDiffuse = new SamplerState();
			m_pDiffuse->Serialize(sl,"Diffuse");
			
			m_pEffect->GetParameter("u_texture")->setSampler(m_pDiffuse);

			// Param
			UINT nSize = 0;
			sl.Serialize(nSize,"arrParamSize");
			for (UINT nCnt = 0; nCnt < nSize; ++nCnt)
			{
				std::string sParamName;
				sl.Serialize(sParamName,"ParameterName");

				MaterialParameter* pParam = m_pEffect->GetParameter(sParamName.c_str());
				m_parameters.push_back(pParam);

				pParam->Serialize(sl,"ParamValue");
			}
		}
		else
		{	
			std::string strShader = m_pEffect->GetEffectName();

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
