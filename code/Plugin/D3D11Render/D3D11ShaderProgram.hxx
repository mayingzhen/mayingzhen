#include "D3D11ShaderProgram.h"



namespace ma
{
	D3D11ShaderProgram::D3D11ShaderProgram()
	{
		m_pVertexShader = NULL;
		m_pPiexelShader = NULL;
		m_pVertexShader = NULL;
		//m_pVShConstantTable = NULL;
		//m_pPShConstantTable = NULL;
	}

	D3D11ShaderProgram::~D3D11ShaderProgram()
	{
		Destory();
	}
	
	void D3D11ShaderProgram::Destory()
	{
		//SAFE_RELEASE(m_pVShConstantTable);
		//SAFE_RELEASE(m_pPShConstantTable);

		SAFE_RELEASE(m_pVertexShader);
		SAFE_RELEASE(m_pPiexelShader);
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
		dwShaderFlags |= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;

		char pszPath[MAX_PATH] = {0};
		GetFullPathName("ShaderCach/D3D11/",MAX_PATH,pszPath,NULL);
		std::string strDir = pszPath;
		std::string strVshName = strDir + string(GetVSFile()) + "_" + string(GetShaderMacro()) + ".vsh";
		std::string strFshName = strDir + string(GetPSFile()) + "_" + string(GetShaderMacro()) + ".fsh";
		StaticFunc::MakeDir( StaticFunc::GetFileDir(strVshName).c_str() );
		StaticFunc::MakeDir( StaticFunc::GetFileDir(strFshName).c_str() );
		
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

#endif // _DEBUG

		if (vshSize > 0)
		{	
#ifdef _DEBUG
			hr = D3DX11CompileFromFile ( 
				strVshName.c_str(),
				NULL, 
				NULL,
				"main",
				"vs_4_0", 
				dwShaderFlags, 
				dwShaderFlags,
				NULL,
				&pCode,
				&error, 
				&hr ) ;
#else
			hr = D3DX11CompileFromMemory ( 
				vshSource, 
				vshSize,
				NULL, 
				NULL,
				"main",
				"vs_4_0", 
				dwShaderFlags, 
				dwShaderFlags,
				NULL,
				&pCode,
				&error, 
				&hr) ;
#endif
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
				// Then strip everything not necessary to use the shader
				ID3DBlob* strippedCode = pCode;
				//D3DStripShader(pCode->GetBufferPointer(), pCode->GetBufferSize(),
				//	D3DCOMPILER_STRIP_REFLECTION_DATA | D3DCOMPILER_STRIP_DEBUG_INFO | D3DCOMPILER_STRIP_TEST_BLOBS, &strippedCode);
				m_pByteVSCode.resize((unsigned)strippedCode->GetBufferSize());
				memcpy(&m_pByteVSCode[0], strippedCode->GetBufferPointer(), m_pByteVSCode.size());
				strippedCode->Release();
				hr = GetD3D11DxDevive()->CreateVertexShader( &m_pByteVSCode[0],m_pByteVSCode.size(),NULL,&m_pVertexShader );
				ASSERT(hr == S_OK);
			}
		}


		if (fshSize > 0)
		{
#if _DEBUG
			hr = D3DX11CompileFromFile( 
				strFshName.c_str(), 
				NULL, 
				NULL,
				"main",
				"ps_4_0", 
				dwShaderFlags, 
				dwShaderFlags,
				NULL,
				&pCode,
				&error, 
				&hr ) ;
#else
			hr = D3DX11CompileFromMemory( 
				fshSource, 
				fshSize,
				NULL, 
				NULL,
				"main",
				"ps_4_0", 
				dwShaderFlags, 
				dwShaderFlags,
				NULL,
				&pCode,
				&error, 
				&hr ) ;
#endif
			if (hr != S_OK)
			{
				const char* pErrCode = (const char*)error->GetBufferPointer();	
				LogError(pErrCode);
				ASSERT(false);
			}
			else
			{
				// Then strip everything not necessary to use the shader
				ID3DBlob* strippedCode = pCode;
				//D3DStripShader(pCode->GetBufferPointer(), pCode->GetBufferSize(),
				//	D3DCOMPILER_STRIP_REFLECTION_DATA | D3DCOMPILER_STRIP_DEBUG_INFO | D3DCOMPILER_STRIP_TEST_BLOBS, &strippedCode);
				m_pBytePSCode.resize((unsigned)strippedCode->GetBufferSize());
				memcpy(&m_pBytePSCode[0], strippedCode->GetBufferPointer(), m_pBytePSCode.size());
				strippedCode->Release();

				hr = GetD3D11DxDevive()->CreatePixelShader( &m_pBytePSCode[0], m_pBytePSCode.size(),NULL, &m_pPiexelShader );
				ASSERT(hr == S_OK);
			}
		}

		return;
	}

	const char* elementSemantics[] =
	{
		"POSITION",
		"NORMAL",
		"COLOR",
		"TEXCOORD",
		"TEXCOORD",
		"TEXCOORD",
		"TEXCOORD",
		"TANGENT",
		"BLENDWEIGHT",
		"BLENDINDICES",
		"TEXCOORD",
		"TEXCOORD",
		"TEXCOORD"
	};

	const unsigned elementSemanticIndices[] =
	{
		0,
		0,
		0,
		0,
		1,
		0,
		1,
		0,
		0,
		0,
		2,
		3,
		4
	};

	void D3D11ShaderProgram::ParseUniform()
	{
		{
			ID3D11ShaderReflection* reflection = 0;
			D3D11_SHADER_DESC shaderDesc;

			D3DReflect(&m_pByteVSCode[0], m_pByteVSCode.size(), IID_ID3D11ShaderReflection, (void**)&reflection);
			if (!reflection)
			{
				LogError("Failed to reflect vertex shader's input signature");
				return;
			}

			reflection->GetDesc(&shaderDesc);

			//if (type_ == VS)
			{
				for (unsigned i = 0; i < shaderDesc.InputParameters; ++i)
				{
					D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
					reflection->GetInputParameterDesc((UINT)i, &paramDesc);
					for (unsigned j = 0; j < 4/*MAX_VERTEX_ELEMENTS*/; ++j)
					{
						if ( string(paramDesc.SemanticName) == string(elementSemantics[j]) &&
							 paramDesc.SemanticIndex == elementSemanticIndices[j])
						{
							elementMask_ |= (1 << j);
							break;
						}
					}
				}
			}

			map<string, unsigned> cbRegisterMap;

			for (unsigned i = 0; i < shaderDesc.BoundResources; ++i)
			{
				D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
				reflection->GetResourceBindingDesc(i, &resourceDesc);
				string resourceName(resourceDesc.Name);
				if (resourceDesc.Type == D3D_SIT_CBUFFER)
					cbRegisterMap[resourceName] = resourceDesc.BindPoint;
				else if (resourceDesc.Type == D3D_SIT_SAMPLER && resourceDesc.BindPoint < 16/*MAX_TEXTURE_UNITS*/)
					useTextureUnit_[resourceDesc.BindPoint] = true;
			}

			for (unsigned i = 0; i < shaderDesc.ConstantBuffers; ++i)
			{
				ID3D11ShaderReflectionConstantBuffer* cb = reflection->GetConstantBufferByIndex(i);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				cb->GetDesc(&cbDesc);
				unsigned cbRegister = cbRegisterMap[string(cbDesc.Name)];

				for (unsigned j = 0; j < cbDesc.Variables; ++j)
				{
					ID3D11ShaderReflectionVariable* var = cb->GetVariableByIndex(j);
					D3D11_SHADER_VARIABLE_DESC varDesc;
					var->GetDesc(&varDesc);

					parametersVS_[varDesc.Name] = ShaderParameter(VS, varDesc.Name, cbRegister, varDesc.StartOffset, varDesc.Size);
				}
			}

			reflection->Release();

			CalculateConstantBufferSizesVS();
		}

		{
			ID3D11ShaderReflection* reflection = 0;
			D3D11_SHADER_DESC shaderDesc;

			D3DReflect(&m_pBytePSCode[0], m_pBytePSCode.size(), IID_ID3D11ShaderReflection, (void**)&reflection);
			if (!reflection)
			{
				LogError("Failed to reflect vertex shader's input signature");
				return;
			}

			reflection->GetDesc(&shaderDesc);

// 			//if (type_ == VS)
// 			{
// 				for (unsigned i = 0; i < shaderDesc.InputParameters; ++i)
// 				{
// 					D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
// 					reflection->GetInputParameterDesc((UINT)i, &paramDesc);
// 					for (unsigned j = 0; j < 4/*MAX_VERTEX_ELEMENTS*/; ++j)
// 					{
// 						if ( string(paramDesc.SemanticName) == string(elementSemantics[j]) &&
// 							paramDesc.SemanticIndex == elementSemanticIndices[j])
// 						{
// 							elementMask_ |= (1 << j);
// 							break;
// 						}
// 					}
// 				}
// 			}

			map<string, unsigned> cbRegisterMap;

			for (unsigned i = 0; i < shaderDesc.BoundResources; ++i)
			{
				D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
				reflection->GetResourceBindingDesc(i, &resourceDesc);
				string resourceName(resourceDesc.Name);
				if (resourceDesc.Type == D3D_SIT_CBUFFER)
					cbRegisterMap[resourceName] = resourceDesc.BindPoint;
				else if (resourceDesc.Type == D3D_SIT_SAMPLER && resourceDesc.BindPoint < 16/*MAX_TEXTURE_UNITS*/)
					useTextureUnit_[resourceDesc.BindPoint] = true;
			}

			for (unsigned i = 0; i < shaderDesc.ConstantBuffers; ++i)
			{
				ID3D11ShaderReflectionConstantBuffer* cb = reflection->GetConstantBufferByIndex(i);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				cb->GetDesc(&cbDesc);
				unsigned cbRegister = cbRegisterMap[string(cbDesc.Name)];

				for (unsigned j = 0; j < cbDesc.Variables; ++j)
				{
					ID3D11ShaderReflectionVariable* var = cb->GetVariableByIndex(j);
					D3D11_SHADER_VARIABLE_DESC varDesc;
					var->GetDesc(&varDesc);

					parametersPS_[varDesc.Name] = ShaderParameter(VS, varDesc.Name, cbRegister, varDesc.StartOffset, varDesc.Size);
				}
			}

			reflection->Release();

			CalculateConstantBufferSizesPS();
		}

	}

	void D3D11ShaderProgram::CalculateConstantBufferSizesVS()
	{
		for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
			constantBufferSizesVS_[i] = 0;

		for (map<string, ShaderParameter>::const_iterator i = parametersVS_.begin(); i != parametersVS_.end(); ++i)
		{
			if (i->second.buffer_ < MAX_SHADER_PARAMETER_GROUPS)
			{
				unsigned oldSize = constantBufferSizesVS_[i->second.buffer_];
				unsigned paramEnd = i->second.offset_ + i->second.size_;
				if (paramEnd > oldSize)
					constantBufferSizesVS_[i->second.buffer_] = paramEnd;
			}
		}
	}

	void D3D11ShaderProgram::CalculateConstantBufferSizesPS()
	{
		for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
			constantBufferSizesPS_[i] = 0;

		for (map<string, ShaderParameter>::const_iterator i = parametersPS_.begin(); i != parametersPS_.end(); ++i)
		{
			if (i->second.buffer_ < MAX_SHADER_PARAMETER_GROUPS)
			{
				unsigned oldSize = constantBufferSizesPS_[i->second.buffer_];
				unsigned paramEnd = i->second.offset_ + i->second.size_;
				if (paramEnd > oldSize)
					constantBufferSizesPS_[i->second.buffer_] = paramEnd;
			}
		}
	}

	void D3D11ShaderProgram::RT_SetShader()
	{
		D3D11RenderDevice* pD3D11Device = (D3D11RenderDevice*)GetRenderDevice();
		pD3D11Device->SetShaderProgram(this);
		
		GetD3D11DxDeviveContext()->VSSetShader(m_pVertexShader,NULL,0);

		GetD3D11DxDeviveContext()->PSSetShader(m_pPiexelShader,NULL,0);
	}

}

