#include "Material.h"
#include "ShaderProgram.h"

namespace ma
{
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

		for (uint32_t i = 0; i < m_arrParameters.size(); ++i)
		{
			m_pShadingTech->SetParameter(m_arrParameters[i].GetName(), m_arrParameters[i].GetValue());
		}
	}

	Technique* SubMaterial::GetShadowDepthTechnqiue()
	{
		if (m_pShadowDepthTech == NULL)
		{
			VertexDeclaration* pVertexDecl = m_pShadingTech->GetShaderProgram()->GetVertexDeclaration();

			std::vector<VertexElement> vecElement;
			for (uint32_t i = 0; i < pVertexDecl->GetElementCount(0); ++i)
			{
				vecElement.push_back(pVertexDecl->GetElement(0, i));
			}

			RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(vecElement.data(), vecElement.size());

			std::string strShaderMacro = m_pShadingTech->GetShaderProgram()->GetShaderMacro();
          
			RefPtr<BlendState> pBlendSate = CreateBlendState();
			pBlendSate->m_bColorWrite = false;

			DirectonalLight* pDirLight = GetRenderSystem()->GetScene()->GetDirLight();
			ShadowMapFrustum& shadowMap = pDirLight->GetShadowMapFrustum(0);

			ShaderCreateInfo info;
			info.m_strVSFile = "ShadowDepth";
			info.m_strPSFile = "ShadowDepth";
			info.m_shaderMacro = strShaderMacro;
			info.m_pVertexDecl = pDeclaration;
			info.m_pBlendState = pBlendSate;
			info.m_pRenderPass = shadowMap.GetShadowMapFrameBuffer();
			m_pShadowDepthTech = CreateTechnique("ShadowDepth", info);

			//m_pShadowDepthTech->SetRenderPass(shadowMap.GetShadowMapFrameBuffer());

			GetRenderSystem()->TechniqueStreamComplete(m_pShadowDepthTech.get());

			for (uint32_t i = 0; i < m_arrParameters.size(); ++i)
			{
				m_pShadowDepthTech->SetParameter(m_arrParameters[i].GetName(), m_arrParameters[i].GetValue());
			}
		}

		return m_pShadowDepthTech.get();
	}

	Technique* SubMaterial::GetShadingTechnqiue()
	{
		return m_pShadingTech.get();
	}

	Technique* SubMaterial::GetShadingInstTechnqiue()
	{
		if (m_pShadingInstTech == NULL)
		{
			m_pShadingInstTech = m_pShadingTech->CreateInstTech();

			for (uint32_t i = 0; i < m_arrParameters.size(); ++i)
			{
				m_pShadingInstTech->SetParameter(m_arrParameters[i].GetName(), m_arrParameters[i].GetValue());
			}
		}
		return m_pShadingInstTech.get();
	}

	Technique* SubMaterial::GetShadowDepthInstTechnqiue()
	{
		if (m_pShadowDepthInstTech == NULL)
		{
			m_pShadowDepthInstTech = m_pShadowDepthTech->CreateInstTech();

			for (uint32_t i = 0; i < m_arrParameters.size(); ++i)
			{
				m_pShadingInstTech->SetParameter(m_arrParameters[i].GetName(), m_arrParameters[i].GetValue());
			}
		}
		return m_pShadowDepthInstTech.get();
	}

	void SubMaterial::Import(rapidxml::xml_node<>* pXmlElem,Resource* pParent)
	{
		rapidxml::xml_node<>* pXmlShadingTech = pXmlElem->first_node("ShadingTech");
		if (pXmlShadingTech)
		{
			const char* pszTechName = pXmlShadingTech->findAttribute("TechName");
			const char* pszTechMacro = pXmlShadingTech->findAttribute("TechMarco");

			m_pShadingTech = CreateTechnique(pszTechName, pszTechMacro);
		}

		rapidxml::xml_node<>* pXmlShadowDepthTech = pXmlElem->first_node("ShadowDepthTech");
		if (pXmlShadowDepthTech)
		{
			const char* pszTechName = pXmlShadingTech->findAttribute("TechName");
			const char* pszTechMacro = pXmlShadingTech->findAttribute("TechMarco");

			m_pShadowDepthTech = CreateTechnique(pszTechName, pszTechMacro);
		}

		rapidxml::xml_node<>* pXmlParameter = pXmlElem->first_node("Parameters");
		while(pXmlParameter)
		{
			Parameter parameter;
			parameter.Import(pXmlParameter,pParent);
			m_arrParameters.push_back(parameter);

			SetParameter(parameter.GetName(),parameter.GetValue());
			
			pXmlParameter = pXmlParameter->next_sibling("Parameters");
		}

		if (m_pShadingTech)
		{
			m_pShadingTech->GetShaderProgram()->SetRenderPass(GetRenderSystem()->GetDefaultRenderPass());

			GetRenderSystem()->TechniqueStreamComplete(m_pShadingTech.get());
		}

		if (m_pShadowDepthTech)
		{
			DirectonalLight* pDirLight = GetRenderSystem()->GetScene()->GetDirLight();
			ShadowMapFrustum& shadowMap = pDirLight->GetShadowMapFrustum(0);
			m_pShadowDepthTech->GetShaderProgram()->SetRenderPass(shadowMap.GetShadowMapFrameBuffer());

			GetRenderSystem()->TechniqueStreamComplete(m_pShadowDepthTech.get());
		}
	}

	void SubMaterial::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		if (m_pShadingTech)
		{
			rapidxml::xml_node<>* pXmlShadingTech = doc.allocate_node(rapidxml::node_element, doc.allocate_string("ShadingTech"));
			pXmlElem->append_node(pXmlShadingTech);

			const char* pszName = m_pShadingTech->GetTechName();
			const char* pszMacro = m_pShadingTech->GetShaderDefine();

			rapidxml::append_attribute(pXmlShadingTech, doc, "TechName", pszName);
			rapidxml::append_attribute(pXmlShadingTech, doc, "TechMarco", pszMacro);
		}

		if (m_pShadowDepthTech)
		{
			rapidxml::xml_node<>* pXmlShadowDepthTech = doc.allocate_node(rapidxml::node_element, doc.allocate_string("ShadowDepthTech"));
			pXmlElem->append_node(pXmlShadowDepthTech);
			
			const char* pszName = m_pShadowDepthTech->GetTechName();
			const char* pszMacro = m_pShadowDepthTech->GetShaderDefine();

			rapidxml::append_attribute(pXmlShadowDepthTech, doc, "TechName", pszName);
			rapidxml::append_attribute(pXmlShadowDepthTech, doc, "TechMarco", pszMacro);
		}

		for (uint32_t i = 0; i < m_arrParameters.size(); ++i)
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
		{
			m_pShadingTech->SetParameter(pszName, value);
		}

		if (m_pShadingInstTech)
		{
			m_pShadingInstTech->SetParameter(pszName, value);
		}

		if (m_pShadowDepthTech)
		{
			m_pShadowDepthTech->SetParameter(pszName, value);
		}

		if (m_pShadowDepthInstTech)
		{
			m_pShadowDepthInstTech->SetParameter(pszName, value);
		}
	}

	Parameter* SubMaterial::GetParameter(const char* pszName)
	{
		ASSERT(pszName);
		if (pszName == NULL)
			return NULL;

		for (uint32_t i = 0; i < m_arrParameters.size(); ++i)
		{
			if (strcmp(m_arrParameters[i].GetName(), pszName) == 0)
			{
				return &m_arrParameters[i];
			}
		}

		return NULL;
	}

	void SubMaterial::ReLoad()
	{
		m_pShadingInstTech = NULL;
		m_pShadowDepthInstTech = NULL;
		m_pShadowDepthTech = NULL;

		m_pShadingTech->ReLoad();
	}

	RefPtr<SubMaterial> SubMaterial::Clone()
	{
		SubMaterial* pClonMaterial = new SubMaterial();

		rapidxml::xml_document<> doc;
		rapidxml::xml_node<>* pRoot = doc.allocate_node(rapidxml::node_element, doc.allocate_string("SubMaterial"));
		this->Export(pRoot,doc);
		
		pClonMaterial->Import(pRoot,NULL);

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

	uint32_t Material::GetLodSubNumber(uint32_t nLod) const
	{
		if (nLod >= m_arrLodSubMaterial.size())
			return 0;

		return m_arrLodSubMaterial[nLod].size();
	}

	SubMaterial* Material::GetLodSubByIndex(uint32_t nLod,uint32_t index) const
	{
		ASSERT(nLod < m_arrLodSubMaterial.size());
		if (nLod >= m_arrLodSubMaterial.size())
			return NULL;

		ASSERT(index < m_arrLodSubMaterial[nLod].size());
		if (index >= m_arrLodSubMaterial[nLod].size())
			return NULL;

		return m_arrLodSubMaterial[nLod][index].get();
	}

	void Material::AddSubMaterial(uint32_t nLod,SubMaterial* pSubMaterial)
	{
		if (nLod >= m_arrLodSubMaterial.size())
		{
			m_arrLodSubMaterial.resize(nLod + 1);
		}
		m_arrLodSubMaterial[nLod].push_back(pSubMaterial);
	}


	bool Material::Import(rapidxml::xml_node<>* pXmlElem)
	{
		uint32_t nLod = 0;
		rapidxml::xml_node<>* pXmlLodSubMaterial = pXmlElem->first_node("LodMaterial");
		while(pXmlLodSubMaterial)
		{
			rapidxml::xml_node<>* pXmlSubMaterial = pXmlLodSubMaterial->first_node("SubMaterial");
			while(pXmlSubMaterial)
			{
				RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
				this->AddSubMaterial(nLod,pSubMaterial.get());

				pSubMaterial->Import(pXmlSubMaterial,this);

				pXmlSubMaterial = pXmlSubMaterial->next_sibling("SubMaterial");
			}
			pXmlLodSubMaterial = pXmlLodSubMaterial->next_sibling("LodMaterial");
			nLod++;
		}

		return true;
	}

	bool Material::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		for (uint32_t iLod = 0; iLod < m_arrLodSubMaterial.size(); ++iLod)
		{
			rapidxml::xml_node<>* pXmlLodSubMaterial = doc.allocate_node(rapidxml::node_element, doc.allocate_string("LodMaterial"));
			pXmlElem->append_node(pXmlLodSubMaterial);

			for (uint32_t iSub = 0; iSub < m_arrLodSubMaterial[iLod].size(); ++iSub)
			{
				rapidxml::xml_node<>* pXmlSubMaterial = doc.allocate_node(rapidxml::node_element, doc.allocate_string("SubMaterial"));
				pXmlLodSubMaterial->append_node(pXmlSubMaterial);

				SubMaterial* pSubMaterial = m_arrLodSubMaterial[iLod][iSub].get();
				pSubMaterial->Export(pXmlSubMaterial,doc);
			}
		}

		return true;
	}

	void Material::ReLoad()
	{
		for (uint32_t iLod = 0; iLod < m_arrLodSubMaterial.size(); ++iLod)
		{
			for (uint32_t iSub = 0; iSub < m_arrLodSubMaterial[iLod].size(); ++iSub)
			{
				m_arrLodSubMaterial[iLod][iSub]->ReLoad();
			}
		}
	}

	RefPtr<Material> Material::Clone()
	{
		Material* pClonMaterial = new Material();

		rapidxml::xml_document<> doc;
		rapidxml::xml_node<>* pRoot = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Material"));
		this->Export(pRoot,doc);

		pClonMaterial->Import(pRoot);

		pClonMaterial->SetResState(ResInited);

		return pClonMaterial;
	}

	ResourceSystem<Material>* g_pMaterialManager = NULL;

	RefPtr<Material> CreateMaterial()
	{
		return new Material();
	}

	RefPtr<Material> CreateMaterial(const char* pszPath)
	{
		return g_pMaterialManager->CreateResource(pszPath);
	}


}
