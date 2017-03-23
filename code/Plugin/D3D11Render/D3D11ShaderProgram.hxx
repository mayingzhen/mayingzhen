#include "D3D11ShaderProgram.h"



namespace ma
{
	D3D11ShaderProgram::D3D11ShaderProgram()
	{
		m_pVertexShader = NULL;
		m_pPiexelShader = NULL;
	}

	D3D11ShaderProgram::~D3D11ShaderProgram()
	{
		Destory();
	}

	void D3D11ShaderProgram::Destory()
	{
		SAFE_RELEASE(m_pVertexShader);
		SAFE_RELEASE(m_pPiexelShader);

		for (uint32 i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
		{
			m_vecVSConstantBuffers[i] = NULL;
			m_vecPSConstantBuffers[i] = NULL;
		}
	}

	void D3D11ShaderProgram::CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize)
	{
		Destory();

		ASSERT(vshSource);
		ASSERT(fshSource);
		if (vshSource == NULL || fshSource == NULL)
			return;

		HRESULT hr = S_OK;
		ID3D10Blob* pCode = NULL;
		ID3D10Blob* error = NULL;


		DWORD dwShaderFlags = 0;
#ifdef _DEBUG
		dwShaderFlags |= D3D10_SHADER_DEBUG;

		char pszPath[MAX_PATH] = { 0 };
		GetFullPathName("ShaderCach/D3D11/", MAX_PATH, pszPath, NULL);
		std::string strDir = pszPath;
		std::string strVshName = strDir + string(GetVSFile()) + "_" + string(GetShaderMacro()) + ".vsh";
		std::string strFshName = strDir + string(GetPSFile()) + "_" + string(GetShaderMacro()) + ".fsh";
		StaticFunc::MakeDir(StaticFunc::GetFileDir(strVshName).c_str());
		StaticFunc::MakeDir(StaticFunc::GetFileDir(strFshName).c_str());

		{
			std::ofstream saveVSile;
			saveVSile.open(strVshName.c_str());
			saveVSile << vshSource;
			saveVSile.close();

			std::ofstream saveFSile;
			saveFSile.open(strFshName.c_str());
			saveFSile << fshSource;
			saveFSile.close();
		}
#else
		dwShaderFlags != D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif 

		if (vshSize > 0)
		{
// #ifdef _DEBUG
// 			hr = D3DX11CompileFromFile(
// 				strVshName.c_str(),
// 				NULL,
// 				NULL,
// 				"main",
// 				"vs_4_0", 
// 				dwShaderFlags,
// 				dwShaderFlags,
// 				NULL,
// 				&pCode,
// 				&error,
// 				&hr);
// #else
			hr = D3DCompile(
				vshSource, 
				vshSize,
				NULL, 
				NULL,
				NULL,
				"main",
				"vs_4_0", 
				dwShaderFlags, 
				dwShaderFlags,
				&pCode,
				&error) ;
//#endif
			if (hr != S_OK)
			{
				if (error)
				{
					const char* pErrCode = (const char*)error->GetBufferPointer();
					LogError(pErrCode);
					ASSERT(false);
				}
			}
			else
			{
				m_pByteVSCode.resize((unsigned)pCode->GetBufferSize());
				memcpy(&m_pByteVSCode[0], pCode->GetBufferPointer(), m_pByteVSCode.size());
				hr = GetD3D11DxDevive()->CreateVertexShader(&m_pByteVSCode[0], m_pByteVSCode.size(), NULL, &m_pVertexShader);
				ASSERT(hr == S_OK);
			}
		}


		if (fshSize > 0)
		{
// #if _DEBUG
// 			hr = D3DX11CompileFromFile(
// 				strFshName.c_str(),
// 				NULL,
// 				NULL,
// 				"main",
// 				"ps_4_0", 
// 				dwShaderFlags,
// 				dwShaderFlags,
// 				NULL,
// 				&pCode,
// 				&error,
// 				&hr);
// #else
			hr = D3DCompile(
				fshSource, 
				fshSize,
				NULL, 
				NULL,
				NULL,
				"main",
				"ps_4_0", 
				dwShaderFlags, 
				dwShaderFlags,
				&pCode,
				&error) ;
//#endif
			if (hr != S_OK)
			{
				const char* pErrCode = (const char*)error->GetBufferPointer();
				LogError(pErrCode);
				ASSERT(false);
			}
			else
			{
				m_pBytePSCode.resize((unsigned)pCode->GetBufferSize());
				memcpy(&m_pBytePSCode[0], pCode->GetBufferPointer(), m_pBytePSCode.size());

				hr = GetD3D11DxDevive()->CreatePixelShader(pCode->GetBufferPointer(), pCode->GetBufferSize(), NULL, &m_pPiexelShader);
				ASSERT(hr == S_OK);
			}
		}

		return;
	}


	void D3D11ShaderProgram::ParseUniform()
	{
		ParseShaderUniform(VS,m_pByteVSCode,m_vecVSConstantBuffers);
		ParseShaderUniform(PS,m_pBytePSCode,m_vecPSConstantBuffers);
	}

	void D3D11ShaderProgram::ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
		RefPtr<ConstantBuffer> ConstantBuffersPtr[])
	{
		ID3D11ShaderReflection* reflection = 0;
		D3D11_SHADER_DESC shaderDesc;

		D3DReflect(&vecByteCode[0], vecByteCode.size(), IID_ID3D11ShaderReflection, (void**)&reflection);
		if (!reflection)
		{
			LogError("Failed to reflect vertex shader's input signature");
			return;
		}

		reflection->GetDesc(&shaderDesc);

// 		if (eType == VS)
// 		{
// 			for (unsigned i = 0; i < shaderDesc.InputParameters; ++i)
// 			{
// 				D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
// 				reflection->GetInputParameterDesc((UINT)i, &paramDesc);
// 				for (unsigned j = 0; j < MAX_DECL_USAGE; ++j)
// 				{
// 					if (string(paramDesc.SemanticName) == string(elementSemantics[j]) &&
// 						paramDesc.SemanticIndex == elementSemanticIndices[j])
// 					{
// 						elementMask_ |= (1 << j);
// 						break;
// 					}
// 				}
// 			}
// 		}

		map<string, unsigned> cbRegisterMap;

		for (unsigned i = 0; i < shaderDesc.BoundResources; ++i)
		{
			D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
			reflection->GetResourceBindingDesc(i, &resourceDesc);
			string resourceName(resourceDesc.Name);
			if (resourceDesc.Type == D3D_SIT_CBUFFER)
			{
				cbRegisterMap[resourceName] = resourceDesc.BindPoint;
			}
			else if (resourceDesc.Type == D3D_SIT_TEXTURE)
			{
				Uniform* pUniform = this->AddUniform(resourceDesc.Name);
				pUniform->m_index = resourceDesc.BindPoint;
				pUniform->m_vshShder = false;
			}
		}

		D3D11RenderDevice* pD3D11Device = (D3D11RenderDevice*)GetRenderDevice();

		for (unsigned i = 0; i < shaderDesc.ConstantBuffers; ++i)
		{
			ID3D11ShaderReflectionConstantBuffer* cb = reflection->GetConstantBufferByIndex(i);
			D3D11_SHADER_BUFFER_DESC cbDesc;
			cb->GetDesc(&cbDesc);
			unsigned cbRegister = cbRegisterMap[string(cbDesc.Name)];

			ConstantBuffer* pConstantBuffer = pD3D11Device->GetOrCreateConstantBuffer(eType, cbRegister, cbDesc.Size);
			ConstantBuffersPtr[cbRegister] = pConstantBuffer;

			for (unsigned j = 0; j < cbDesc.Variables; ++j)
			{
				ID3D11ShaderReflectionVariable* var = cb->GetVariableByIndex(j);
				D3D11_SHADER_VARIABLE_DESC varDesc;
				var->GetDesc(&varDesc);

				if (varDesc.uFlags & D3D_SVF_USED)
				{
					Uniform* pUniform = this->AddUniform(varDesc.Name);
					pUniform->m_vshShder = eType == VS;

					pUniform->m_nCBOffset = varDesc.StartOffset;
					pUniform->m_nCBSize = varDesc.Size;
					pUniform->m_pD3D11CBPtr = pConstantBuffer;
				}
			}
		}

		reflection->Release();
	}

	void D3D11ShaderProgram::RT_SetShader()
	{
		GetD3D11DxDeviveContext()->VSSetShader(m_pVertexShader, NULL, 0);

		GetD3D11DxDeviveContext()->PSSetShader(m_pPiexelShader, NULL, 0);

		D3D11RenderDevice* pD3D11Device = (D3D11RenderDevice*)GetRenderDevice();
		pD3D11Device->SetShaderProgram(this);
	}

}

