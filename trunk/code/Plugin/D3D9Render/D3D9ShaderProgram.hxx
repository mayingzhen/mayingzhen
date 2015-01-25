#include "D3D9ShaderProgram.h"



namespace ma
{
	D3D9ShaderProgram::D3D9ShaderProgram()
		:ShaderProgram(NULL)
	{
		m_pVertexShader = NULL;
		m_pPiexelShader = NULL;
		m_pVertexShader = NULL;
		m_pVShConstantTable = NULL;
		m_pPShConstantTable = NULL;
	}

	D3D9ShaderProgram::~D3D9ShaderProgram()
	{
		Destory();
	}
	
	void D3D9ShaderProgram::Destory()
	{
		SAFE_RELEASE(m_pVShConstantTable);
		SAFE_RELEASE(m_pPShConstantTable);

		SAFE_RELEASE(m_pVertexShader);
		SAFE_RELEASE(m_pPiexelShader);
	}

	void D3D9ShaderProgram::CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize)
	{
		Destory();

		ASSERT(vshSource);
		ASSERT(fshSource);
		if (vshSource == NULL || fshSource == NULL)
		 	return;

		HRESULT hr = D3D_OK;
		LPD3DXBUFFER pCode = NULL;
		LPD3DXBUFFER error = NULL;


		DWORD dwShaderFlags = 0;
#ifdef _DEBUG
		dwShaderFlags |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;

		char pszPath[MAX_PATH] = {0};
		GetFullPathName("ShaderCach/D3D9/",MAX_PATH,pszPath,NULL);
		std::string strDir = pszPath;
		std::string strVshName = strDir + string(GetVSFile()) + "_" + string(GetShaderDefine()) + ".vsh";
		std::string strFshName = strDir + string(GetPSFile()) + "_" + string(GetShaderDefine()) + ".fsh";
		
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
			hr = D3DXCompileShaderFromFile( 
				strVshName.c_str(),
				NULL, 
				NULL,
				"main",
				"vs_3_0", 
				dwShaderFlags, 
				&pCode,
				&error, 
				&m_pVShConstantTable ) ;
#else
			hr = D3DXCompileShader( 
				vshSource, 
				vshSize,
				NULL, 
				NULL,
				"main",
				"vs_3_0", 
				dwShaderFlags, 
				&pCode,
				&error, 
				&m_pVShConstantTable ) ;
#endif
			if (hr != D3D_OK)
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
				hr = GetD3D9DxDevive()->CreateVertexShader( (DWORD*)pCode->GetBufferPointer(),&m_pVertexShader );
				ASSERT(hr == D3D_OK);
			}
		}


		if (fshSize > 0)
		{
#if _DEBUG
			hr = D3DXCompileShaderFromFile( 
				strFshName.c_str(), 
				NULL, 
				NULL,
				"main",
				"ps_3_0", 
				dwShaderFlags, 
				&pCode,
				&error, 
				&m_pPShConstantTable ) ;
#else
			hr = D3DXCompileShader( 
				fshSource, 
				fshSize,
				NULL, 
				NULL,
				"main",
				"ps_3_0", 
				dwShaderFlags, 
				&pCode,
				&error, 
				&m_pPShConstantTable ) ;
#endif
			if (hr != D3D_OK)
			{
				const char* pErrCode = (const char*)error->GetBufferPointer();	
				LogError(pErrCode);
				ASSERT(false);
			}
			else
			{
				hr = GetD3D9DxDevive()->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(), &m_pPiexelShader );
				ASSERT(hr == D3D_OK);
			}
		}

		return;
	}

	void D3D9ShaderProgram::ParseUniform()
	{
		HRESULT hr = D3D_OK;

		if (m_pVShConstantTable)
		{
			D3DXCONSTANTTABLE_DESC conTabledesc;
			hr = m_pVShConstantTable->GetDesc(&conTabledesc);
			ASSERT(hr == D3D_OK);

			UINT samplerIndex = 0;
			for (UINT i = 0; i < conTabledesc.Constants; ++i)
			{
				D3DXHANDLE handle = m_pVShConstantTable->GetConstant(NULL, i);

				D3DXCONSTANT_DESC constantDesc;
				UINT num = 1;

				hr = m_pVShConstantTable->GetConstantDesc(handle, &constantDesc, &num);
				ASSERT(hr == D3D_OK);

				Uniform* uniform = this->AddUniform(constantDesc.Name);
				uniform->m_vshShder = true; 
				uniform->m_location = constantDesc.RegisterIndex;
				uniform->m_type = constantDesc.Type;
				uniform->m_nCount = constantDesc.RegisterCount;
			}
		}

		if (m_pPShConstantTable)
		{
			D3DXCONSTANTTABLE_DESC conTabledesc;
			hr = m_pPShConstantTable->GetDesc(&conTabledesc);
			ASSERT(hr == D3D_OK);

			for (UINT i = 0; i < conTabledesc.Constants; ++i)
			{
				D3DXHANDLE handle = m_pPShConstantTable->GetConstant(NULL, i);

				D3DXCONSTANT_DESC constantDesc;
				UINT num = 1;

				hr = m_pPShConstantTable->GetConstantDesc(handle, &constantDesc, &num);
				ASSERT(hr == D3D_OK);

				RefPtr<Uniform> uniform = this->AddUniform(constantDesc.Name);
				uniform->m_vshShder = false;
				uniform->m_location = constantDesc.RegisterIndex;
				uniform->m_type = constantDesc.Type;
				uniform->m_nCount = constantDesc.RegisterCount;
			}
		}

	}

	void D3D9ShaderProgram::RT_SetShader()
	{
		HRESULT hr = D3D_OK;

		hr = GetD3D9DxDevive()->SetVertexShader(m_pVertexShader);
		ASSERT(hr == D3D_OK);

		hr = GetD3D9DxDevive()->SetPixelShader(m_pPiexelShader);
		ASSERT(hr == D3D_OK);
	}

}

