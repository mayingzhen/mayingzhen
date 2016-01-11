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
		
			if (GetDeviceCapabilities()->GetD24S8Supported())
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

	void SubMaterial::Improt(rapidxml::xml_node<>* pXmlElem)
	{
		rapidxml::xml_node<>* pXmlShadingTech = pXmlElem->first_node("ShadingTech");
		if (pXmlShadingTech)
		{
			m_pShadingTech = new Technique();
			m_pShadingTech->Improt(pXmlShadingTech);
		}

		rapidxml::xml_node<>* pXmlShadowDepthTech = pXmlElem->first_node("ShadowDepthTech");
		if (pXmlShadowDepthTech)
		{
			m_pShadowDepthTech = new Technique();
			m_pShadowDepthTech->Improt(pXmlShadowDepthTech);
		}

		rapidxml::xml_node<>* pXmlParameter = pXmlElem->first_node("Parameters");
		while(pXmlParameter)
		{
			Parameter parameter;
			parameter.Improt(pXmlParameter);
			m_arrParameters.push_back(parameter);

			SetParameter(parameter.GetName(),parameter.GetValue());
			
			pXmlParameter = pXmlParameter->next_sibling("Parameters");
		}
	}

	void SubMaterial::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		if (m_pShadingTech)
		{
			rapidxml::xml_node<>* pXmlShadingTech = doc.allocate_node(rapidxml::node_element, doc.allocate_string("ShadingTech"));
			pXmlElem->append_node(pXmlShadingTech);

			m_pShadingTech->Export(pXmlShadingTech,doc);
		}

		if (m_pShadowDepthTech)
		{
			rapidxml::xml_node<>* pXmlShadowDepthTech = doc.allocate_node(rapidxml::node_element, doc.allocate_string("ShadowDepthTech"));
			pXmlElem->append_node(pXmlShadowDepthTech);

			m_pShadingTech->Export(pXmlShadowDepthTech,doc);	
		}

		for (UINT i = 0; i < m_arrParameters.size(); ++i)
		{
			rapidxml::xml_node<>* pXmlParameter = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Parameters"));
			pXmlElem->append_node(pXmlParameter);

			Parameter& parameter = m_arrParameters[i];
			parameter.Export(pXmlParameter,doc);
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


	Material::Material()
	{
	}

	Material::~Material()
	{
	}

	void Material::AddSubMaterial(uint32 nLod,SubMaterial* pSubMaterial)
	{
		if (nLod >= m_arrLodSubMaterial.size())
		{
			m_arrLodSubMaterial.resize(nLod + 1);
		}
		m_arrLodSubMaterial[nLod].push_back(pSubMaterial);
	}


	bool Material::Improt(rapidxml::xml_node<>* pXmlElem)
	{
		uint32 nLod = 0;
		rapidxml::xml_node<>* pXmlLodSubMaterial = pXmlElem->first_node("LodSubMaterial");
		while(pXmlLodSubMaterial)
		{
			rapidxml::xml_node<>* pXmlSubMaterial = pXmlLodSubMaterial->first_node("SubMaterial");
			while(pXmlSubMaterial)
			{
				RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
				this->AddSubMaterial(nLod,pSubMaterial.get());

				pSubMaterial->Improt(pXmlSubMaterial);

				pXmlSubMaterial = pXmlSubMaterial->next_sibling("SubMaterial");
			}
			pXmlLodSubMaterial = pXmlLodSubMaterial->next_sibling("LodSubMaterial");
			nLod++;
		}

		return true;
	}

	bool Material::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		for (UINT iLod = 0; iLod < m_arrLodSubMaterial.size(); ++iLod)
		{
			rapidxml::xml_node<>* pXmlLodSubMaterial = doc.allocate_node(rapidxml::node_element, doc.allocate_string("LodSubMaterial"));
			pXmlElem->append_node(pXmlLodSubMaterial);

			for (UINT iSub = 0; iSub < m_arrLodSubMaterial[iLod].size(); ++iSub)
			{
				rapidxml::xml_node<>* pXmlSubMaterial = doc.allocate_node(rapidxml::node_element, doc.allocate_string("SubMaterial"));
				pXmlLodSubMaterial->append_node(pXmlSubMaterial);

				SubMaterial* pSubMaterial = m_arrLodSubMaterial[iLod][iSub].get();
				pSubMaterial->Export(pXmlSubMaterial,doc);
			}
		}

		return true;
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
