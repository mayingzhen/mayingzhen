#include "Material.h"
#include "ShaderProgram.h"

namespace ma
{

	static const char* ShadowDepth = "ShadowDepth";
	static const char* Shading = "Shading";
	
	SubMaterial::SubMaterial()
	{
	}

	SubMaterial::~SubMaterial()
	{
		m_pShadingTech = NULL;
	}

	void SubMaterial::SetShadingTechnqiue(Technique* pTech)
	{
		m_pShadingTech = pTech;
	}

	void SubMaterial::SetShadingTechnqiue(const char* pShaderName, const char* pDefine)
	{
		m_strShaderName = pShaderName;
		m_strShaderMacro = pDefine;
		m_pShadingTech = CreateTechnique(Shading, pShaderName, pShaderName, pDefine);
	}

	Technique* SubMaterial::GetShadowDepthTechnqiue()
	{
		if (m_pShadowDepthTech == NULL)
		{
			string strShaderMacro = m_pShadingTech->GetShaderProgram()->GetShaderMacro();
			m_pShadowDepthTech = CreateTechnique(ShadowDepth, ShadowDepth, ShadowDepth, strShaderMacro.c_str());
			//m_pShadowDepthTech->GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;
		
			if (GetDeviceCapabilities()->GetDepthTextureSupported())
			{
				m_pShadowDepthTech->GetRenderState().SetColorWrite(false);
			}
		}

		return m_pShadowDepthTech.get();
	}

	void SubMaterial::SetShadowDepthTechnqiue(Technique* pTech)
	{
		m_pShadowDepthTech = pTech;
	}

	void SubMaterial::SetShadowDepthTechnqiue(const char* pShaderName,const char* pDefine)
	{
		string strMacro = pDefine ? pDefine : "";
		strMacro += m_pShadingTech->GetShaderProgram()->GetShaderMacro();
		m_pShadowDepthTech = CreateTechnique(ShadowDepth, pShaderName, pShaderName, strMacro.c_str());
	}

	Technique* SubMaterial::GetShadingTechnqiue()
	{
		return m_pShadingTech.get();
	}

	void SubMaterial::Improt(TiXmlElement* pXmlElem)
	{
		TiXmlElement* pXmlShadingTech = pXmlElem->FirstChildElement("ShadingTech");
		if (pXmlShadingTech)
		{
			m_pShadingTech = new Technique();
			m_pShadingTech->Improt(pXmlShadingTech);
		}

		TiXmlElement* pXmlShadowDepthTech = pXmlElem->FirstChildElement("ShadowDepthTech");
		{
			m_pShadowDepthTech = new Technique();
			m_pShadowDepthTech->Improt(pXmlShadingTech);
		}

		TiXmlElement* pXmlParameter = pXmlElem->FirstChildElement("Parameters");
		while(pXmlParameter)
		{
			Parameter parameter;
			parameter.Improt(pXmlParameter);
			m_arrParameters.push_back(parameter);
			
			pXmlParameter = pXmlParameter->NextSiblingElement("Parameters");
		}
	}

	void SubMaterial::Export(TiXmlElement* pXmlElem)
	{
		if (m_pShadingTech)
		{
			TiXmlElement* pXmlShadingTech = new TiXmlElement("ShadingTech");
			pXmlElem->LinkEndChild(pXmlShadingTech);

			m_pShadingTech->Export(pXmlShadingTech);
		}

		if (m_pShadowDepthTech)
		{
			TiXmlElement* pXmlShadowDepthTech = new TiXmlElement("ShadowDepthTech");
			pXmlElem->LinkEndChild(pXmlShadowDepthTech);

			m_pShadingTech->Export(pXmlShadowDepthTech);	
		}

		for (UINT i = 0; i < m_arrParameters.size(); ++i)
		{
			TiXmlElement* pXmlParameter = new TiXmlElement("Parameters");
			pXmlElem->LinkEndChild(pXmlParameter);

			Parameter& parameter = m_arrParameters[i];
			parameter.Export(pXmlParameter);
		}
	}

	void SubMaterial::SetShaderName(const char* pszSharName)
	{
		m_strShaderName = pszSharName ? pszSharName : "";
	}

	void SubMaterial::SetShderMacro(const char* pszShaderMacro)
	{
		m_strShaderMacro = pszShaderMacro ? pszShaderMacro : "";
	}

	void SubMaterial::AddParameter(const char* pName,Any value)
	{
		Parameter matParam;
		matParam.SetName(pName);
		matParam.SetValue(value);
		m_arrParameters.push_back(matParam);
	}

	RefPtr<SubMaterial> SubMaterial::Clone()
	{
		SubMaterial* pClonMaterial = new SubMaterial();

		if (m_pShadingTech)
		{
			pClonMaterial->m_pShadingTech = m_pShadingTech->Clone();
		}

		if (m_pShadowDepthTech)
		{
			pClonMaterial->m_pShadowDepthTech = m_pShadowDepthTech->Clone();
		}
		pClonMaterial->m_strShaderMacro = m_strShaderMacro;

		return pClonMaterial;
	}


	RefPtr<SubMaterial> CreateSubMaterial()
	{
		SubMaterial* pMaterial = new SubMaterial();

		return pMaterial;
	}


	IMPL_OBJECT(Material,Serializable);

	Material::Material()
	{
	}

	Material::~Material()
	{
	}

	void Material::AddSubMaterial(SubMaterial* pSubMaterial)
	{
		m_arrSubMaterial.push_back(pSubMaterial);
	}


	void Material::Improt(TiXmlElement* pXmlElem)
	{
		TiXmlElement* pXmlSubMaterial = pXmlElem->FirstChildElement("SubMaterial");
		while(pXmlSubMaterial)
		{
			RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
			this->AddSubMaterial(pSubMaterial.get());

			pSubMaterial->Improt(pXmlSubMaterial);

			pXmlSubMaterial = pXmlSubMaterial->NextSiblingElement("SubMaterial");
		}
	}

	void Material::Export(TiXmlElement* pXmlElem)
	{
		for (UINT i = 0; i < m_arrSubMaterial.size(); ++i)
		{
			TiXmlElement* pXmlSubMaterial = new TiXmlElement("SubMaterial");
			pXmlElem->LinkEndChild(pXmlSubMaterial);

			SubMaterial* pSubMaterial = m_arrSubMaterial[i].get();
			pSubMaterial->Export(pXmlSubMaterial);
		}

	}

	RefPtr<Material> CreateMaterial()
	{
		return new Material();
	}

	RefPtr<Material> CreateMaterial(const char* pszPath)
	{
		RefPtr<Material> pMaterial = new Material();
		pMaterial->LoadFromXML(pszPath);
		return pMaterial;
	}


}
