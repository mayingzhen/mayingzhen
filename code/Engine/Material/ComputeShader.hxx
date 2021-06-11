#include "ComputeShader.h"

namespace ma
{	

	ComputeShader::ComputeShader()
	{
	}
	
	ComputeShader::~ComputeShader()
	{
	}

	void ComputeShader::CreateFromFile(const std::string& strFile, const std::string& strMacro)
	{
		if (m_eResState == ResReady)
		{
			ASSERT(false);

			return;
		}

		m_strFile = strFile;
		m_strMacro = strMacro;
        
		m_eResState = ResLoadIng;
        
        //GetRenderSystem()->ShaderStreamComplete(this);
	}

	void ComputeShader::AddConstBuffer(ConstantBuffer* pConstBuffer)
	{
		m_vecConstBuffer.push_back(pConstBuffer);
	}

	void ComputeShader::AddStorgeBuffer(Uniform* pUniform)
	{
		m_vecStorgeBuffer.push_back(pUniform);
	}

	uint32_t ComputeShader::GetStorgeBufferCount()
	{
		return m_vecStorgeBuffer.size();
	}

	Uniform* ComputeShader::GetStorgeBufferByIndex(uint32_t nIndex)
	{
		ASSERT(nIndex < m_vecStorgeBuffer.size());
		if (nIndex >= m_vecStorgeBuffer.size())
			return NULL;

		return m_vecStorgeBuffer[nIndex].get();
	}

	uint32_t ComputeShader::GetConstBufferCount()
	{
		return m_vecConstBuffer.size();
	}

	ConstantBuffer* ComputeShader::GetConstBufferByIndex(uint32_t nIndex)
	{
		if (nIndex >= m_vecConstBuffer.size())
			return NULL;

		return m_vecConstBuffer[nIndex].get();
	}

	RefPtr<ComputeShader> CreateComputeShader(const std::string& strFile, const std::string& strMacro)
	{
		return g_pShaderManager->CreateShader(createInfo);
	}

}
