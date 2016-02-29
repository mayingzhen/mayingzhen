#include "ShaderManager.h"

namespace ma
{
	ShaderManager::ShaderManager()
	{

	}

	ShaderManager::~ShaderManager()
	{

	}

	RefPtr<ShaderProgram> ShaderManager::CreateShader(const char* pszVSFile,const char* pszPSFile,const char* pszMarco)
	{
		string stMarco = pszMarco ? pszMarco : "";
		string strKey = string(pszVSFile) + string("+") + pszPSFile + string("+") + stMarco + ".shader";

		ResourceMap::iterator itRes = m_resMap.find(strKey);
		if (itRes != m_resMap.end())
			return itRes->second.get();

		RefPtr<ShaderProgram> pShader = GetRenderDevice()->CreateShaderProgram();
		pShader->CreateFromFile(pszVSFile,pszPSFile,pszMarco);
		m_resMap[strKey] = pShader;
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
