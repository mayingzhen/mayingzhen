#include "Technqiue.h"

namespace ma
{
	Technique::Technique()
	{
		m_nStencilRef = 0;
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
	
 		m_pShaderProgram = CreateShaderProgram(pVSFile,pPSFile,strFinal.c_str());
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

		m_pShaderProgram = CreateShaderProgram(pVSFile,pPSFile,strFinal.c_str());
	}

	void Technique::Bind()
	{
		ASSERT(m_pDeclaration);
		if (m_pDeclaration)
		{
			GetRenderSystem()->SetVertexDeclaration(m_pDeclaration.get());
		}

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
		
		ASSERT(m_pShaderProgram);
		if (m_pShaderProgram)
		{
			GetRenderSystem()->SetShaderProgram(m_pShaderProgram.get());

			m_pShaderProgram->BindUniform();

			for (UINT i = 0; i < m_pShaderProgram->GetUniformCount(); ++i)
			{
				Uniform* pUniform = m_pShaderProgram->GetUniform(i);
				Parameter* pMatParam = GetParameter(pUniform->GetName());
				if (pMatParam == NULL)
					continue;

				BindParametersUniform(pUniform, pMatParam->GetValue());
			}
		}
	}

	void Technique::BindParametersUniform(Uniform* pUniform,const Any& anyValue)
	{
		const std::type_info& type = anyValue.getType();
		if (type == typeid(int))
		{
			const int* value = any_cast<int>(&anyValue);
			GetRenderSystem()->SetValue(pUniform, *value);
		}
		else if (type == typeid(float))
		{
			const float* value = any_cast<float>(&anyValue);
			GetRenderSystem()->SetValue(pUniform, *value);
		}
		else if (type == typeid(Vector2))
		{
			const Vector2* value = any_cast<Vector2>(&anyValue);
			GetRenderSystem()->SetValue(pUniform, *value);
		}
		else if (type == typeid(Vector3))
		{
			const Vector3* value = any_cast<Vector3>(&anyValue);
			GetRenderSystem()->SetValue(pUniform, *value);
		}
		else if (type == typeid(Vector4))
		{
			const Vector4* value = any_cast<Vector4>(&anyValue);
			GetRenderSystem()->SetValue(pUniform, *value);
		}
		else if (type == typeid(ColourValue))
		{
			ColourValue cColor = any_cast<ColourValue>(anyValue);
			Vector4 vColor(cColor.r,cColor.g,cColor.b,cColor.a);
			GetRenderSystem()->SetValue(pUniform,vColor);
		}
		else if (type == typeid(Matrix4))
		{
			const Matrix4* value = any_cast<Matrix4>(&anyValue);
			GetRenderSystem()->SetValue(pUniform, *value);
		}
		else if (type == typeid(RefPtr<SamplerState>))
		{
			SamplerState* pTexture = any_cast< RefPtr<SamplerState> >(&anyValue)->get();
			GetRenderSystem()->SetValue(pUniform,pTexture);
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
		ASSERT(m_pShaderProgram);
		if (m_pShaderProgram)
		{
			m_pShaderProgram->CommitChanges();
		}
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

		GetRenderSystem()->BlendStateStreamComplete(pBlendState);
	}

	void Technique::SetDepthStencilState(DepthStencilState* pDSSate)
	{
		m_pDSState = pDSSate;

		GetRenderSystem()->DepthStencilStateStreamComplete(pDSSate);
	}

	void Technique::SetRasterizerState(RasterizerState* pRSState)
	{
		m_pRSState = pRSState;

		GetRenderSystem()->RasterizerStateStreamComplete(pRSState);
	}

	void Technique::SetVertexDeclaration(VertexDeclaration* pVertexDecl)
	{
		m_pDeclaration = pVertexDecl;

		pVertexDecl->SetShaderProgram(m_pShaderProgram.get());

		GetRenderSystem()->VertexDeclaComplete(pVertexDecl);
	}

	bool Technique::Import(rapidxml::xml_node<>* pXmlElem)
	{
		rapidxml::xml_node<>* pXmlShader = pXmlElem->first_node("Shader");
		ASSERT(pXmlShader);
		if (pXmlShader)
		{
			const char* pszVSFile = pXmlShader->findAttribute("VSFile");
			const char* pszPSFile = pXmlShader->findAttribute("PSFile");

			m_pShaderProgram = CreateShaderProgram(pszVSFile, pszPSFile, m_strDefine.c_str());
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

		rapidxml::xml_node<>* pXmlVertexDeclaration = pXmlElem->first_node("VertexDeclaration");
		ASSERT(pXmlVertexDeclaration);
		if (pXmlVertexDeclaration)
		{
			RefPtr<VertexDeclaration> pDeclaration = CreateVertexDeclaration();
			pDeclaration->Import(pXmlVertexDeclaration);

			this->SetVertexDeclaration(pDeclaration.get());
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

		if (m_pDeclaration)
		{
			rapidxml::xml_node<>* pXmlVertexDeclaration = doc.allocate_node(rapidxml::node_element, doc.allocate_string("VertexDeclaration"));
			pXmlElem->append_node(pXmlVertexDeclaration);

			m_pDeclaration->Export(pXmlVertexDeclaration, doc);
		}
	
		return true;
	}

	RefPtr<Technique> CreateTechnique()
	{
		return new Technique();
	}

	RefPtr<Technique> CreateTechnique(const char* pszXMLFile, const char* pDefine)
	{
		Technique* pTech = new Technique();
		pTech->SetTechName(pszXMLFile);
		pTech->SetShaderDefine(pDefine);
		pTech->LoadFromXML(pszXMLFile);
		pTech->IsReady();
		return pTech;
	}

	RefPtr<Technique> CreateTechnique(const char* pTechName,const char* pVSFile, const char* pPSFile, const char* pDefine)
	{
		Technique* pTech = new Technique();
		pTech->SetTechName(pTechName);

		RefPtr<ShaderProgram> pShader = CreateShaderProgram(pVSFile,pPSFile,pDefine);

		pTech->SetShaderProgram(pShader.get());

		return pTech;
	}
}
