#pragma once

namespace ma
{
    class VertexDeclaration;
    
	class ShaderManager 
	{
	public:
		ShaderManager();

		~ShaderManager();

		RefPtr<ShaderProgram> CreateShader(const char* pszVSFile,const char* pszPSFile,const char* pszMarco,VertexDeclaration* pVertexDecl);

		void ReLoad();

		void Clear() {m_resMap.clear();}

	private:
        
        struct Reskey
        {
            std::string m_strName;
            uint64_t m_nVertecDecl;
            
            bool operator < (const Reskey& other) const
            {
                if (m_nVertecDecl != other.m_nVertecDecl)
                    return m_nVertecDecl < other.m_nVertecDecl;
                
                if (m_strName != other.m_strName)
                    return m_strName < other.m_strName;
                
                return false;
            }
        };
        
		typedef std::map<Reskey, RefPtr<ShaderProgram> > ResourceMap;

		ResourceMap			m_resMap;
	};

	extern ShaderManager* g_pShaderManager;
}

