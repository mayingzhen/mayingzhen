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

			MaterialParameter* pParam = AddParameter( pUniform->m_name.c_str() );
			GetParameterManager()->UseDefaultBing(pParam);
		}
	}

	Technique::~Technique()
	{
		for (UINT i = 0; i < m_parameters.size(); ++i)
		{
			SAFE_DELETE(m_parameters[i]);
		}
		m_parameters.clear();
	
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
			MaterialParameter* pMatPar = GetParameter(pUniform->m_name.c_str());
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
			if (m_parameters[i] && strcmp(m_parameters[i]->GetName(), pszName) == 0)
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

// 	void Technique::Serialize(Serializer& sl, const char* pszLable/* = "Technique"*/)
// 	{
// 		sl.BeginSection(pszLable);
// 
// 		sl.Serialize(m_stName);
// 
// 		//m_pShaderProgram->
// 
// 		//m_renderState
// 
// 		sl.EndSection();
// 	}
}