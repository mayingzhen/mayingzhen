#include "ShaderManager.h"

namespace ma
{
	ShaderManager::ShaderManager()
	{

	}

	ShaderManager::~ShaderManager()
	{

	}

	RefPtr<ShaderProgram> ShaderManager::CreateShader(const ShaderCreateInfo& createInfo)
	{
		auto itRes = m_resMap.find(createInfo);
		if (itRes != m_resMap.end())
			return itRes->second.get();

		RefPtr<ShaderProgram> pShader = GetRenderDevice()->CreateShaderProgram();
		pShader->CreateFromFile(createInfo);
		m_resMap[createInfo] = pShader;
		return pShader;	
	}	

	void ShaderManager::ReLoad()
	{
		ResourceMap::iterator it = m_resMap.begin();
		for (; it != m_resMap.end(); ++it)
		{
			ShaderProgram* pRes = it->second.get();
			pRes->Reload();
		}
	}

	ShaderManager* g_pShaderManager = NULL;
}
