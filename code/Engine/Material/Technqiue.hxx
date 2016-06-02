#include "Technqiue.h"

namespace ma
{
	Technique::Technique()
	{
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
		COPY_BASE_ATTRIBUTES(Technique,RenderState);
	}

	void Technique::SetShaderProgram(ShaderProgram* pShader)
	{
		if (m_pShaderProgram == pShader)
			return;

		m_pShaderProgram = pShader;
	}

	int Technique::GetShaderMacroValue(const char* pszMacro)
	{
		ASSERT(pszMacro);
		if (pszMacro == NULL)
			return false;

		const char* pVSFile = m_pShaderProgram->GetVSFile();
		const char* pPSFile = m_pShaderProgram->GetPSFile();
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

	void Technique::SetShaderMacroValue(const char* pszMacro, int nValue)
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

	bool Technique::GetShaderMacro(const char* pszMacro)
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

	void Technique::SetShaderMacro(const char* pszMacro, bool b)
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
		ASSERT(m_pShaderProgram);
		if (m_pShaderProgram == NULL)
			return;

		GetRenderSystem()->SetShaderProgram(m_pShaderProgram.get());

		m_pShaderProgram->BindUniform();
		
		GetRenderSystem()->SetRenderState(*this);

		for (UINT i = 0; i < m_pShaderProgram->GetUniformCount(); ++i)
		{
			Uniform* pUniform = m_pShaderProgram->GetUniform(i);
			Parameter* pMatParam = GetParameter( pUniform->GetName() );
			if (pMatParam == NULL)
				continue;

			BindParametersUniform(pUniform,pMatParam->GetValue());
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

	bool Technique::Import(rapidxml::xml_node<>* pXmlElem)
	{
		rapidxml::xml_node<>* pXmlShader = pXmlElem->first_node("Shader");
		const char* pszVSFile = pXmlShader->findAttribute("VSFile");
		const char* pszPSFile = pXmlShader->findAttribute("PSFile");
		const char* pszMacro = pXmlShader->findAttribute("ShaderMacro");
		m_pShaderProgram = CreateShaderProgram(pszVSFile,pszPSFile,pszMacro);

		rapidxml::xml_node<>* pXmlRenderState = pXmlElem->first_node("RenderState");
		RenderState::Import(pXmlRenderState);

		return true;
	}

	bool Technique::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		rapidxml::xml_node<>* pXmlShader = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Shader"));
		pXmlElem->append_node(pXmlShader);
		pXmlShader->append_attribute(doc.allocate_attribute(doc.allocate_string("VSFile"),doc.allocate_string(m_pShaderProgram->GetVSFile())));
		pXmlShader->append_attribute(doc.allocate_attribute(doc.allocate_string("PSFile"),doc.allocate_string(m_pShaderProgram->GetPSFile())));
		pXmlShader->append_attribute(doc.allocate_attribute(doc.allocate_string("ShaderMacro"),doc.allocate_string(m_pShaderProgram->GetShaderMacro())));

		rapidxml::xml_node<>* pXmlRenderState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("RenderState"));
		pXmlElem->append_node(pXmlRenderState);
		RenderState::Export(pXmlRenderState,doc);
	
		return true;
	}

	RefPtr<Technique> CreateTechnique()
	{
		return new Technique();
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
