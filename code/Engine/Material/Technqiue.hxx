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
			MaterialParameter* pMatParam = m_arrParameters[i];
			Uniform* pUniform = m_pShaderProgram->GetUniform( pMatParam->GetName() );
			pMatParam->Bind(pUniform);
		}
	}

	void Technique::UnBind()
	{
		
	}

	void Technique::SetParameter(const char* pszName,const Any& value)	
	{
		MaterialParameter* pParame = GetParameter(pszName);
		if (pParame)
		{
			pParame->SetValue(value);
		}
		else
		{
			pParame = new MaterialParameter();
			pParame->SetName(pszName);
			pParame->SetValue(value);
			m_arrParameters.push_back(pParame);
		}
	}

	MaterialParameter* Technique::GetParameter(const char* pszName)
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

		return pClone;
	}

	RefPtr<Technique> CreateTechnique(const char* pTechName,const char* pVSFile, const char* pPSFile, const char* pDefine)
	{
		Technique* pTech = new Technique();
		pTech->SetTechName(pTechName);

		string strTemp = string(pVSFile) + string("+") + string(pDefine) + ".shader";
		RefPtr<ShaderProgram> pShaderProgram = DeclareResource<ShaderProgram>( strTemp.c_str() );
		pShaderProgram->CreateFromFile(pVSFile,pPSFile,pDefine);

		pTech->SetShaderProgram(pShaderProgram.get());

		return pTech;
	}


}