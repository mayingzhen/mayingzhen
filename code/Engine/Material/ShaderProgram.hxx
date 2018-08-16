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
		if (m_createInfo.m_pRenderPass == NULL)
		{
			m_createInfo.m_pRenderPass = GetRenderSystem()->GetDefaultRenderPass();
		}
        
		m_eResState = ResLoaded;
        
        GetRenderSystem()->ShaderStreamComplete(this);
	}

	void ShaderProgram::AddSampler(ShaderType eType, Uniform* pUniform)
	{
		m_vecSamplers[eType].push_back(pUniform);
	}

	uint32_t ShaderProgram::GetSamplerCount(ShaderType eType)
	{
		return m_vecSamplers[eType].size();
	}

	Uniform* ShaderProgram::GetSamplerByIndex(ShaderType eType, uint32_t nIndex)
	{
		return m_vecSamplers[eType][nIndex].get();
	}

	void ShaderProgram::AddConstBuffer(ShaderType eType, ConstantBuffer* pConstBuffer)
	{
		m_vecConstBuffer[eType].push_back(pConstBuffer);
	}

	void ShaderProgram::AddStorgeBuffer(Uniform* pUniform)
	{
		m_vecStorgeBuffer.push_back(pUniform);
	}

	uint32_t ShaderProgram::GetStorgeBufferCount()
	{
		return m_vecStorgeBuffer.size();
	}

	Uniform* ShaderProgram::GetStorgeBufferByIndex(uint32_t nIndex)
	{
		ASSERT(nIndex < m_vecStorgeBuffer.size());
		if (nIndex >= m_vecStorgeBuffer.size())
			return NULL;

		return m_vecStorgeBuffer[nIndex].get();
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
