#include "D3D11ShaderProgram.h"
#include "D3D11ConstantBuffer.h"

namespace ma
{

	//vector<ConstantBuffer*> g_vecDirtyConstantBuffers;
	ID3D11Buffer* g_vecD3D11ConstantBuffers[2][MAX_SHADER_PARAMETER_GROUPS];

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


	void D3D11ShaderProgram::LoadByteCodeFromCache(const char* pszFile, vector<BYTE>& byteCode)
	{
		Stream*	pVSFile = GetArchiveMananger()->Open(pszFile);
		if (pVSFile)
		{
			m_pByteVSCode.resize(pVSFile->GetSize());
			UINT uReadSize = pVSFile->Read(&m_pByteVSCode[0], pVSFile->GetSize());
		}
	}

	void D3D11ShaderProgram::SaveByteCodeToCache(const char* pszFile, vector<BYTE>& byteCode)
	{

	}

	void D3D11ShaderProgram::CompleVSFile()
	{
		DWORD dwShaderFlags = 0;
#ifdef _DEBUG
		dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif
		ID3D10Blob* pCode = NULL;
		ID3D10Blob* error = NULL;

		std::string strVshSource = PrePareShaderSource(strPathVS.c_str(), GetShaderMacro());
	
		HRESULT hr = D3DCompile(
			strVshSource.c_str(),
			strVshSource.size(),
			NULL,
			NULL,
			NULL,
			"main",
			"vs_4_0",
			dwShaderFlags,
			dwShaderFlags,
			&pCode,
			&error);
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
	}

	void D3D11ShaderProgram::CompleFSFile()
	{		
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
			&error);
		if (hr != S_OK)
		{
			const char* pErrCode = (const char*)error->GetBufferPointer();
			LogError(pErrCode);
			ASSERT(false);
		}

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
	}

	void D3D11ShaderProgram::LoadByteCode()
	{
		std::string strPath = GetRenderSystem()->GetShaderPath();

		std::string strPathVS = strPath + GetVSFile() + ".vert";
		std::string strPathFS = strPath + GetPSFile() + ".frag";

		LoadByteCodeFromCache(strPathVS.c_str(), m_pByteVSCode);
		LoadByteCodeFromCache(strPathFS.c_str(), m_pBytePSCode);

		if (m_pByteVSCode.empty())
		{
			CompleVSFile();

			SaveByteCodeToCache(strPathVS.c_str(), m_pByteVSCode);
		}

		if (m_pBytePSCode.empty())
		{
			CompleFSFile();

			SaveByteCodeToCache(strPathFS.c_str(), m_pBytePSCode);
		}
	}

	void D3D11ShaderProgram::CreateFromByteCode()
	{
		Destory();

		HRESULT hr = S_OK;

		hr = GetD3D11DxDevive()->CreateVertexShader(&m_pByteVSCode[0], m_pByteVSCode.size(), NULL, &m_pVertexShader);
		ASSERT(hr == S_OK);
	
		hr = GetD3D11DxDevive()->CreatePixelShader(&m_pBytePSCode[0], m_pBytePSCode.size(), NULL, &m_pPiexelShader);
		ASSERT(hr == S_OK);

		return;
	}


	void D3D11ShaderProgram::ParseUniform()
	{
		ParseShaderUniform(VS,m_pByteVSCode,m_vecVSConstantBuffers);
		ParseShaderUniform(PS,m_pBytePSCode,m_vecPSConstantBuffers);
	}

	void D3D11ShaderProgram::ParseShaderUniform(ShaderType eType,const vector<BYTE>& vecByteCode,
		RefPtr<D3D11ConstantBuffer> ConstantBuffersPtr[])
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

			RefPtr<D3D11ConstantBuffer> pConstantBuffer = CreateConstantBuffer(eType, cbRegister, cbDesc.Size);
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

					pUniform->m_nOffSetInCB = varDesc.StartOffset;
					pUniform->m_nSizeInCB = varDesc.Size;
					pUniform->m_pCBPtr = pConstantBuffer.get();
				}
			}
		}

		reflection->Release();
	}

	void D3D11ShaderProgram::RT_Create()
	{
		LoadByteCode();

		CreateFromByteCode();

		ParseUniform();
	}

	void D3D11ShaderProgram::RT_SetShader()
	{
		GetD3D11DxDeviveContext()->VSSetShader(m_pVertexShader, NULL, 0);

		GetD3D11DxDeviveContext()->PSSetShader(m_pPiexelShader, NULL, 0);

		( (D3D11RenderDevice*)GetRenderDevice() )->SetVertexDeclaration(GetVertexDeclaration());

		bool vsBuffersChanged = false;
		bool psBuffersChanged = false;

		for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
		{
			ID3D11Buffer* vsBuffer = m_vecVSConstantBuffers[i] ? m_vecVSConstantBuffers[i]->GetD3D11Buffer() : NULL;
			if (vsBuffer != g_vecD3D11ConstantBuffers[VS][i])
			{
				g_vecD3D11ConstantBuffers[VS][i] = vsBuffer;
				vsBuffersChanged = true;
			}

			ID3D11Buffer* psBuffer = m_vecPSConstantBuffers[i] ? m_vecPSConstantBuffers[i]->GetD3D11Buffer() : NULL;
			if (psBuffer != g_vecD3D11ConstantBuffers[PS][i])
			{
				g_vecD3D11ConstantBuffers[PS][i] = psBuffer;
				psBuffersChanged = true;
			}
		}

		if (vsBuffersChanged)
		{
			GetD3D11DxDeviveContext()->VSSetConstantBuffers(0, MAX_SHADER_PARAMETER_GROUPS, &g_vecD3D11ConstantBuffers[VS][0]);
		}

		if (psBuffersChanged)
		{
			GetD3D11DxDeviveContext()->PSSetConstantBuffers(0, MAX_SHADER_PARAMETER_GROUPS, &g_vecD3D11ConstantBuffers[PS][0]);
		}
	}

	void D3D11ShaderProgram::CommitChanges()
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

