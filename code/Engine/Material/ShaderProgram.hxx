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

		CreateFromFile(m_createInfo);
	}

	void ShaderProgram::CreateFromFile(const ShaderCreateInfo& createInfo)
	{
		if (m_eResState == ResInited)
		{
			m_createInfo = createInfo;
			
			return;
		}

		m_createInfo = createInfo;
        
		m_eResState = ResLoaded;
        
        GetRenderSystem()->ShaderStreamComplete(this);
	}

	const char* ShaderProgram::GetVSFile() const
	{
		return m_createInfo.m_strVSFile.c_str();
	}

	void ShaderProgram::SetVSFile(const char* pszVSFile)
	{
		m_createInfo.m_strVSFile = pszVSFile ? pszVSFile : "";
	}

	const char*	ShaderProgram::GetPSFile() const
	{
		return m_createInfo.m_strPSFile.c_str();
	}

	void ShaderProgram::SetPSFile(const char* pszPSFile)
	{
		m_createInfo.m_strPSFile = pszPSFile ? pszPSFile : "";
	}

	const char*	ShaderProgram::GetShaderMacro() const
	{
		return m_createInfo.m_shaderMacro.c_str();
	}

	void ShaderProgram::SetShaderMacro(const char* pszMacro)
	{
		m_createInfo.m_shaderMacro = pszMacro ? pszMacro : "";
	}

	int ShaderProgram::GetShaderMacroInt(const char* pszMacro)
	{
		ASSERT(pszMacro);
		if (pszMacro == NULL)
			return false;

		std::string stdShder = this->GetShaderMacro();

		const vector<string> vecMacros = StringUtil::split(stdShder, ";");

		if (vecMacros.empty())
			return 0;

		for (uint32_t i = 0; i < vecMacros.size(); i++)
		{
			const vector<string> keyValue = StringUtil::split(vecMacros[i], "=");
			uint32_t nSize = keyValue.size();
			if (nSize != 2)
				continue;

			if (keyValue[0] != string(pszMacro))
				continue;

			return StringConverter::parseInt(keyValue[1]);
		}

		return 0;
	}

	void ShaderProgram::SetShaderMacroInt(const char* pszMacro, int nValue)
	{
		ASSERT(pszMacro);
		if (pszMacro == NULL)
			return;

		std::string strShder = this->GetShaderMacro();

		vector<string> vecMacros = StringUtil::split(strShder, ";");
		if (vecMacros.empty())
			return;

		uint32_t i = 1;
		for (; i < vecMacros.size(); ++i)
		{
			vector<string> keyValue = StringUtil::split(vecMacros[i], "=");
			uint32_t nSize = keyValue.size();
			if (nSize != 2)
				continue;

			if (keyValue[0] != string(pszMacro))
				continue;

			keyValue[1] = StringConverter::toString(nValue);

			vecMacros[i] = keyValue[0] + "=" + keyValue[1];
			break;
		}

		if (i == vecMacros.size())
		{
			string strKey = pszMacro;
			string strValue = StringConverter::toString(nValue);
			vecMacros.push_back(strKey + "=" + strValue);
		}

		std::sort(vecMacros.begin() + 1, vecMacros.end());

		string strFinal = vecMacros[0];
		for (uint32_t i = 1; i < vecMacros.size(); ++i)
		{
			strFinal += ";" + vecMacros[i];
		}

		m_createInfo.m_shaderMacro = strFinal;

		this->Reload();
	}

	bool ShaderProgram::GetShaderMacroBool(const char* pszMacro)
	{
		ASSERT(pszMacro);
		if (pszMacro == NULL)
			return false;

		std::string strShder = this->GetShaderMacro();

		vector<string> vecMacros = StringUtil::split(strShder, ";");
		if (vecMacros.empty())
			return false;

		for (uint32_t i = 0; i < vecMacros.size(); ++i)
		{
			if (vecMacros[i] == string(pszMacro))
			{
				return true;
			}
		}

		return false;
	}

	void ShaderProgram::SetShaderMacroBool(const char* pszMacro, bool b)
	{
		ASSERT(pszMacro);
		if (pszMacro == NULL)
			return;

		std::string strShder = this->GetShaderMacro();

		vector<string> vecMacros = StringUtil::split(strShder, ";");
		if (vecMacros.empty())
			return;

		uint32_t i = 1;
		for (; i < vecMacros.size(); ++i)
		{
			if (vecMacros[i] == string(pszMacro))
			{
				break;
			}
		}

		if (i == vecMacros.size())
		{
			if (b)
				vecMacros.push_back(pszMacro);
		}
		else
		{
			if (!b)
				vecMacros.erase(vecMacros.begin() + i);
		}

		std::sort(vecMacros.begin() + 1, vecMacros.end());

		string strFinal = vecMacros[0];
		for (uint32_t i = 1; i < vecMacros.size(); ++i)
		{
			strFinal += ";" + vecMacros[i];
		}

		m_createInfo.m_shaderMacro = strFinal;

		this->Reload();
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

	RefPtr<ShaderProgram> CreateShaderProgram(const ShaderCreateInfo& createInfo)
	{
		return g_pShaderManager->CreateShader(createInfo);
	}

}
