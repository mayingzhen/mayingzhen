#pragma once

namespace ma
{
	class ShaderManager 
	{
	public:
		ShaderManager();

		~ShaderManager();

		RefPtr<ShaderProgram> CreateShader(const char* pszVSFile,const char* pszPSFile,const char* pszMarco);

		void ReLoad();

	private:
		typedef std::map<std::string, RefPtr<ShaderProgram> > ResourceMap;

		ResourceMap			m_resMap;
	};

	extern ShaderManager* g_pShaderManager;
}

