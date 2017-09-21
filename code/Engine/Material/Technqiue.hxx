#include "Technqiue.h"

namespace ma
{
	Technique::Technique()
	{
		m_nStencilRef = 0;

		m_pBlendState = CreateBlendState();

		m_pDSState = CreateDepthStencilState();

		m_pRSState = CreateRasterizerState();

		memset(m_arrSampler, 0, sizeof(m_arrSampler));
	}

	Technique::~Technique()
	{
		for (UINT i = 0; i < m_arrParameters.size(); ++i)
		{
			SAFE_DELETE(m_arrParameters[i]);
		}
		m_arrParameters.clear();
	
		m_pShaderProgram = NULL;
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

	int Technique::GetShaderMacroInt(const char* pszMacro)
	{
		ASSERT(pszMacro);
		if (pszMacro == NULL)
			return false;

		std::string stdShder = m_pShaderProgram->GetShaderMacro();

		const vector<string> vecMacros = StringUtil::split(stdShder, ";");

		if( vecMacros.empty() )
			return 0;

		for(uint32 i=0; i< vecMacros.size(); i++)
		{
			const vector<string> keyValue = StringUtil::split(vecMacros[i], "=");
			uint32 nSize = keyValue.size();
			if(nSize != 2)
				continue;

			if(keyValue[0] != string(pszMacro) )
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

		const char* pVSFile = m_pShaderProgram->GetVSFile();
		const char* pPSFile = m_pShaderProgram->GetPSFile();
		std::string strShder = m_pShaderProgram->GetShaderMacro();
        VertexDeclaration* pVertexDecl = m_pShaderProgram->GetVertexDeclaration();

		vector<string> vecMacros = StringUtil::split(strShder,";");
		if (vecMacros.empty())
			return;

		uint32 i = 1;
		for (; i < vecMacros.size(); ++i)
		{
			vector<string> keyValue = StringUtil::split(vecMacros[i],"=");
			uint32 nSize = keyValue.size();
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
		for (uint32 i = 1; i < vecMacros.size(); ++i)
		{
			strFinal += ";" + vecMacros[i];
		}
	
 		m_pShaderProgram = CreateShaderProgram(pVSFile,pPSFile,strFinal.c_str(),pVertexDecl);
	}

	bool Technique::GetShaderMacroBool(const char* pszMacro)
	{
		ASSERT(pszMacro);
		if (pszMacro == NULL)
			return false;

		std::string strShder = m_pShaderProgram->GetShaderMacro();

		vector<string> vecMacros = StringUtil::split(strShder,";");
		if (vecMacros.empty())
			return false;

		for (UINT i = 0; i < vecMacros.size(); ++i)
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

		const char* pVSFile = m_pShaderProgram->GetVSFile();
		const char* pPSFile = m_pShaderProgram->GetPSFile();
		std::string strShder = m_pShaderProgram->GetShaderMacro();
        VertexDeclaration* pVertexDecl = m_pShaderProgram->GetVertexDeclaration();

		vector<string> vecMacros = StringUtil::split(strShder,";");
		if (vecMacros.empty())
			return;

		uint32 i = 1;
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
		for (uint32 i = 1; i < vecMacros.size(); ++i)
		{
			strFinal += ";" + vecMacros[i];
		}

		m_pShaderProgram = CreateShaderProgram(pVSFile,pPSFile,strFinal.c_str(),pVertexDecl);
	}

	void Technique::Bind()
	{
		if (m_pBlendState)
		{
			GetRenderSystem()->SetBlendState(m_pBlendState.get());
		}
	
		if (m_pDSState)
		{
			GetRenderSystem()->SetDepthStencilState(m_pDSState.get(), m_nStencilRef);
		}
		
		if (m_pRSState)
		{
			GetRenderSystem()->SetRasterizerState(m_pRSState.get());
		}
		
		BindUniform();
	}

	void Technique::BindUniform()
	{
		GetRenderSystem()->SetShaderProgram(m_pShaderProgram.get());

		for (UINT i = 0; i < ShaderType_Number; ++i)
		{
			for (UINT iCB = 0; iCB < m_vecConstBuffer[i].size(); ++iCB)
			{
				RefPtr<ConstantBuffer>& pCB = m_vecConstBuffer[i][iCB];
				for (UINT iUniform = 0; iUniform < pCB->GetUniformCount(); ++iUniform)
				{
					Uniform* pUniform = pCB->GetUniformByIndex(iUniform);

					pUniform->Bind();

					Parameter* pMatParam = GetParameter(pUniform->GetName());
					if (pMatParam == NULL)
						continue;

					BindParametersUniform(pUniform, pMatParam->GetValue());
				}
			}
		}
		
		for (uint32 i = 0; i < this->GetSamplerCount(); ++i)
		{
			Uniform* pUniform = this->GetSamplerByIndex(i);
			
			pUniform->Bind();

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

	void Technique::CommitChanges()
	{
// 		ASSERT(m_pShaderProgram);
// 		if (m_pShaderProgram)
// 		{
// 			m_pShaderProgram->CommitChanges();
// 		}
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
			pParame = new Parameter();
			pParame->SetName(pszName);
			pParame->SetValue(value);
			m_arrParameters.push_back(pParame);
		}

// 		Uniform* pUniform = this->GetUniform(pszName);
// 		if (pUniform == NULL)
// 			return;
// 
// 		BindParametersUniform(this->GetUniform(pszName), value);
	}

	Parameter* Technique::GetParameter(const char* pszName)
	{
		ASSERT(pszName);
		if (pszName == NULL)
			return NULL;

		for (UINT i = 0; i < m_arrParameters.size(); ++i)
		{
			if (m_arrParameters[i] && strcmp(m_arrParameters[i]->GetName(), pszName) == 0)
			{
				return m_arrParameters[i];
			}
		}

		return NULL;
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

	void Technique::SetBlendState(BlendState* pBlendState)
	{
		m_pBlendState = pBlendState;
	}

	void Technique::SetDepthStencilState(DepthStencilState* pDSSate)
	{
		m_pDSState = pDSSate;
	}

	void Technique::SetRasterizerState(RasterizerState* pRSState)
	{
		m_pRSState = pRSState;
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

	void Technique::SetValue(Uniform* pUniform, const Matrix4* values, UINT count)
	{
		SetValue(pUniform, (const float*)values, sizeof(Matrix4) * count);
	}

	void Technique::SetValue(Uniform* pUniform, const Vector4* values, UINT count)
	{
		SetValue(pUniform, (const float*)values, sizeof(Vector4) * count);
	}

	void Technique::SetValue(Uniform* pUniform, const ColourValue& value)
	{
		SetValue(pUniform, (const float*)&value, sizeof(ColourValue));
	}

	void Technique::SetValue(Uniform* pUniform, const float* values, UINT nSize)
	{
		if (pUniform == NULL)
			return;

		ASSERT(values);

		ASSERT(nSize <= pUniform->GetSize());
		pUniform->GetParent()->SetParameter(pUniform->GetOffset(), pUniform->GetSize(), values);
	}

	void Technique::SetValue(Uniform* pUniform, Texture* pTexture)
	{

	}

	void Technique::SetValue(Uniform* pUniform, SamplerState* sampler)
	{
		m_arrSampler[pUniform->GetIndex()] = sampler;
	}

	void Technique::AddConstBuffer(ShaderType eType, ConstantBuffer* pConstBuffer)
	{
		m_vecConstBuffer[eType].push_back(pConstBuffer);
	}

	UINT Technique::GetConstBufferCount(ShaderType eType)
	{
		return m_vecConstBuffer[eType].size();
	}

	ConstantBuffer*	Technique::GetConstBufferByIndex(ShaderType eType, UINT nIndex)
	{
		if (nIndex >= m_vecConstBuffer[eType].size())
			return NULL;

		return m_vecConstBuffer[eType][nIndex].get();
	}

	void Technique::AddSampler(Uniform* pUniform)
	{
		m_vecPSSamplers.push_back(pUniform);
	}

	UINT Technique::GetSamplerCount()
	{
		return m_vecPSSamplers.size();
	}

	Uniform* Technique::GetSamplerByIndex(UINT nIndex)
	{
		return m_vecPSSamplers[nIndex].get();
	}

	Uniform* Technique::GetUniform(const char* pszName)
	{
		for (UINT i = 0; i < ShaderType_Number; ++i)
		{
			for (UINT iCB = 0; iCB < m_vecConstBuffer[i].size(); ++iCB)
			{
				RefPtr<ConstantBuffer>& pCB = m_vecConstBuffer[i][iCB];
				for (UINT iUniform = 0; iUniform < pCB->GetUniformCount(); ++iUniform)
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

		for (UINT i = 0; i < m_vecPSSamplers.size(); ++i)
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

    void Technique::StreamComplete()
    {
        GetRenderSystem()->BlendStateStreamComplete(m_pBlendState.get());
        
        GetRenderSystem()->DepthStencilStateStreamComplete(m_pDSState.get());
        
        GetRenderSystem()->RasterizerStateStreamComplete(m_pRSState.get());

		GetRenderSystem()->TechniqueStreamComplete(this);
    }

	bool Technique::Import(rapidxml::xml_node<>* pXmlElem)
	{
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
            
			m_pShaderProgram = CreateShaderProgram(pszVSFile, pszPSFile,m_strDefine.c_str(),pDeclaration.get());
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

				this->SetBlendState(pBlendState.get());
			}

			rapidxml::xml_node<>* pXmlDSState = pXmlElem->first_node("DepthStencilState");
			if (pXmlDSState)
			{
				RefPtr<DepthStencilState> pDSState = CreateDepthStencilState();
				pDSState->Import(pXmlDSState);

				this->SetDepthStencilState(pDSState.get());
			}

			rapidxml::xml_node<>* pXmlRSState = pXmlElem->first_node("RasterizerState");
			if (pXmlRSState)
			{
				RefPtr<RasterizerState> pRSState = CreateRasterizerState();
				pRSState->Import(pXmlRSState);

				this->SetRasterizerState(pRSState.get());
			}
		}
	
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

		if (m_pBlendState)
		{
			rapidxml::xml_node<>* pXmlBlendState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("BlendState"));
			pXmlRenderState->append_node(pXmlBlendState);

			m_pBlendState->Export(pXmlBlendState, doc);
		}
	
		if (m_pDSState)
		{
			rapidxml::xml_node<>* pXmlDSState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("DepthStencilState"));
			pXmlRenderState->append_node(pXmlDSState);

			m_pDSState->Export(pXmlDSState, doc);
		}

		if (m_pRSState)
		{
			rapidxml::xml_node<>* pXmlRSState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("RasterizerState"));
			pXmlRenderState->append_node(pXmlRSState);

			m_pRSState->Export(pXmlRSState, doc);
		}

		return true;
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

		RefPtr<ShaderProgram> pShader = CreateShaderProgram(pVSFile,pPSFile,pDefine,pVertexDecl);

		pTech->SetShaderProgram(pShader.get());

		return pTech;
	}
}
