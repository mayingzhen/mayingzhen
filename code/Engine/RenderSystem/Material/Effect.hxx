#include "Effect.h"


namespace ma
{

	Effect::Effect(const char* pEffectName)
	{
		m_pCurTechnque = NULL;
		m_strEffectName = pEffectName;
	}

// 	void Effect::Begin(const char* pTechName)
// 	{
// 		ASSERT(pTechName);
// 		if (pTechName == NULL)
// 			return;
// 
// 		for (Uint i = 0; i < m_arrTechnique.size(); ++i)
// 		{
// 			Technique* pTech = m_arrTechnique[i];
// 			ASSERT(pTech);
// 			if (pTech == NULL)
// 				continue;
// 
// 			if ( strcmp(pTechName,pTech->GetTechName()) == 0 )
// 			{
// 				m_pCurTechnque = m_arrTechnique[i];
// 				return;
// 			}
// 		}
// 	}

	void Effect::SetCurCurTechnqiue(const char* pTechName)
	{
		ASSERT(pTechName);
		if (pTechName == NULL)
			return;

		for (Uint i = 0; i < m_arrTechnique.size(); ++i)
		{
			Technique* pTech = m_arrTechnique[i];
			ASSERT(pTech);
			if (pTech == NULL)
				continue;

			if ( strcmp(pTechName,pTech->GetTechName()) == 0 )
			{
				m_pCurTechnque = m_arrTechnique[i];
				return;
			}
		}
	}

	Technique*	Effect::GetTechnqiue(const char* pTechName)
	{
		ASSERT(pTechName);
		if (pTechName == NULL)
			return NULL;

		for (Uint i = 0; i < m_arrTechnique.size(); ++i)
		{
			Technique* pTech = m_arrTechnique[i];
			ASSERT(pTech);
			if (pTech == NULL)
				continue;

			if ( strcmp(pTechName,pTech->GetTechName()) == 0 )
			{
				return pTech;
			}
		}

		return NULL;
	}

// 	void Effect::CommitParam()
// 	{
// 		ASSERT(m_pCurTechnque);
// 		if (m_pCurTechnque == NULL)
// 			return;
// 
// 		m_pCurTechnque->Bind();
// 	}
// 
// 	void Effect::End()
// 	{
// 		m_pCurTechnque->UnBind();
// 
// 		m_pCurTechnque = NULL;
// 	}

	void Effect::Bind()
	{
		ASSERT(m_pCurTechnque);
		if (m_pCurTechnque == NULL)
			return;

		m_pCurTechnque->Bind();
	}

	void Effect::UnBind()
	{
		ASSERT(m_pCurTechnque);
		if (m_pCurTechnque == NULL)
			return;

		m_pCurTechnque->UnBind();
	}


	Technique*	Effect::AddTechnique(const char* pTechName,const char* pShadrName, const char* pDefine)
	{
		ASSERT(pTechName);
		if (pTechName == NULL)
			return NULL;

		Technique* pTechnique = new Technique(this,pTechName);
		m_arrTechnique.push_back(pTechnique);
		pTechnique->CreateShaderProgram(pShadrName,pDefine);
		m_pCurTechnque = pTechnique;
		return pTechnique;
	}

	MaterialParameter* Effect::GetParameter(const char* name)
	{
		ASSERT(name);

		// Search for an existing parameter with this name.
		MaterialParameter* param;
		for (size_t i = 0, count = m_parameters.size(); i < count; ++i)
		{
			param = m_parameters[i];
			ASSERT(param);
			if (strcmp(param->getName(), name) == 0)
			{
				return param;
			}
		}

		// Create a new parameter and store it in our list.
		param = new MaterialParameter(name);
		m_parameters.push_back(param);

		return param;
	}

	void Effect::AddParameter(MaterialParameter* pParam)
	{
		m_parameters.push_back(pParam);
	}
}

