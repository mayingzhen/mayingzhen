#include "Technqiue.h"

namespace ma
{
	Technique::Technique(const char* pName,const char* pShadrName, const char* pDefine)
	{
		m_stName = pName;

		m_pShaderProgram = GetRenderSystem()->CreateShaderProgram(this,pShadrName,pDefine);
		UINT nUniform = m_pShaderProgram->GetUniformCount();
		for (UINT i = 0; i < nUniform; ++i)
		{
			Uniform* pUniform = m_pShaderProgram->GetUniform(i);
			ASSERT(pUniform);
			if (pUniform == NULL)
				continue;

			MaterialParameter* pParam = /*m_pEffect->*/AddParameter( pUniform->m_name.c_str() );
			GetParameterManager()->UseDefaultBing(pParam);
		}
	}

	Technique::~Technique()
	{

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
			MaterialParameter* pMatPar =/* m_pEffect->*/GetParameter(pUniform->m_name.c_str());
			pMatPar->Bind(pUniform);
		}
	}

	void Technique::UnBind()
	{
		
	}

	MaterialParameter* Technique::GetParameter(const char* pszName)
	{
		ASSERT(pszName);
		for (UINT i = 0; i < m_parameters.size(); ++i)
		{
			if (m_parameters[i] && strcmp(m_parameters[i]->getName(), pszName) == 0)
			{
				return m_parameters[i];
			}
		}

		return NULL;
	}

	MaterialParameter*	Technique::AddParameter(const char* pszName)
	{
		MaterialParameter* pParm = GetParameter(pszName);
		ASSERT(!pParm && "Parameter finded");
		if (pParm)
			return pParm;

		pParm = new MaterialParameter(pszName);
		m_parameters.push_back(pParm);
		return pParm;
	}


	const char*	Technique::GetTechName()
	{
		return m_stName.c_str();
	}
}