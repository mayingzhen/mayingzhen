#include "Technqiue.h"

namespace ma
{
	Technique::Technique(Material* pMaterial,const char* pName)
	{
		m_pMaterial = pMaterial;
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
	}

	void Technique::UnBind()
	{
		
	}

	void Technique::CreateShaderProgram(const char* pszName,const char* pDefine)
	{
		ASSERT(pszName);
		if (pszName == NULL)
			return;

		ASSERT(m_pMaterial);
		if (m_pMaterial == NULL)
			return;

		std::string shaderDefine = m_pMaterial->GetMaterialFlage();
		if (pDefine)
		{
			shaderDefine = shaderDefine + ";" + pDefine;
		}

		m_pShaderProgram = GetRenderDevice()->CreateShaderProgram();
		m_pShaderProgram->CreateFromShaderName(pszName,shaderDefine.c_str());

		UINT nUniform = m_pShaderProgram->GetUniformCount();
		for (UINT i = 0; i < nUniform; ++i)
		{
			m_pMaterial->UseDefaultBing(m_pShaderProgram->GetUniform(i));
		}
	}

	const char*	Technique::GetTechName()
	{
		return m_stName.c_str();
	}
}