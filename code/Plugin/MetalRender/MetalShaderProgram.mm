#include "MetalShaderProgram.h"
#include "MetalConstantBuffer.h"

namespace ma
{

	//vector<ConstantBuffer*> g_vecDirtyConstantBuffers;
	id<MTLBuffer> g_vecMetalConstantBuffers[2][MAX_SHADER_PARAMETER_GROUPS];

	MetalShaderProgram::MetalShaderProgram()
	{
		m_pVertexShader = NULL;
		m_pPiexelShader = NULL;
	}

	MetalShaderProgram::~MetalShaderProgram()
	{
		Destory();
	}

	void MetalShaderProgram::Destory()
	{
		//SAFE_RELEASE(m_pVertexShader);
		//SAFE_RELEASE(m_pPiexelShader);

		for (uint32 i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
		{
			m_vecVSConstantBuffers[i] = NULL;
			m_vecPSConstantBuffers[i] = NULL;
		}
	}

	void MetalShaderProgram::CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize)
	{
		Destory();

		ASSERT(vshSource);
		ASSERT(fshSource);
		if (vshSource == NULL || fshSource == NULL)
			return;

		if (vshSize > 0)
		{

		}


		if (fshSize > 0)
		{


		}

		return;
	}


	void MetalShaderProgram::ParseUniform()
	{
		ParseShaderUniform(VS,m_pByteVSCode,m_vecVSConstantBuffers);
		ParseShaderUniform(PS,m_pBytePSCode,m_vecPSConstantBuffers);
	}

	void MetalShaderProgram::ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
		RefPtr<ConstantBuffer> ConstantBuffersPtr[])
	{


	}

	void MetalShaderProgram::RT_SetShader()
	{

	}

	void MetalShaderProgram::CommitChanges()
	{
		for (UINT i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
		{
			if (m_vecVSConstantBuffers[i])
			{
				m_vecVSConstantBuffers[i]->Apply();
			}
			
			if (m_vecPSConstantBuffers[i])
			{
				m_vecPSConstantBuffers[i]->Apply();
			}
		}
	}

}

