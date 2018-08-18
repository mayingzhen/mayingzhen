#include "Technqiue.h"

namespace ma
{
	Technique::Technique()
	{

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
		BindUniform(pRenderable, VS);
		BindUniform(pRenderable, GS);
		BindUniform(pRenderable, PS);
	}

	void Technique::BindCompute(Renderable* pRenderable)
	{
		BindUniform(pRenderable, CS);

		for (auto &pUniform : m_vecStorgeBuffer)
		{
			pUniform->Bind(pRenderable);

			Parameter* pMatParam = GetParameter(pUniform->GetName());
			if (pMatParam == NULL)
				continue;

			BindParametersUniform(pRenderable, pUniform.get(), pMatParam->GetValue());
		}
	}

	void Technique::BindUniform(Renderable* pRenderable, ShaderType eType)
	{
		VEC_CONSTBUFFER vecConstBuffer = m_vecConstBuffer[eType];
		for (uint32_t iCB = 0; iCB < vecConstBuffer.size(); ++iCB)
		{
			RefPtr<ConstantBuffer>& pCB = vecConstBuffer[iCB];
			for (uint32_t iUniform = 0; iUniform < pCB->GetUniformCount(); ++iUniform)
			{
				Uniform* pUniform = pCB->GetUniformByIndex(iUniform);

				pUniform->Bind(pRenderable);

				Parameter* pMatParam = GetParameter(pUniform->GetName());
				if (pMatParam == NULL)
					continue;

				BindParametersUniform(pRenderable, pUniform, pMatParam->GetValue());
			}
		}

		VEC_SAMPLER& vecSampler = m_vecSamplers[eType];
		for (uint32_t iSmpler = 0; iSmpler < vecSampler.size(); ++iSmpler)
		{
			Uniform* pUniform = vecSampler[iSmpler].get();

			pUniform->Bind(pRenderable);

			Parameter* pMatParam = GetParameter(pUniform->GetName());
			if (pMatParam == NULL)
				continue;
			
			BindParametersUniform(pRenderable, pUniform, pMatParam->GetValue());
		}
	}

	void Technique::BindParametersUniform(Renderable* pRenderable, Uniform* pUniform, const Any& anyValue)
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
		else if (type == typeid(std::vector<Vector4>))
		{
			const std::vector<Vector4>* value = any_cast<std::vector<Vector4>>(&anyValue);
			const std::vector<Vector4>& vecValue = *value;
			this->SetValue(pUniform, &vecValue[0], vecValue.size());
		}
		else if (type == typeid(ColourValue))
		{
			ColourValue cColor = any_cast<ColourValue>(anyValue);
			Vector4 vColor(cColor.r, cColor.g, cColor.b, cColor.a);
			this->SetValue(pUniform, vColor);
		}
		else if (type == typeid(Matrix4))
		{
			const Matrix4* value = any_cast<Matrix4>(&anyValue);
			this->SetValue(pUniform, *value);
		}
		else if (type == typeid(RefPtr<VertexBuffer>))
		{
			VertexBuffer* pStrorgeBuffer = any_cast<RefPtr<VertexBuffer>>(&anyValue)->get();
			this->SetStorageBuffer(pUniform, pStrorgeBuffer);
		}
		else if (type == typeid(RefPtr<SamplerState>))
		{
			SamplerState* pTexture = any_cast<RefPtr<SamplerState>>(&anyValue)->get();
			this->SetValue(pUniform, pTexture);
		}
		else if (type == typeid(RefPtr<MethodBinding>))
		{
			MethodBinding* pMethod = any_cast<RefPtr<MethodBinding>>(&anyValue)->get();
			pMethod->SetValue(pRenderable, this, pUniform);
		}
		else if (type == typeid(RefPtr<UniformAnimation>))
		{
			UniformAnimation* pUniformAnimation = any_cast<RefPtr<UniformAnimation>>(&anyValue)->get();

			this->BindParametersUniform(pRenderable, pUniform, pUniformAnimation->GetValue());
		}
		else
		{
			ASSERT(false);
		}
	}


	void Technique::SetParameter(const char* pszName, const Any& value)
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
		m_strDefine = pszDefine ? pszDefine : "";
	}

	void Technique::SetRenderPass(RenderPass* pPass)
	{
		m_pRenderPass = pPass;
	}

	int Technique::GetShaderMacroInt(const char* pszMacro)
	{
		ASSERT(pszMacro);
		if (pszMacro == NULL)
			return false;

		std::string stdShder = m_strDefine;

		const vector<string> vecMacros = StringUtil::split(stdShder, ";");

		if (vecMacros.empty())
			return 0;

		for (uint32_t i = 0; i < vecMacros.size(); i++)
		{
			const vector<string> keyValue = StringUtil::split(vecMacros[i], "=");
			uint32_t nSize = keyValue.size();
			if (nSize != 2)
				continue;

			if (keyValue[0] != string(pszMacro))
				continue;

			return StringConverter::parseInt(keyValue[1]);
		}

		return 0;
	}

	void Technique::SetShaderMacroInt(const char* pszMacro, int nValue)
	{
		ASSERT(pszMacro);
		if (pszMacro == NULL)
			return;

		std::string strShder = m_strDefine;

		vector<string> vecMacros = StringUtil::split(strShder, ";");
		if (vecMacros.empty())
			return;

		uint32_t i = 1;
		for (; i < vecMacros.size(); ++i)
		{
			vector<string> keyValue = StringUtil::split(vecMacros[i], "=");
			uint32_t nSize = keyValue.size();
			if (nSize != 2)
				continue;

			if (keyValue[0] != string(pszMacro))
				continue;

			keyValue[1] = StringConverter::toString(nValue);

			vecMacros[i] = keyValue[0] + "=" + keyValue[1];
			break;
		}

		if (i == vecMacros.size())
		{
			string strKey = pszMacro;
			string strValue = StringConverter::toString(nValue);
			vecMacros.push_back(strKey + "=" + strValue);
		}

		std::sort(vecMacros.begin() + 1, vecMacros.end());

		string strFinal = vecMacros[0];
		for (uint32_t i = 1; i < vecMacros.size(); ++i)
		{
			strFinal += ";" + vecMacros[i];
		}

		m_strDefine = strFinal;

		this->ReLoad();
	}

	bool Technique::GetShaderMacroBool(const char* pszMacro)
	{
		ASSERT(pszMacro);
		if (pszMacro == NULL)
			return false;

		std::string strShder = m_strDefine;

		vector<string> vecMacros = StringUtil::split(strShder, ";");
		if (vecMacros.empty())
			return false;

		for (uint32_t i = 0; i < vecMacros.size(); ++i)
		{
			if (vecMacros[i] == string(pszMacro))
			{
				return true;
			}
		}

		return false;
	}

	void Technique::SetShaderMacroBool(const char* pszMacro, bool b)
	{
		ASSERT(pszMacro);
		if (pszMacro == NULL)
			return;

		std::string strShder = m_strDefine;

		vector<string> vecMacros = StringUtil::split(strShder, ";");
		if (vecMacros.empty())
			return;

		uint32_t i = 1;
		for (; i < vecMacros.size(); ++i)
		{
			if (vecMacros[i] == string(pszMacro))
			{
				break;
			}
		}

		if (i == vecMacros.size())
		{
			if (b)
				vecMacros.push_back(pszMacro);
		}
		else
		{
			if (!b)
				vecMacros.erase(vecMacros.begin() + i);
		}

		std::sort(vecMacros.begin() + 1, vecMacros.end());

		string strFinal = vecMacros[0];
		for (uint32_t i = 1; i < vecMacros.size(); ++i)
		{
			strFinal += ";" + vecMacros[i];
		}

		m_strDefine = strFinal;

		//this->ReLoad();
	}

	void Technique::SetValue(Uniform* pUniform, int value)
	{
		SetValue(pUniform, (const uint8_t*)&value, sizeof(int));
	}

	void Technique::SetValue(Uniform* pUniform, float value)
	{
		SetValue(pUniform, (const uint8_t*)&value, sizeof(float));
	}

	void Technique::SetValue(Uniform* pUniform, const Vector2& value)
	{
		SetValue(pUniform, (const uint8_t*)&value, sizeof(Vector2));
	}

	void Technique::SetValue(Uniform* pUniform, const Vector3& value)
	{
		SetValue(pUniform, (const uint8_t*)&value, sizeof(Vector3));
	}

	void Technique::SetValue(Uniform* pUniform, const Vector4& value)
	{
		SetValue(pUniform, (const uint8_t*)&value, sizeof(Vector4));
	}

	void Technique::SetValue(Uniform* pUniform, const Matrix4& value)
	{
		SetValue(pUniform, (const uint8_t*)&value, sizeof(Matrix4));
	}

	void Technique::SetValue(Uniform* pUniform, const Matrix4* values, uint32_t count)
	{
		SetValue(pUniform, (const uint8_t*)values, sizeof(Matrix4) * count);
	}

	void Technique::SetValue(Uniform* pUniform, const Vector4* values, uint32_t count)
	{
		SetValue(pUniform, (const uint8_t*)values, sizeof(Vector4) * count);
	}

	void Technique::SetValue(Uniform* pUniform, const ColourValue& value)
	{
		SetValue(pUniform, (const uint8_t*)&value, sizeof(ColourValue));
	}

	void Technique::SetValue(Uniform* pUniform, const uint8_t* values, uint32_t nSize)
	{
		ASSERT(pUniform);
		if (pUniform == NULL)
			return;

		ASSERT(values);

		GetRenderSystem()->SetUniformValue(pUniform, values, nSize);
	}


	void Technique::SetValue(Uniform* pUniform, SamplerState* sampler)
	{
		if (sampler == nullptr)
			return;

		SetActiveSampler(pUniform, sampler);
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

	void Technique::AddSampler(ShaderType eType, Uniform* pUniform)
	{
		ASSERT(eType == pUniform->GetShaderType());
		m_vecSamplers[eType].push_back(pUniform);
	}

	uint32_t Technique::GetSamplerCount(ShaderType eType)
	{
		return m_vecSamplers[eType].size();
	}

	Uniform* Technique::GetSamplerByIndex(ShaderType eType, uint32_t nIndex)
	{
		return m_vecSamplers[eType][nIndex].get();
	}

	void Technique::ClearSampler(ShaderType eType)
	{
		m_vecSamplers[eType].clear();
	}

	void Technique::AddStorgeBuffer(Uniform* pUniform)
	{
		m_vecStorgeBuffer.push_back(pUniform);
	}

	uint32_t Technique::GetStorgeBufferCount()
	{
		return m_vecStorgeBuffer.size();
	}

	Uniform* Technique::GetStorgeBufferByIndex(uint32_t nIndex)
	{
		ASSERT(nIndex < m_vecStorgeBuffer.size());
		if (nIndex >= m_vecStorgeBuffer.size())
			return NULL;

		return m_vecStorgeBuffer[nIndex].get();
	}

	void Technique::ClearStorgeBuffer()
	{
		m_vecStorgeBuffer.clear();
	}

	void Technique::SetActiveSampler(Uniform* pUniform, SamplerState* pSampler)
	{
		GetRenderSystem()->SetSampler(pUniform, pSampler);
	}

	void Technique::SetStorageBuffer(Uniform* pUniform, HardwareBuffer* pBuffer)
	{
		GetRenderSystem()->SetStorageBuffer(pUniform, pBuffer);
	}

	Uniform* Technique::GetUniform(ShaderType eType, const char* pszName)
	{
		for (uint32_t iCB = 0; iCB < m_vecConstBuffer[eType].size(); ++iCB)
		{
			RefPtr<ConstantBuffer>& pCB = m_vecConstBuffer[eType][iCB];
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

		for (uint32_t iSampler = 0; iSampler < m_vecSamplers[eType].size(); ++iSampler)
		{
			Uniform* pUniform = m_vecSamplers[eType][iSampler].get();
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
			const char* pszMacro = pXmlShader->findAttribute("Macro");

			info.m_shaderMacro = pszMacro ? pszMacro : "";
			info.m_shaderMacro += m_strDefine;

			info.m_strVSFile = pszVSFile;
			info.m_strPSFile = pszPSFile;
			info.m_pVertexDecl = pDeclaration;
		}

		rapidxml::xml_node<>* pXmlRenderState = pXmlElem->first_node("RenderState");
		//ASSERT(pXmlRenderState);
		if (pXmlRenderState)
		{
			rapidxml::xml_node<>* pXmlBlendState = pXmlRenderState->first_node("BlendState");
			if (pXmlBlendState)
			{
				RefPtr<BlendState> pBlendState = CreateBlendState();
				pBlendState->Import(pXmlBlendState);

				info.m_pBlendState = pBlendState;
			}

			rapidxml::xml_node<>* pXmlDSState = pXmlRenderState->first_node("DepthStencilState");
			if (pXmlDSState)
			{
				RefPtr<DepthStencilState> pDSState = CreateDepthStencilState();
				pDSState->Import(pXmlDSState);

				info.m_pDSState = pDSState;
			}

			rapidxml::xml_node<>* pXmlRSState = pXmlRenderState->first_node("RasterizerState");
			if (pXmlRSState)
			{
				RefPtr<RasterizerState> pRSState = CreateRasterizerState();
				pRSState->Import(pXmlRSState);

				info.m_pRSState = pRSState;
			}
		}

		info.m_pRenderPass = m_pRenderPass ? m_pRenderPass : GetRenderSystem()->GetDefaultRenderPass();

		m_pShaderProgram = CreateShaderProgram(info);

		GetRenderSystem()->TechniqueStreamComplete(this);

		return true;
	}

	bool Technique::Export(rapidxml::xml_node<>* pXmlElem, rapidxml::xml_document<>& doc)
	{
		if (m_pShaderProgram == NULL)
			return false;

		rapidxml::xml_node<>* pXmlShader = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Shader"));
		pXmlElem->append_node(pXmlShader);

		const ShaderCreateInfo& info = m_pShaderProgram->GetShaderCreateInfo();

		rapidxml::append_attribute(pXmlShader, doc, "VSFile", info.m_strVSFile.c_str());
		rapidxml::append_attribute(pXmlShader, doc, "PSFile", info.m_strPSFile.c_str());
		rapidxml::append_attribute(pXmlShader, doc, "Macro", info.m_shaderMacro.c_str());

		if (info.m_pVertexDecl)
		{
			rapidxml::xml_node<>* pXmlVertexDeclaration = doc.allocate_node(rapidxml::node_element, doc.allocate_string("VertexDeclaration"));
			pXmlShader->append_node(pXmlVertexDeclaration);

			info.m_pVertexDecl->Export(pXmlVertexDeclaration, doc);
		}

		rapidxml::xml_node<>* pXmlRenderState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("RenderState"));
		pXmlElem->append_node(pXmlRenderState);

		if (info.m_pBlendState)
		{
			rapidxml::xml_node<>* pXmlBlendState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("BlendState"));
			pXmlRenderState->append_node(pXmlBlendState);

			info.m_pBlendState->Export(pXmlBlendState, doc);
		}

		if (info.m_pDSState)
		{
			rapidxml::xml_node<>* pXmlDSState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("DepthStencilState"));
			pXmlRenderState->append_node(pXmlDSState);

			info.m_pDSState->Export(pXmlDSState, doc);
		}

		if (info.m_pRSState)
		{
			rapidxml::xml_node<>* pXmlRSState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("RasterizerState"));
			pXmlRenderState->append_node(pXmlRSState);

			info.m_pRSState->Export(pXmlRSState, doc);
		}

		return true;
	}

	RefPtr<Technique> Technique::CreateInstTech()
	{
		ShaderProgram* pShader = this->GetShaderProgram();
		ASSERT(pShader);
		if (pShader == NULL)
			return NULL;

		const ShaderCreateInfo& info = pShader->GetShaderCreateInfo();

		VertexDeclaration* pVertexDecl = info.m_pVertexDecl.get();

		std::vector<VertexElement> vecElement;
		for (uint32_t i = 0; i < pVertexDecl->GetElementCount(0); ++i)
		{
			vecElement.push_back(pVertexDecl->GetElement(0, i));
		}
		vecElement.push_back(VertexElement(1, 0, DT_FLOAT4, DU_TEXCOORD, 1));
		vecElement.push_back(VertexElement(1, 16, DT_FLOAT4, DU_TEXCOORD, 2));
		vecElement.push_back(VertexElement(1, 32, DT_FLOAT4, DU_TEXCOORD, 3));

		RefPtr<VertexDeclaration> pDeclaration = GetRenderSystem()->CreateVertexDeclaration(vecElement.data(), vecElement.size());

		std::string strShaderMacro = info.m_shaderMacro;
		strShaderMacro += ";INSTANCE";

		ShaderCreateInfo instInfo;
		instInfo.m_strVSFile = info.m_strVSFile;
		instInfo.m_strPSFile = info.m_strPSFile;
		instInfo.m_shaderMacro = strShaderMacro;
		instInfo.m_pVertexDecl = pDeclaration;
		instInfo.m_pRenderPass = pShader->GetRenderPass();

		RefPtr<Technique> pInstTech = CreateTechnique("instance.tech", instInfo);

		for (uint32_t i = 0; i < m_arrParameters.size(); ++i)
		{
			pInstTech->SetParameter(m_arrParameters[i].GetName(), m_arrParameters[i].GetValue());
		}

		return pInstTech;
	}

	void Technique::ReLoad()
	{
		GetRenderSystem()->FlushAndWait(); // TechniqueStreamComplete 可能还在RenderThread

		ShaderCreateInfo info = m_pShaderProgram->GetShaderCreateInfo();
		info.m_shaderMacro = m_strDefine;

		m_pShaderProgram = CreateShaderProgram(info);

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

	RefPtr<Technique> CreateTechnique(const char* pszXMLFile, const char* pDefine, RenderPass* pPass)
	{
		Technique* pTech = GetRenderDevice()->CreateTechnique();
		pTech->SetTechName(pszXMLFile);
		pTech->SetShaderDefine(pDefine);
		pTech->SetRenderPass(pPass);
		pTech->LoadFromXML(pszXMLFile);
		pTech->IsReady();
		return pTech;
	}

	RefPtr<Technique> CreateTechnique(const char* pTechName, const ShaderCreateInfo& info)
	{
		Technique* pTech = GetRenderDevice()->CreateTechnique();
		pTech->SetTechName(pTechName);

		RefPtr<ShaderProgram> pShader = CreateShaderProgram(info);

		pTech->SetShaderProgram(pShader.get());

		GetRenderSystem()->TechniqueStreamComplete(pTech);

		return pTech;
	}

}

