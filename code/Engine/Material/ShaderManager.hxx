#include "ShaderManager.h"

namespace ma
{
	ShaderManager::ShaderManager()
	{

	}

	ShaderManager::~ShaderManager()
	{

	}

	RefPtr<ShaderProgram> ShaderManager::CreateShader(const char* pszVSFile,const char* pszPSFile,const char* pszMarco,VertexDeclaration* pVertexDecl)
	{
		string stMarco = pszMarco ? pszMarco : "";
        
        Reskey key;
		key.m_strName = string(pszVSFile) + string("+") + pszPSFile + string("+") + stMarco + ".shader";
        key.m_nVertecDecl= pVertexDecl->GetHash();
        
		ResourceMap::iterator itRes = m_resMap.find(key);
		if (itRes != m_resMap.end())
			return itRes->second.get();

		RefPtr<ShaderProgram> pShader = GetRenderDevice()->CreateShaderProgram();
		pShader->CreateFromFile(pszVSFile,pszPSFile,pszMarco,pVertexDecl);
		m_resMap[key] = pShader;
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
