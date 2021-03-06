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

	Technique* SubMaterial::GetTechnqiue(RenderPass* pPass)
	{
		if (m_pShadingTech && m_pShadingTech->GetRenderPass() == pPass)
		{
			return m_pShadingTech.get();
		}

		for (auto & tech : m_vecExtTech)
		{
			if (tech->GetRenderPass() == pPass)
			{
				return tech.get();
			}
		}

		m_pShadingTech->SetRenderPass(pPass);
		m_pShadingTech->ReLoad();
		
		return m_pShadingTech.get();
	}

	void SubMaterial::SetShadingTechnqiue(Technique* pTech)
	{
		m_pShadingTech = pTech;

		for (uint32_t i = 0; i < m_arrParameters.size(); ++i)
		{
			m_pShadingTech->SetParameter(m_arrParameters[i].GetName(), m_arrParameters[i].GetValue());
		}
	}

	Technique* SubMaterial::GetShadowDepthTechnqiue(RenderPass* pShadowPass, uint32_t nSubIndex)
	{
		if (m_vecShadowDepthTech.size() <= nSubIndex)
		{
			m_vecShadowDepthTech.resize(nSubIndex + 1);
		}

		if (m_vecShadowDepthTech[nSubIndex] == NULL)
		{
			VertexDeclaration* pVertexDecl = m_pShadingTech->GetShaderProgram()->GetShaderCreateInfo().m_pVertexDecl.get();

			std::vector<VertexElement> vecElement;
			for (uint32_t i = 0; i < pVertexDecl->GetElementCount(0); ++i)
			{
				vecElement.push_back(pVertexDecl->GetElement(0, i));
			}

			RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(vecElement.data(), vecElement.size());

			std::string strShaderMacro = m_pShadingTech->GetShaderProgram()->GetShaderCreateInfo().m_shaderMacro;
          
			RefPtr<BlendState> pBlendSate = CreateBlendState();
			pBlendSate->m_blendDesc[0].nColorWrite = 0;

			RefPtr<RasterizerState> pRasState = CreateRasterizerState();
			pRasState->m_fConstantBias = 1.25f;
			pRasState->m_fSlopeScaleBias = 1.75f;

			DirectonalLight* pDirLight = GetRenderSystem()->GetScene()->GetMainDirLight();

			ShaderCreateInfo info;
			info.m_strVSFile = "ShadowDepth.hlsl:vs_main";
			info.m_strPSFile = "ShadowDepth.hlsl:ps_main";
			info.m_shaderMacro = strShaderMacro;
			info.m_pVertexDecl = pDeclaration;
			info.m_pBlendState = pBlendSate;
			info.m_pRSState = pRasState;
			info.m_pRenderPass = pShadowPass;
			RefPtr<Technique> pShadowDepthTech = CreateTechnique(info);

			for (uint32_t i = 0; i < m_arrParameters.size(); ++i)
			{
				pShadowDepthTech->SetParameter(m_arrParameters[i].GetName(), m_arrParameters[i].GetValue());
			}

			m_vecShadowDepthTech[nSubIndex] = pShadowDepthTech;
		}

		return m_vecShadowDepthTech[nSubIndex].get();
	}

	Technique* SubMaterial::GetShadingTechnqiue()
	{
		return m_pShadingTech.get();
	}

	void SubMaterial::Import(rapidxml::xml_node<>* pXmlElem,Resource* pParent)
	{
		rapidxml::xml_node<>* pXmlShadingTech = pXmlElem->first_node("ShadingTech");
		if (pXmlShadingTech)
		{
			const char* pszTechName = pXmlShadingTech->findAttribute("TechName");
			const char* pszTechMacro = pXmlShadingTech->findAttribute("TechMarco");
			RenderPass* pRenderPass = GetRenderSystem()->GetBaseRenderPass();

			m_pShadingTech = CreateTechnique(pszTechName, pszTechMacro, pRenderPass);
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
	}

	void SubMaterial::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		if (m_pShadingTech)
		{
			rapidxml::xml_node<>* pXmlShadingTech = doc.allocate_node(rapidxml::node_element, doc.allocate_string("ShadingTech"));
			pXmlElem->append_node(pXmlShadingTech);

			const char* pszName = m_pShadingTech->GetResPath();
			const char* pszMacro = m_pShadingTech->GetShaderDefine();

			rapidxml::append_attribute(pXmlShadingTech, doc, "TechName", pszName);
			rapidxml::append_attribute(pXmlShadingTech, doc, "TechMarco", pszMacro);
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
		m_vecShadowDepthTech.clear();

		m_pShadingTech->ReLoad();
	}

	RefPtr<SubMaterial> SubMaterial::Clone()
	{
		RefPtr<SubMaterial> pClonMaterial = CreateSubMaterial();

		rapidxml::xml_document<> doc;
		rapidxml::xml_node<>* pRoot = doc.allocate_node(rapidxml::node_element, doc.allocate_string("SubMaterial"));
		this->Export(pRoot,doc);
		
		pClonMaterial->Import(pRoot,NULL);

		return pClonMaterial;
	}

	std::vector< RefPtr<SubMaterial> > g_subMaterilalAll;

	RefPtr<SubMaterial> CreateSubMaterial()
	{
		SubMaterial* pMaterial = new SubMaterial();

		g_subMaterilalAll.push_back(pMaterial);

		return pMaterial;
	}


	Material::Material()
	{
	}

	Material::~Material()
	{
	}

	uint32_t Material::GetSubNumber() const
	{
		return m_arrSubMaterial.size();
	}

	SubMaterial* Material::GetSubByIndex(uint32_t index) const
	{
		ASSERT(index < m_arrSubMaterial.size());
		if (index >= m_arrSubMaterial.size())
			return NULL;

		return m_arrSubMaterial[index].get();
	}

	void Material::AddSubMaterial(SubMaterial* pSubMaterial)
	{
		m_arrSubMaterial.push_back(pSubMaterial);
	}

	bool Material::Import(rapidxml::xml_node<>* pXmlElem)
	{
		rapidxml::xml_node<>* pXmlSubMaterial = pXmlElem->first_node("SubMaterial");
		while(pXmlSubMaterial)
		{
			RefPtr<SubMaterial> pSubMaterial = CreateSubMaterial();
			this->AddSubMaterial(pSubMaterial.get());

			pSubMaterial->Import(pXmlSubMaterial,this);

			pXmlSubMaterial = pXmlSubMaterial->next_sibling("SubMaterial");
		}

		return true;
	}

	bool Material::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		for (uint32_t iSub = 0; iSub < m_arrSubMaterial.size(); ++iSub)
		{
			rapidxml::xml_node<>* pXmlSubMaterial = doc.allocate_node(rapidxml::node_element, doc.allocate_string("SubMaterial"));
			pXmlElem->append_node(pXmlSubMaterial);

			SubMaterial* pSubMaterial = m_arrSubMaterial[iSub].get();
			pSubMaterial->Export(pXmlSubMaterial,doc);
		}

		return true;
	}

	void Material::ReLoad()
	{
		for (uint32_t iSub = 0; iSub < m_arrSubMaterial.size(); ++iSub)
		{
			m_arrSubMaterial[iSub]->ReLoad();
		}
	}

	RefPtr<Material> Material::Clone()
	{
		Material* pClonMaterial = new Material();

		rapidxml::xml_document<> doc;
		rapidxml::xml_node<>* pRoot = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Material"));
		this->Export(pRoot,doc);

		pClonMaterial->Import(pRoot);

		pClonMaterial->SetResState(ResReady);

		return pClonMaterial;
	}

	ResourceSystem<Material>* g_pMaterialManager = NULL;

	RefPtr<Material> CreateMaterial()
	{
		return new Material();
	}

	RefPtr<Material> CreateMaterial(const char* pszPath, const RES_CALL_BACK& call_back)
	{
		return g_pMaterialManager->CreateResource(pszPath, call_back);
	}


}
