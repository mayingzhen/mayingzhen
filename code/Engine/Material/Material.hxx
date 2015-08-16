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

		for (uint32 i = 0; i < m_arrParameters.size(); ++i)
		{
			m_pShadingTech->SetParameter(m_arrParameters[i].GetName(),m_arrParameters[i].GetValue());
		}
	}

	void SubMaterial::SetShadingTechnqiue(const char* pShaderName, const char* pDefine)
	{
		m_strShaderName = pShaderName;
		m_strShaderMacro = pDefine;

		SetShadingTechnqiue( CreateTechnique(Shading, pShaderName, pShaderName, pDefine).get() );
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

		for (uint32 i = 0; i < m_arrParameters.size(); ++i)
		{
			m_pShadowDepthTech->SetParameter(m_arrParameters[i].GetName(),m_arrParameters[i].GetValue());
		}
	}

	void SubMaterial::SetShadowDepthTechnqiue(const char* pShaderName,const char* pDefine)
	{
		string strMacro = pDefine ? pDefine : "";
		strMacro += m_pShadingTech->GetShaderProgram()->GetShaderMacro();

		SetShadowDepthTechnqiue( CreateTechnique(ShadowDepth, pShaderName, pShaderName, strMacro.c_str() ).get() );
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
		if (pXmlShadowDepthTech)
		{
			m_pShadowDepthTech = new Technique();
			m_pShadowDepthTech->Improt(pXmlShadowDepthTech);
		}

		TiXmlElement* pXmlParameter = pXmlElem->FirstChildElement("Parameters");
		while(pXmlParameter)
		{
			Parameter parameter;
			parameter.Improt(pXmlParameter);
			m_arrParameters.push_back(parameter);

			SetParameter(parameter.GetName(),parameter.GetValue());
			
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

	void SubMaterial::SetParameter(const char* pszName,const Any& value)	
	{
		Parameter* pParame = GetParameter(pszName);
		if (pParame)
		{
			pParame->SetValue(value);
		}
		else
		{
			Parameter matParam;
			matParam.SetName(pszName);
			matParam.SetValue(value);
			m_arrParameters.push_back(matParam);
		}

		if (m_pShadingTech)
			m_pShadingTech->SetParameter(pszName,value);

		if (m_pShadowDepthTech)
			m_pShadowDepthTech->SetParameter(pszName,value);
	}

	Parameter* SubMaterial::GetParameter(const char* pszName)
	{
		ASSERT(pszName);
		if (pszName == NULL)
			return NULL;

		for (UINT i = 0; i < m_arrParameters.size(); ++i)
		{
			if (strcmp(m_arrParameters[i].GetName(), pszName) == 0)
			{
				return &m_arrParameters[i];
			}
		}

		return NULL;
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
