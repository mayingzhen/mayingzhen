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
			if (shaderDefine == "")
				shaderDefine = pDefine;
			else
				shaderDefine = shaderDefine + ";" + pDefine;
		}

		m_pShaderProgram = GetRenderDevice()->CreateShaderProgram();
		m_pShaderProgram->CreateFromShaderName(pszName,shaderDefine.c_str());

		UINT nUniform = m_pShaderProgram->GetUniformCount();
		for (UINT i = 0; i < nUniform; ++i)
		{
			Uniform* pUniform = m_pShaderProgram->GetUniform(i);
			ASSERT(pUniform);
			if (pUniform == NULL)
				continue;

			MaterialParameter* pParam = m_pMaterial->GetParameter(pUniform->getName());
			GetMaterialManager()->UseDefaultBing(pParam);
		}
	}

	const char*	Technique::GetTechName()
	{
		return m_stName.c_str();
	}
}