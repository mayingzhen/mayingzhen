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

	void Technique::SetShaderProgram(ShaderProgram* pShader)
	{
		if (m_pShaderProgram == pShader)
			return;

		m_pShaderProgram = pShader;
	}

	void Technique::AddShaderMarco(const char* pszMarco)
	{
		const char* pVSFile = m_pShaderProgram->GetVSFile();
		const char* pPSFile = m_pShaderProgram->GetPSFile();
		const char* pPreMrco = m_pShaderProgram->GetShaderMacro();

		string strMrco = string(pPreMrco) + string(";") + string(pszMarco);
		
		string strTemp = string(pVSFile) + string("+") + strMrco + ".shader";
		m_pShaderProgram = FindResource<ShaderProgram>( strTemp.c_str() );
		if(m_pShaderProgram == NULL)
		{
			m_pShaderProgram = DeclareResource<ShaderProgram>( strTemp.c_str() );
			m_pShaderProgram->CreateFromFile(pVSFile,pVSFile,strMrco.c_str());
		}
	}

	void Technique::Bind()
	{
		ASSERT(m_pShaderProgram);
		if (m_pShaderProgram == NULL)
			return;

		GetRenderSystem()->SetShaderProgram(m_pShaderProgram.get());

		m_pShaderProgram->BindUniform();
		
		GetRenderSystem()->SetRenderState(m_renderState);

		for (UINT i = 0; i < m_arrParameters.size(); ++i)
		{
			Parameter* pMatParam = m_arrParameters[i];
			
			Uniform* pUniform = m_pShaderProgram->GetUniform( pMatParam->GetName() );
			//ASSERT(pUniform);
			if (pUniform == NULL)
				continue;
			
			const Any& anyValue = pMatParam->GetValue();
			
			const std::type_info& type = anyValue.getType();
			if (type == typeid(float))
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
			else if (type == typeid(RefPtr<Texture>))
			{
				Texture* pTexture = any_cast< RefPtr<Texture> >(&anyValue)->get();
				GetRenderSystem()->SetValue(pUniform,pTexture);
			}
			else
			{
				ASSERT(false);
			}	
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

	RefPtr<Technique> Technique::Clone()
	{
		RefPtr<Technique> pClone = new Technique();

		pClone->SetTechName(this->GetTechName());
		pClone->SetRenderState(this->GetRenderState());
		pClone->SetShaderProgram(this->GetShaderProgram());

		for (uint32 i = 0; i < m_arrParameters.size(); ++i)
		{
			pClone->SetParameter( m_arrParameters[i]->GetName(), m_arrParameters[i]->GetValue() );
		}

		return pClone;
	}

	RefPtr<Technique> CreateTechnique(const char* pTechName,const char* pVSFile, const char* pPSFile, const char* pDefine)
	{
		Technique* pTech = new Technique();
		pTech->SetTechName(pTechName);

		string strTemp = string(pVSFile) + string("+") + string(pPSFile) + string("+") + string(pDefine) + ".shader";
		RefPtr<ShaderProgram> pShaderProgram = FindResource<ShaderProgram>( strTemp.c_str() );
		if(pShaderProgram == NULL)
		{
			pShaderProgram = DeclareResource<ShaderProgram>( strTemp.c_str() );
			pShaderProgram->CreateFromFile(pVSFile,pPSFile,pDefine);
		}

		pTech->SetShaderProgram(pShaderProgram.get());

		return pTech;
	}


}