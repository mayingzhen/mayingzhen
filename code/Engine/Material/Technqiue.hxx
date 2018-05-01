#include "Technqiue.h"

namespace ma
{
	Technique::Technique()
	{
// 		m_nStencilRef = 0;
// 
// 		m_pBlendState = CreateBlendState();
// 
// 		m_pDSState = CreateDepthStencilState();
// 
// 		m_pRSState = CreateRasterizerState();

		memset(m_arrSampler, 0, sizeof(m_arrSampler));
	}

	Technique::~Technique()
	{

	}

	void Technique::RegisterAttribute()
	{
	}

	void Technique::SetShaderProgram(ShaderProgram* pShader)
	{
		if (m_pShaderProgram == pShader)
			return;

		m_pShaderProgram = pShader;
	}

	void Technique::Bind(Renderable* pRenderable)
	{
		BindUniform(pRenderable);
	}

	void Technique::BindUniform(Renderable* pRenderable)
	{
		for (uint32_t i = 0; i < ShaderType_Number; ++i)
		{
			for (uint32_t iCB = 0; iCB < m_vecConstBuffer[i].size(); ++iCB)
			{
				RefPtr<ConstantBuffer>& pCB = m_vecConstBuffer[i][iCB];
				for (uint32_t iUniform = 0; iUniform < pCB->GetUniformCount(); ++iUniform)
				{
					Uniform* pUniform = pCB->GetUniformByIndex(iUniform);

					pUniform->Bind(pRenderable);

					Parameter* pMatParam = GetParameter(pUniform->GetName());
					if (pMatParam == NULL)
						continue;

					BindParametersUniform(pUniform, pMatParam->GetValue());
				}
			}
		}
		
		for (uint32_t i = 0; i < this->GetSamplerCount(); ++i)
		{
			Uniform* pUniform = this->GetSamplerByIndex(i);
			
			pUniform->Bind(pRenderable);

			Parameter* pMatParam = GetParameter(pUniform->GetName());
			if (pMatParam == NULL)
				continue;

			BindParametersUniform(pUniform, pMatParam->GetValue());
		}
	}

	void Technique::BindParametersUniform(Uniform* pUniform,const Any& anyValue)
	{
		ASSERT(pUniform);
		if (pUniform == NULL)
			return;

		const std::type_info& type = anyValue.getType();
		if (type == typeid(int))
		{
			const int* value = any_cast<int>(&anyValue);
			this->SetValue(pUniform, *value);
		}
		else if (type == typeid(float))
		{
			const float* value = any_cast<float>(&anyValue);
			this->SetValue(pUniform, *value);
		}
		else if (type == typeid(Vector2))
		{
			const Vector2* value = any_cast<Vector2>(&anyValue);
			this->SetValue(pUniform, *value);
		}
		else if (type == typeid(Vector3))
		{
			const Vector3* value = any_cast<Vector3>(&anyValue);
			this->SetValue(pUniform, *value);
		}
		else if (type == typeid(Vector4))
		{
			const Vector4* value = any_cast<Vector4>(&anyValue);
			this->SetValue(pUniform, *value);
		}
		else if (type == typeid(ColourValue))
		{
			ColourValue cColor = any_cast<ColourValue>(anyValue);
			Vector4 vColor(cColor.r,cColor.g,cColor.b,cColor.a);
			this->SetValue(pUniform,vColor);
		}
		else if (type == typeid(Matrix4))
		{
			const Matrix4* value = any_cast<Matrix4>(&anyValue);
			this->SetValue(pUniform, *value);
		}
		else if (type == typeid(RefPtr<SamplerState>))
		{
			SamplerState* pTexture = any_cast< RefPtr<SamplerState> >(&anyValue)->get();
			this->SetValue(pUniform,pTexture);
		}
		else if (type == typeid(RefPtr<UniformAnimation>))
		{
			UniformAnimation* pUniformAnimation= any_cast< RefPtr<UniformAnimation> >(&anyValue)->get();
	
			this->BindParametersUniform(pUniform,pUniformAnimation->GetValue());
		}
		else
		{
			ASSERT(false);
		}
	}

	void Technique::UnBind()
	{
		
	}

	void Technique::SetParameter(const char* pszName,const Any& value)	
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
	}

	Parameter* Technique::GetParameter(const char* pszName)
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

	uint32_t Technique::GetParameterCount()
	{
		return m_arrParameters.size();
	}

	Parameter*	Technique::GetParameterByIndex(uint32_t nIndex)
	{
		ASSERT(nIndex < m_arrParameters.size());
		if (nIndex >= m_arrParameters.size())
			return NULL;

		return &m_arrParameters[nIndex];
	}

	const char*	Technique::GetTechName() const
	{
		return m_stName.c_str();
	}

	void Technique::SetTechName(const char* pName)
	{
		m_stName = pName ? pName : "";
	}	

	const char*	Technique::GetShaderDefine() const
	{
		return m_strDefine.c_str();
	}

	void Technique::SetShaderDefine(const char* pszDefine)
	{
		ASSERT(pszDefine);
		if (pszDefine == NULL)
			return;

		m_strDefine = pszDefine;
	}

	void Technique::SetValue(Uniform* pUniform, int value)
	{
		SetValue(pUniform, (const float*)&value, sizeof(int));
	}

	void Technique::SetValue(Uniform* pUniform, float value)
	{
		SetValue(pUniform, (const float*)&value, sizeof(float));
	}

	void Technique::SetValue(Uniform* pUniform, const Vector2& value)
	{
		SetValue(pUniform, (const float*)&value, sizeof(Vector2));
	}

	void Technique::SetValue(Uniform* pUniform, const Vector3& value)
	{
		SetValue(pUniform, (const float*)&value, sizeof(Vector3));
	}

	void Technique::SetValue(Uniform* pUniform, const Vector4& value)
	{
		SetValue(pUniform, (const float*)&value, sizeof(Vector4));
	}

	void Technique::SetValue(Uniform* pUniform, const Matrix4& value)
	{
		SetValue(pUniform, (const float*)&value, sizeof(Matrix4));
	}

	void Technique::SetValue(Uniform* pUniform, const Matrix4* values, uint32_t count)
	{
		SetValue(pUniform, (const float*)values, sizeof(Matrix4) * count);
	}

	void Technique::SetValue(Uniform* pUniform, const Vector4* values, uint32_t count)
	{
		SetValue(pUniform, (const float*)values, sizeof(Vector4) * count);
	}

	void Technique::SetValue(Uniform* pUniform, const ColourValue& value)
	{
		SetValue(pUniform, (const float*)&value, sizeof(ColourValue));
	}

	void Technique::SetValue(Uniform* pUniform, const float* values, uint32_t nSize)
	{
		ASSERT(pUniform);
		if (pUniform == NULL)
			return;

		ASSERT(values);

		GetRenderSystem()->SetUniformValue(pUniform, values, nSize);
	}

	void Technique::SetValue(Uniform* pUniform, Texture* pTexture)
	{

	}

	void Technique::SetValue(Uniform* pUniform, SamplerState* sampler)
	{
		if (m_arrSampler[pUniform->GetIndex()] != sampler)
		{
			m_arrSampler[pUniform->GetIndex()] = sampler;

			GetRenderSystem()->SetSampler(pUniform, sampler);
		}
	}

	void Technique::AddConstBuffer(ShaderType eType, ConstantBuffer* pConstBuffer)
	{
		m_vecConstBuffer[eType].push_back(pConstBuffer);
	}

	uint32_t Technique::GetConstBufferCount(ShaderType eType)
	{
		return m_vecConstBuffer[eType].size();
	}

	ConstantBuffer*	Technique::GetConstBufferByIndex(ShaderType eType, uint32_t nIndex)
	{
		if (nIndex >= m_vecConstBuffer[eType].size())
			return NULL;

		return m_vecConstBuffer[eType][nIndex].get();
	}

	void Technique::ClearConstBuffer(ShaderType eType)
	{
		return m_vecConstBuffer[eType].clear();
	}

	void Technique::AddSampler(Uniform* pUniform)
	{
		m_vecPSSamplers.push_back(pUniform);
	}

	uint32_t Technique::GetSamplerCount()
	{
		return m_vecPSSamplers.size();
	}

	Uniform* Technique::GetSamplerByIndex(uint32_t nIndex)
	{
		return m_vecPSSamplers[nIndex].get();
	}

	void Technique::ClearSampler()
	{
		m_vecPSSamplers.clear();
	}

	Uniform* Technique::GetUniform(const char* pszName)
	{
		for (uint32_t i = 0; i < ShaderType_Number; ++i)
		{
			for (uint32_t iCB = 0; iCB < m_vecConstBuffer[i].size(); ++iCB)
			{
				RefPtr<ConstantBuffer>& pCB = m_vecConstBuffer[i][iCB];
				for (uint32_t iUniform = 0; iUniform < pCB->GetUniformCount(); ++iUniform)
				{
					Uniform* pUniform = pCB->GetUniformByIndex(iUniform);
					if (pUniform == NULL)
						continue;

					if (strcmp(pszName, pUniform->GetName()) == 0)
					{
						return pUniform;
					}
				}
			}
		}

		for (uint32_t i = 0; i < m_vecPSSamplers.size(); ++i)
		{
			Uniform* pUniform = m_vecPSSamplers[i].get();
			if (pUniform == NULL)
				continue;

			if (strcmp(pszName, pUniform->GetName()) == 0)
			{
				return pUniform;
			}
		}

		return NULL;
	}

	bool Technique::Import(rapidxml::xml_node<>* pXmlElem)
	{
		ShaderCreateInfo info;

		rapidxml::xml_node<>* pXmlShader = pXmlElem->first_node("Shader");
		ASSERT(pXmlShader);
		if (pXmlShader)
		{
			RefPtr<VertexDeclaration> pDeclaration;
			rapidxml::xml_node<>* pXmlVertexDeclaration = pXmlShader->first_node("VertexDeclaration");
			ASSERT(pXmlVertexDeclaration);
			if (pXmlVertexDeclaration)
			{
				pDeclaration = CreateVertexDeclaration();
				pDeclaration->Import(pXmlVertexDeclaration);
			}

			const char* pszVSFile = pXmlShader->findAttribute("VSFile");
			const char* pszPSFile = pXmlShader->findAttribute("PSFile");
            
			info.m_shaderMacro = m_strDefine;
			info.m_strVSFile = pszVSFile;
			info.m_strPSFile = pszPSFile;
			info.m_pVertexDecl = pDeclaration;
			//m_pShaderProgram = CreateShaderProgram(pszVSFile, pszPSFile,m_strDefine.c_str(),pDeclaration.get());
		}

		rapidxml::xml_node<>* pXmlRenderState = pXmlElem->first_node("RenderState");
		ASSERT(pXmlRenderState);
		if (pXmlRenderState)
		{
			rapidxml::xml_node<>* pXmlBlendState = pXmlElem->first_node("BlendState");
			if (pXmlBlendState)
			{
				RefPtr<BlendState> pBlendState = CreateBlendState();
				pBlendState->Import(pXmlBlendState);

				info.m_pBlendState = pBlendState;
				//this->SetBlendState(pBlendState.get());
			}

			rapidxml::xml_node<>* pXmlDSState = pXmlElem->first_node("DepthStencilState");
			if (pXmlDSState)
			{
				RefPtr<DepthStencilState> pDSState = CreateDepthStencilState();
				pDSState->Import(pXmlDSState);

				info.m_pDSState = pDSState;
				//this->SetDepthStencilState(pDSState.get());
			}

			rapidxml::xml_node<>* pXmlRSState = pXmlElem->first_node("RasterizerState");
			if (pXmlRSState)
			{
				RefPtr<RasterizerState> pRSState = CreateRasterizerState();
				pRSState->Import(pXmlRSState);

				info.m_pRSState = pRSState;
				//this->SetRasterizerState(pRSState.get());
			}
		}
	
		m_pShaderProgram = CreateShaderProgram(info);

		return true;
	}

	bool Technique::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		if (m_pShaderProgram)
		{
			rapidxml::xml_node<>* pXmlShader = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Shader"));
			pXmlElem->append_node(pXmlShader);

			pXmlShader->append_attribute(doc.allocate_attribute(doc.allocate_string("VSFile"), doc.allocate_string(m_pShaderProgram->GetVSFile())));
			pXmlShader->append_attribute(doc.allocate_attribute(doc.allocate_string("PSFile"), doc.allocate_string(m_pShaderProgram->GetPSFile())));
            
            VertexDeclaration* pVertexDecl = m_pShaderProgram->GetVertexDeclaration();
            if (pVertexDecl)
            {
                rapidxml::xml_node<>* pXmlVertexDeclaration = doc.allocate_node(rapidxml::node_element, doc.allocate_string("VertexDeclaration"));
                pXmlShader->append_node(pXmlVertexDeclaration);
                
                pVertexDecl->Export(pXmlVertexDeclaration, doc);
            }
		}

		rapidxml::xml_node<>* pXmlRenderState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("RenderState"));
		pXmlElem->append_node(pXmlRenderState);

		if (m_pShaderProgram->GetBlendState())
		{
			rapidxml::xml_node<>* pXmlBlendState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("BlendState"));
			pXmlRenderState->append_node(pXmlBlendState);

			m_pShaderProgram->GetBlendState()->Export(pXmlBlendState, doc);
		}
	
		if (m_pShaderProgram->GetDepthStencilState())
		{
			rapidxml::xml_node<>* pXmlDSState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("DepthStencilState"));
			pXmlRenderState->append_node(pXmlDSState);

			m_pShaderProgram->GetDepthStencilState()->Export(pXmlDSState, doc);
		}

		if (m_pShaderProgram->GetRasterizerState())
		{
			rapidxml::xml_node<>* pXmlRSState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("RasterizerState"));
			pXmlRenderState->append_node(pXmlRSState);

			m_pShaderProgram->GetRasterizerState()->Export(pXmlRSState, doc);
		}

		return true;
	}

	RefPtr<Technique> Technique::CreateInstTech()
	{
		ShaderProgram* pShader = this->GetShaderProgram();
		ASSERT(pShader);
		if (pShader == NULL)
			return NULL;

		VertexDeclaration* pVertexDecl = pShader->GetVertexDeclaration();

		std::vector<VertexElement> vecElement;
		for (uint32_t i = 0; i < pVertexDecl->GetElementCount(0); ++i)
		{
			vecElement.push_back(pVertexDecl->GetElement(0, i));
		}
		vecElement.push_back(VertexElement(1, 0, DT_FLOAT4, DU_TEXCOORD, 1));
		vecElement.push_back(VertexElement(1, 16, DT_FLOAT4, DU_TEXCOORD, 2));
		vecElement.push_back(VertexElement(1, 32, DT_FLOAT4, DU_TEXCOORD, 3));

		RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(vecElement.data(), vecElement.size());

		std::string strShaderMacro = pShader->GetShaderMacro();
		strShaderMacro += ";INSTANCE";

		ShaderCreateInfo info;
		info.m_strVSFile = pShader->GetVSFile();
		info.m_strPSFile = pShader->GetPSFile();
		info.m_shaderMacro = strShaderMacro;
		info.m_pVertexDecl = pDeclaration;
		info.m_pRenderPass = pShader->GetRenderPass();

		RefPtr<Technique> pInstTech = CreateTechnique("instance.tech", info);

		GetRenderSystem()->TechniqueStreamComplete(pInstTech.get());

		for (uint32_t i = 0; i < m_arrParameters.size(); ++i)
		{
			pInstTech->SetParameter(m_arrParameters[i].GetName(), m_arrParameters[i].GetValue());
		}

		return pInstTech;
	}

	void Technique::ReLoad()
	{
		m_pShaderProgram->Reload();

		GetRenderSystem()->TechniqueStreamComplete(this);
	}

	RefPtr<Technique> Technique::Clone()
	{
		Technique* pClonTechnique = GetRenderDevice()->CreateTechnique();

		rapidxml::xml_document<> doc;
		rapidxml::xml_node<>* pRoot = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Technique"));
		this->Export(pRoot, doc);

		pClonTechnique->Import(pRoot);

		for (uint32_t i = 0; i < m_arrParameters.size(); ++i)
		{
			pClonTechnique->SetParameter(m_arrParameters[i].GetName(), m_arrParameters[i].GetValue());
		}

		return pClonTechnique;
	}

	RefPtr<Technique> CreateTechnique()
	{
		return GetRenderDevice()->CreateTechnique();
	}

	RefPtr<Technique> CreateTechnique(const char* pszXMLFile, const char* pDefine)
	{
		Technique* pTech = GetRenderDevice()->CreateTechnique();
		pTech->SetTechName(pszXMLFile);
		pTech->SetShaderDefine(pDefine);
		pTech->LoadFromXML(pszXMLFile);
		pTech->IsReady();
		return pTech;
	}

	RefPtr<Technique> CreateTechnique(const char* pTechName,const char* pVSFile, const char* pPSFile, const char* pDefine,VertexDeclaration* pVertexDecl)
	{
		Technique* pTech = GetRenderDevice()->CreateTechnique();
		pTech->SetTechName(pTechName);
		pTech->SetShaderDefine(pDefine);

		ShaderCreateInfo info;
		info.m_strVSFile = pVSFile;
		info.m_strPSFile = pPSFile;
		info.m_shaderMacro = pDefine;
		info.m_pVertexDecl = pVertexDecl;

		RefPtr<ShaderProgram> pShader = CreateShaderProgram(info);

		pTech->SetShaderProgram(pShader.get());

		return pTech;
	}

	RefPtr<Technique> CreateTechnique(const char* pTechName, const ShaderCreateInfo& info)
	{
		Technique* pTech = GetRenderDevice()->CreateTechnique();
		pTech->SetTechName(pTechName);

		RefPtr<ShaderProgram> pShader = CreateShaderProgram(info);

		pTech->SetShaderProgram(pShader.get());

		return pTech;
	}
}

