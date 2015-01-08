#include "ShaderProgram.h"
#include "PrePareShaderSource.h"

namespace ma
{	
	IMPL_OBJECT(ShaderProgram,Resource)

	ShaderProgram::ShaderProgram(const char* pszPath/* = NULL*/)
		:Resource(pszPath)
	{

	}
	

	ShaderProgram::~ShaderProgram()
	{
		m_uniforms.clear();
	}


	void ShaderProgram::Serialize(Serializer& sl, const char* pszLable/* = "Resource"*/)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_strVSFile,"m_strVSFile");
		sl.Serialize(m_strPSFile,"m_strVSFile");
		sl.Serialize(m_shaderDefine,"m_shaderDefine");

		sl.EndSection();
	}

	void ShaderProgram::CreateFromFile(const char* vshPath, const char* fshPath, const char* defines/* = NULL*/)
	{
		if (m_eResState == ResInited)
		{
			ASSERT(m_strVSFile == vshPath);
			ASSERT(m_strPSFile == fshPath);
			ASSERT(m_shaderDefine == defines);
			return;
		}

		m_strVSFile = vshPath ? vshPath : "";
		m_strPSFile = fshPath ? fshPath : "";
		m_shaderDefine = defines ? defines: "";

		m_eResState = ResLoaded;

		CreateFromMemeory();
	}

	void ShaderProgram::RT_StreamComplete()
	{
		std::string strPath = GetRenderSystem()->GetShaderPath();

		std::string strPathVS = strPath + m_strVSFile + ".vert";
		std::string strPathFS = strPath + m_strPSFile + ".frag";

		std::string strVshSource = PrePareShaderSource(strPathVS.c_str(),m_shaderDefine.c_str());
		std::string strFshSource = PrePareShaderSource(strPathFS.c_str(),m_shaderDefine.c_str());

		CreateFromSource(strVshSource.c_str(), strVshSource.length(), 
			strFshSource.c_str(), strFshSource.length());

		ParseUniform();

		m_eResState = ResInited;
	}

	bool ShaderProgram::CreateFromMemeory()
	{
		if (m_eResState == ResInited)
			return true;

		ASSERT(m_eResState == ResLoaded);
		if (m_eResState != ResLoaded)
			return false;

		GetRenderSystem()->ShaderStreamComplete(this);

		m_eResState = ResInited;

		return true;
	}

	Uniform* ShaderProgram::GetUniform(const char* name) const
	{
		ASSERT(name);
		if (name == NULL)
			return NULL;

		for (UINT i = 0; i < m_uniforms.size(); ++i)
		{
			if (strcmp(name,m_uniforms[i]->m_name.c_str()) == 0)
			{
				return m_uniforms[i].get();
			}
		}
		
		return NULL;
	}

	Uniform* ShaderProgram::GetUniform(UINT index) const
	{
		return m_uniforms[index].get();
	}

	UINT ShaderProgram::GetUniformCount() const
	{
		return m_uniforms.size();
	}

	const char* ShaderProgram::GetVSFile() const
	{
		return m_strVSFile.c_str();
	}

	const char*	ShaderProgram::GetPSFile() const
	{
		return m_strPSFile.c_str();
	}

	const char*	ShaderProgram::GetShaderDefine() const
	{
		return m_shaderDefine.c_str();
	}

	Uniform* ShaderProgram::AddUniform(const char* pName)
	{
		Uniform* pUnifrom = new Uniform();
		pUnifrom->m_name = pName ? pName : "";
		m_uniforms.push_back(pUnifrom);
		return pUnifrom;
	}

}
