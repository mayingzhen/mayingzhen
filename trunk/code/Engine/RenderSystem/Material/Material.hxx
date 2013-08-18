#include "Material.h"
#include "ShaderProgram.h"


namespace ma
{
	Material::Material(const char* pMaterialFlag,const char* pShaderName) 
	{
		m_pCurTechnque = NULL;

		m_strMaterialFlag = pMaterialFlag ? pMaterialFlag : "";
		
		if (pShaderName)
		{
			m_pCurTechnque = CreateTechnique(pShaderName,pShaderName);
		}
			

	}

	Material::~Material()
	{
		//SAFE_DELETE(m_pShaderProgram);
	}

	Technique*	Material::CreateTechnique(const char* pTechName,const char* pShadrName, const char* pDefine)
	{
		ASSERT(pTechName);
		if (pTechName == NULL)
			return NULL;

		Technique* pTechnique = new Technique(this,pTechName);
		m_arrTechnique.push_back(pTechnique);
		pTechnique->CreateShaderProgram(pShadrName,pDefine);
		return pTechnique;
	}

	void Material::SetCurTechnqiue(const char* pShaderName,const char* pDefine)
	{
		std::string sTechName = pShaderName;
		if (pDefine && strcmp(pDefine,"") != 0)
		{
			sTechName += std::string("_") + pDefine;
		}

		for (Uint i = 0; i < m_arrTechnique.size(); ++i)
		{
			Technique* pTech = m_arrTechnique[i];
			ASSERT(pTech);
			if (pTech == NULL)
				continue;

			if ( strcmp(sTechName.c_str(),pTech->GetTechName()) == 0 )
			{
				m_pCurTechnque = m_arrTechnique[i];
				return;
			}
		}

		Technique* pTech = CreateTechnique(sTechName.c_str(),pShaderName,pDefine);
		m_pCurTechnque = pTech;

		//ASSERT(false);
	}

	void Material::Bind()
	{
		ASSERT(m_pCurTechnque);
		if (m_pCurTechnque == NULL)
			return;

		m_pCurTechnque->Bind();

		for (UINT i = 0; i < m_parameters.size(); ++i)
		{
			m_parameters[i]->bind(m_pCurTechnque->GetShaderProgram());
		}
	}

	void Material::UnBind()
	{
		//m_pShaderProgram->UnBind();
	}

	MaterialParameter* Material::GetParameter(const char* name) 
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

	void Material::ClearParameter(const char* name)
	{
		for (size_t i = 0, count = m_parameters.size(); i < count; ++i)
		{
			MaterialParameter* p = m_parameters[i];
			if (p->_name == name)
			{
				m_parameters.erase(m_parameters.begin() + i);
				//SAFE_RELEASE(p);
				break;
			}
		}
	}


	void Material::SetParameterAutoBinding(const char* name, AutoBinding autoBinding)
	{
		MaterialParameter* param = GetParameter(name);
		ASSERT(param);

		GetMaterialManager()->SetParameterAutoBinding(param,autoBinding);

	}
}
