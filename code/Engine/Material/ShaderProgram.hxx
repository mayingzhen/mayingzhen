#include "ShaderProgram.h"
//#include "PrePareShaderSource.h"
#include "ShaderManager.h"

namespace ma
{	
	ShaderProgram::ShaderProgram()
	{
	}
	
	ShaderProgram::~ShaderProgram()
	{
	}

	void ShaderProgram::Reload()
	{
		for (uint32_t i = 0; i < ShaderType_Number; ++i)
		{
			m_vecConstBuffer[i].clear();
		}
	
		m_eResState = ResUnLoad;

		CreateFromFile(m_strVSFile.c_str(),m_strPSFile.c_str(),m_shaderMacro.c_str(),m_pVertexDecl.get());
	}

	void ShaderProgram::CreateFromFile(const char* vshPath, const char* fshPath, const char* defines, VertexDeclaration* pVertexDecl)
	{
		if (m_eResState == ResInited)
		{
			ASSERT(m_strVSFile == vshPath);
			ASSERT(m_strPSFile == fshPath);
			ASSERT(m_shaderMacro == defines);
			return;
		}

		m_strVSFile = vshPath ? vshPath : "";
		m_strPSFile = fshPath ? fshPath : "";
		m_shaderMacro = defines ? defines: "";
        
        m_pVertexDecl = pVertexDecl;
        
		m_eResState = ResLoaded;
        
        GetRenderSystem()->ShaderStreamComplete(this);
        
        GetRenderSystem()->VertexDeclaComplete(pVertexDecl);
	}

	const char* ShaderProgram::GetVSFile() const
	{
		return m_strVSFile.c_str();
	}

	void ShaderProgram::SetVSFile(const char* pszVSFile)
	{
		m_strVSFile = pszVSFile ? pszVSFile : "";
	}

	const char*	ShaderProgram::GetPSFile() const
	{
		return m_strPSFile.c_str();
	}

	void ShaderProgram::SetPSFile(const char* pszPSFile)
	{
		m_strPSFile = pszPSFile ? pszPSFile : "";
	}

	const char*	ShaderProgram::GetShaderMacro() const
	{
		return m_shaderMacro.c_str();
	}

	void ShaderProgram::SetShaderMacro(const char* pszMacro)
	{
		m_shaderMacro = pszMacro ? pszMacro : "";
	}

	void ShaderProgram::AddSampler(Uniform* pUniform)
	{
		m_vecPSSamplers.push_back(pUniform);
	}

	uint32_t ShaderProgram::GetSamplerCount()
	{
		return m_vecPSSamplers.size();
	}

	Uniform* ShaderProgram::GetSamplerByIndex(uint32_t nIndex)
	{
		return m_vecPSSamplers[nIndex].get();
	}

	void ShaderProgram::AddConstBuffer(ShaderType eType, ConstantBuffer* pConstBuffer)
	{
		m_vecConstBuffer[eType].push_back(pConstBuffer);
	}

	uint32_t ShaderProgram::GetConstBufferCount(ShaderType eType)
	{
		return m_vecConstBuffer[eType].size();
	}

	ConstantBuffer*	ShaderProgram::GetConstBufferByIndex(ShaderType eType, uint32_t nIndex)
	{
		if (nIndex >= m_vecConstBuffer[eType].size())
			return NULL;

		return m_vecConstBuffer[eType][nIndex].get();
	}

	RefPtr<ShaderProgram> CreateShaderProgram(const char* pszVSFile,const char* pszPSFile,const char* pszMarco,VertexDeclaration* pVertexDecl)
	{
		return g_pShaderManager->CreateShader(pszVSFile,pszPSFile,pszMarco,pVertexDecl);
	}

}
