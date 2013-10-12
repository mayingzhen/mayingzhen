#include "Technqiue.h"

namespace ma
{
	Technique::Technique(Effect* pEffect,const char* pName)
	{
		m_pEffect = pEffect;
		m_stName = pName;
		m_pShaderProgram = NULL;
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
			MaterialParameter* pMatPar = m_pEffect->GetParameter(pUniform->getName());
			pMatPar->Bind(pUniform);
		}
	}

	void Technique::UnBind()
	{
		
	}

	void Technique::CreateShaderProgram(const char* pszName,const char* pDefine)
	{
		m_pShaderProgram = GetRenderSystem()->CreateShaderProgram(pszName,pDefine);

		UINT nUniform = m_pShaderProgram->GetUniformCount();
		for (UINT i = 0; i < nUniform; ++i)
		{
			Uniform* pUniform = m_pShaderProgram->GetUniform(i);
			ASSERT(pUniform);
			if (pUniform == NULL)
				continue;

			MaterialParameter* pParam = m_pEffect->GetParameter(pUniform->getName());
			GetMaterialManager()->UseDefaultBing(pParam);
		}
	}

	const char*	Technique::GetTechName()
	{
		return m_stName.c_str();
	}
}