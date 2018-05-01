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
