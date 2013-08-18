#include "D3D9ShaderProgram.h"



namespace ma
{
	D3D9ShaderProgram::D3D9ShaderProgram() : m_pVertexShader(0),m_pPiexelShader(0),
		m_pVShConstantTable(0),m_pPShConstantTable(0)
	{
	}

	D3D9ShaderProgram::~D3D9ShaderProgram()
	{
		SAFE_RELEASE(m_pVertexShader);
		SAFE_RELEASE(m_pPiexelShader);
	}


	void D3D9ShaderProgram::CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize)
	{
		ASSERT(vshSource);
		ASSERT(fshSource);
		if (vshSource == NULL || fshSource == NULL)
		 	return;


		DWORD dwShaderFlags = 0;
#ifdef _DEBUG
		dwShaderFlags |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif // _DEBUG


		std::string strDir = FileSystem::getResourcePath();
		strDir += "/shader/D3D9/Cach/";
		std::ofstream saveFile;
		std::string strVshName = strDir + m_name + ".vsh";
		std::string strFshName = strDir + m_name + ".fsh";
		saveFile.open(strVshName.c_str());
		saveFile << vshSource;
		saveFile.close();

		saveFile.open(strFshName.c_str());
		saveFile << fshSource;
		saveFile.close();

		HRESULT hr = D3D_OK;

	
		LPD3DXBUFFER pCode = NULL;
		LPD3DXBUFFER error = NULL;

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
		ASSERT(hr == D3D_OK);

		hr = GetD3D9DxDevive()->CreateVertexShader( 
			( DWORD* )pCode->GetBufferPointer(),
			&m_pVertexShader );
		ASSERT(hr == D3D_OK);


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
		ASSERT(hr == D3D_OK);
		


// 		HRESULT hr = D3D_OK;
// 
// 		DWORD dwShaderFlags = 0;
// #ifdef _DEBUG
// 		dwShaderFlags |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
// #endif // _DEBUG
// 
// 		LPD3DXBUFFER pCode = NULL;
// 		LPD3DXBUFFER error = NULL;
// 
// 		hr = D3DXCompileShader( 
// 			vshSource, 
// 			vshSize,
// 			NULL, 
// 			NULL,
// 			"main",
// 			"vs_3_0", 
// 			dwShaderFlags, 
// 			&pCode,
// 			&error, 
// 			&m_pVShConstantTable ) ;
// 		ASSERT(hr == D3D_OK);
// 		




// 		hr = D3DXCompileShader( 
// 			fshSource, 
// 			fshSize,
// 			NULL, 
// 			NULL,
// 			"main",
// 			"ps_3_0", 
// 			dwShaderFlags, 
// 			&pCode,
// 			&error, 
// 			&m_pPShConstantTable ) ;
// 		ASSERT(hr == D3D_OK);


		hr = GetD3D9DxDevive()->CreatePixelShader( 
			( DWORD* )pCode->GetBufferPointer(),
			&m_pPiexelShader );
		ASSERT(hr == D3D_OK);


		return;
	}

	void D3D9ShaderProgram::ParseUniform()
	{
		HRESULT hr = D3D_OK;

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

			Uniform* uniform = new Uniform();
			uniform->m_vshShder = true; 
			uniform->m_effect = this;
			uniform->m_name = constantDesc.Name;
			uniform->m_location = constantDesc.RegisterIndex;
			uniform->m_type = constantDesc.Type;
			uniform->m_nCount = constantDesc.RegisterCount;
			if (constantDesc.Type == D3DXPT_SAMPLER2D)
			{
				uniform->m_index = samplerIndex;
				samplerIndex += 1;
			}
			else
			{
				uniform->m_index = 0;
			}

			m_uniforms[constantDesc.Name] = uniform;
		}


		//D3DXCONSTANTTABLE_DESC conTabledesc;
		hr = m_pPShConstantTable->GetDesc(&conTabledesc);
		ASSERT(hr == D3D_OK);

		/*UINT */samplerIndex = 0;
		for (UINT i = 0; i < conTabledesc.Constants; ++i)
		{
			D3DXHANDLE handle = m_pPShConstantTable->GetConstant(NULL, i);

			D3DXCONSTANT_DESC constantDesc;
			UINT num = 1;

			hr = m_pPShConstantTable->GetConstantDesc(handle, &constantDesc, &num);
			ASSERT(hr == D3D_OK);

			Uniform* uniform = new Uniform();
			uniform->m_vshShder = false;
			uniform->m_effect = this;
			uniform->m_name = constantDesc.Name;
			uniform->m_location = constantDesc.RegisterIndex;
			uniform->m_type = constantDesc.Type;
			if (constantDesc.Type == D3DXPT_SAMPLER2D)
			{
				uniform->m_index = samplerIndex;
				samplerIndex += 1;
			}
			else
			{
				uniform->m_index = 0;
			}

			m_uniforms[constantDesc.Name] = uniform;
		}
	}


	void D3D9ShaderProgram::SetValue(Uniform* uniform, float value)
	{
		ASSERT(uniform);

		HRESULT hr = D3D_OK;
		if (uniform->m_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->m_location, &value, 1);
		else
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->m_location, &value, 1);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const float* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);

		HRESULT hr = D3D_OK;
		if (uniform->m_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->m_location, values, count);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->m_location, values, count);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, int value)
	{
		ASSERT(uniform);

		HRESULT hr = D3D_OK;
		if (uniform->m_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantI(uniform->m_location, &value, 1);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantI(uniform->m_location, &value, 1);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const int* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);

		HRESULT hr = D3D_OK;
		if (uniform->m_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantI(uniform->m_location, values, count);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantI(uniform->m_location, values, count);
			ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Matrix4x4& value)
	{
		ASSERT(uniform);

		HRESULT hr = D3D_OK;
		if (uniform->m_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->m_location, (const float*)&value, uniform->m_nCount);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->m_location, (const float*)&value, uniform->m_nCount);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Matrix4x4* values, unsigned int count)
	{
		ASSERT(uniform);
		//ASSERT(values);
		if (values == NULL)
			return;
		
		HRESULT hr = D3D_OK;
		if (uniform->m_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->m_location, (const float*)values, uniform->m_nCount);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->m_location, (const float*)values, uniform->m_nCount);
		ASSERT(hr == D3D_OK);
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Vector2& value)
	{
		ASSERT(uniform);

		HRESULT hr = D3D_OK;
		if (uniform->m_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->m_location, (const float*)&value, 1);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->m_location, (const float*)&value, 1);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Vector2* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);

		HRESULT hr = D3D_OK;
		if (uniform->m_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->m_location, (const float*)values, count);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->m_location, (const float*)values, count);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Vector3& value)
	{
		ASSERT(uniform);

		HRESULT hr = D3D_OK;
		if (uniform->m_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->m_location, (const float*)&value, 1);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->m_location, (const float*)&value, 1);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Vector3* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);
		
		HRESULT hr = D3D_OK;
		if (uniform->m_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->m_location, (const float*)values, count);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->m_location, (const float*)values, count);
		ASSERT(hr == D3D_OK);
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Vector4& value)
	{
		ASSERT(uniform);

		HRESULT hr = D3D_OK;
		if (uniform->m_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->m_location, (const float*)&value, 1);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->m_location, (const float*)&value, 1);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Vector4* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);

		HRESULT hr = D3D_OK;
		if (uniform->m_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->m_location, (const float*)values, count);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->m_location, (const float*)values, count);
		ASSERT(hr == D3D_OK);
	
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Sampler* sampler)
	{
		ASSERT(uniform);
		//ASSERT(uniform->m_type == GL_SAMPLER_2D);
		ASSERT(sampler);


		D3D9Texture* pTexture = (D3D9Texture*)sampler->getTexture();
		ASSERT(pTexture);
		if (pTexture == NULL)
			return;

		GetD3D9DxDevive()->SetTexture(uniform->m_index, pTexture->GetD3DTexture());
		
		DWORD minFilter = 0,magFilter = 0,mipFilter = 0;
		D3D9Mapping::GetD3D9Filter(sampler->m_eFilter,minFilter,magFilter,mipFilter);
		DWORD wrapS = D3D9Mapping::GetD3D9Wrap(sampler->m_eWrap);
		DWORD wrapT = D3D9Mapping::GetD3D9Wrap(sampler->m_eWrap);

		//filter mode
		GetD3D9DxDevive()->SetSamplerState(uniform->m_index, D3DSAMP_MAGFILTER, magFilter);
		GetD3D9DxDevive()->SetSamplerState(uniform->m_index, D3DSAMP_MINFILTER, minFilter);
		GetD3D9DxDevive()->SetSamplerState(uniform->m_index, D3DSAMP_MIPFILTER, mipFilter);

		//address mode
		GetD3D9DxDevive()->SetSamplerState(uniform->m_index, D3DSAMP_ADDRESSU, wrapS);
		GetD3D9DxDevive()->SetSamplerState(uniform->m_index, D3DSAMP_ADDRESSV, wrapT);

	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Texture* sampler)
	{
		ASSERT(uniform);
		ASSERT(sampler);


		D3D9Texture* pTexture = (D3D9Texture*)sampler;
		ASSERT(pTexture);
		if (pTexture == NULL)
			return;

		GetD3D9DxDevive()->SetTexture(uniform->m_index, pTexture->GetD3DTexture());

		DWORD minFilter = 0,magFilter = 0,mipFilter = 0;
		D3D9Mapping::GetD3D9Filter(Sampler::TFO_BILINEAR,minFilter,magFilter,mipFilter);
		DWORD wrapS = D3D9Mapping::GetD3D9Wrap(Sampler::CLAMP);
		DWORD wrapT = D3D9Mapping::GetD3D9Wrap(Sampler::CLAMP);

		//filter mode
		GetD3D9DxDevive()->SetSamplerState(uniform->m_index, D3DSAMP_MAGFILTER, magFilter);
		GetD3D9DxDevive()->SetSamplerState(uniform->m_index, D3DSAMP_MINFILTER, minFilter);
		GetD3D9DxDevive()->SetSamplerState(uniform->m_index, D3DSAMP_MIPFILTER, mipFilter);

		//address mode
		GetD3D9DxDevive()->SetSamplerState(uniform->m_index, D3DSAMP_ADDRESSU, wrapS);
		GetD3D9DxDevive()->SetSamplerState(uniform->m_index, D3DSAMP_ADDRESSV, wrapT);
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Texture** values, UINT count)
	{

	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Sampler** values, unsigned int count)
	{
	     ASSERT(uniform);
	     //ASSERT(uniform->m_type == GL_SAMPLER_2D);
	     ASSERT(values);
	
	}

	void D3D9ShaderProgram::Bind()
	{

		GetD3D9DxDevive()->SetVertexShader(m_pVertexShader);

		GetD3D9DxDevive()->SetPixelShader(m_pPiexelShader);

	}

}
