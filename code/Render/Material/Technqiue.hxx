#include "Technqiue.h"

namespace ma
{
	Technique::Technique(const char* pTechName,const char* pVSFile, const char* pPSFile, const char* pDefine)
	{
		m_stName = pTechName;

		m_pShaderProgram = GetRenderSystem()->CreateShaderProgram(this,pVSFile,pPSFile,pDefine);
		UINT nUniform = m_pShaderProgram->GetUniformCount();
		for (UINT i = 0; i < nUniform; ++i)
		{
			Uniform* pUniform = m_pShaderProgram->GetUniform(i);
			ASSERT(pUniform);
			if (pUniform == NULL)
				continue;

			MaterialParameter* pParam = new MaterialParameter(pUniform->m_name.c_str() );
			m_arrParameters.push_back(pParam);
			GetParameterManager()->UseDefaultBing(pParam);
		}
	}

	Technique::~Technique()
	{
		for (UINT i = 0; i < m_arrParameters.size(); ++i)
		{
			SAFE_DELETE(m_arrParameters[i]);
		}
		m_arrParameters.clear();
	
		SAFE_DELETE(m_pShaderProgram);
	}

	void Technique::Bind()
	{
		ASSERT(m_pShaderProgram);
		if (m_pShaderProgram == NULL)
			return;

		m_pShaderProgram->Bind();

		GetRenderDevice()->SetRenderState(m_renderState);
	
		UINT nUnifom = m_pShaderProgram->GetUniformCount();
		for (UINT i = 0; i < nUnifom; ++i)
		{
			Uniform* pUniform = m_pShaderProgram->GetUniform(i);
			MaterialParameter* pMatPar = GetParameter( pUniform->m_name.c_str() );
			pMatPar->Bind(pUniform);
		}
	}

	void Technique::UnBind()
	{
		
	}

	void Technique::SetParameter(const char* pszName,const Any& value)	
	{
		MaterialParameter* pParame = GetParameter(pszName);
		ASSERT(pParame);
		if (pParame == NULL)
			return;

		pParame->SetValue(value);
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

	const char*	Technique::GetTechName()
	{
		return m_stName.c_str();
	}


}