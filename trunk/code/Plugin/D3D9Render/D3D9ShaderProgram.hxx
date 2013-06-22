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

		HRESULT hr = D3D_OK;

		DWORD dwShaderFlags = 0;
#ifdef _DEBUG
		dwShaderFlags |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif // _DEBUG

		LPD3DXBUFFER pCode = NULL;
		LPD3DXBUFFER error = NULL;

		hr = D3DXCompileShader( 
			vshSource, 
			vshSize,
			NULL, 
			NULL,
			"main",
			"vs_2_0", 
			dwShaderFlags, 
			&pCode,
			&error, 
			&m_pVShConstantTable ) ;
		ASSERT(hr == D3D_OK);
		

		hr = GetD3D9DxDevive()->CreateVertexShader( 
			( DWORD* )pCode->GetBufferPointer(),
			&m_pVertexShader );
		ASSERT(hr == D3D_OK);


		hr = D3DXCompileShader( 
			fshSource, 
			fshSize,
			NULL, 
			NULL,
			"main",
			"ps_2_0", 
			dwShaderFlags, 
			&pCode,
			&error, 
			&m_pPShConstantTable ) ;
		ASSERT(hr == D3D_OK);


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
			uniform->_vshShder = true; 
			uniform->_effect = this;
			uniform->_name = constantDesc.Name;
			uniform->_location = constantDesc.RegisterIndex;
			uniform->_type = constantDesc.Type;
			if (constantDesc.Type == D3DXPT_SAMPLER2D)
			{
				uniform->_index = samplerIndex;
				samplerIndex += 1;
			}
			else
			{
				uniform->_index = 0;
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
			uniform->_vshShder = false;
			uniform->_effect = this;
			uniform->_name = constantDesc.Name;
			uniform->_location = constantDesc.RegisterIndex;
			uniform->_type = constantDesc.Type;
			if (constantDesc.Type == D3DXPT_SAMPLER2D)
			{
				uniform->_index = samplerIndex;
				samplerIndex += 1;
			}
			else
			{
				uniform->_index = 0;
			}

			m_uniforms[constantDesc.Name] = uniform;
		}
	}


	void D3D9ShaderProgram::SetValue(Uniform* uniform, float value)
	{
		ASSERT(uniform);

		HRESULT hr = D3D_OK;
		if (uniform->_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->_location, &value, 1);
		else
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->_location, &value, 1);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const float* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);

		HRESULT hr = D3D_OK;
		if (uniform->_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->_location, values, count);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->_location, values, count);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, int value)
	{
		ASSERT(uniform);

		HRESULT hr = D3D_OK;
		if (uniform->_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantI(uniform->_location, &value, 1);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantI(uniform->_location, &value, 1);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const int* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);

		HRESULT hr = D3D_OK;
		if (uniform->_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantI(uniform->_location, values, count);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantI(uniform->_location, values, count);
			ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Matrix4x4& value)
	{
		ASSERT(uniform);

		HRESULT hr = D3D_OK;
		if (uniform->_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->_location, (const float*)&value, 4);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->_location, (const float*)&value, 4);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Matrix4x4* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);
		
		HRESULT hr = D3D_OK;
		if (uniform->_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->_location, (const float*)values, 4* count);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->_location, (const float*)values, 4 * count);
		ASSERT(hr == D3D_OK);
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Vector2& value)
	{
		ASSERT(uniform);

		HRESULT hr = D3D_OK;
		if (uniform->_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->_location, (const float*)&value, 2);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->_location, (const float*)&value, 2);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Vector2* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);

		HRESULT hr = D3D_OK;
		if (uniform->_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->_location, (const float*)values, 2 * count);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->_location, (const float*)values, 2 * count);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Vector3& value)
	{
		ASSERT(uniform);

		HRESULT hr = D3D_OK;
		if (uniform->_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->_location, (const float*)&value, 3);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->_location, (const float*)&value, 3);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Vector3* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);
		
		HRESULT hr = D3D_OK;
		if (uniform->_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->_location, (const float*)values, 3 * count);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->_location, (const float*)values, 3 * count);
		ASSERT(hr == D3D_OK);
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Vector4& value)
	{
		ASSERT(uniform);

		HRESULT hr = D3D_OK;
		if (uniform->_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->_location, (const float*)&value, 4);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->_location, (const float*)&value, 4);
		ASSERT(hr == D3D_OK);
		
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Vector4* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);

		HRESULT hr = D3D_OK;
		if (uniform->_vshShder)
			hr = GetD3D9DxDevive()->SetVertexShaderConstantF(uniform->_location, (const float*)values, 4 * count);
		else 
			hr = GetD3D9DxDevive()->SetPixelShaderConstantF(uniform->_location, (const float*)values, 4 * count);
		ASSERT(hr == D3D_OK);
	
	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Sampler* sampler)
	{
		ASSERT(uniform);
		//ASSERT(uniform->_type == GL_SAMPLER_2D);
		ASSERT(sampler);


		D3D9Texture* pTexture = (D3D9Texture*)sampler->getTexture();
		ASSERT(pTexture);
		if (pTexture == NULL)
			return;

		GetD3D9DxDevive()->SetTexture(uniform->_index, pTexture->GetD3DTexture());
		
		DWORD minFilter = 0,magFilter = 0,mipFilter = 0;
		D3D9Mapping::GetD3D9Filter(sampler->_filter,minFilter,magFilter,mipFilter);
		DWORD wrapS = D3D9Mapping::GetD3D9Wrap(sampler->_wrapS);
		DWORD wrapT = D3D9Mapping::GetD3D9Wrap(sampler->_wrapT);

		//filter mode
		GetD3D9DxDevive()->SetSamplerState(uniform->_index, D3DSAMP_MAGFILTER, magFilter);
		GetD3D9DxDevive()->SetSamplerState(uniform->_index, D3DSAMP_MINFILTER, minFilter);
		GetD3D9DxDevive()->SetSamplerState(uniform->_index, D3DSAMP_MIPFILTER, mipFilter);

		//address mode
		GetD3D9DxDevive()->SetSamplerState(uniform->_index, D3DSAMP_ADDRESSU, wrapS);
		GetD3D9DxDevive()->SetSamplerState(uniform->_index, D3DSAMP_ADDRESSV, wrapT);

	}

	void D3D9ShaderProgram::SetValue(Uniform* uniform, const Sampler** values, unsigned int count)
	{
	     ASSERT(uniform);
	     //ASSERT(uniform->_type == GL_SAMPLER_2D);
	     ASSERT(values);
	 

	}

	void D3D9ShaderProgram::Bind()
	{
	   //GL_ASSERT( glUseProgram(m_program) );

		//__currentEffect = this;

		GetD3D9DxDevive()->SetVertexShader(m_pVertexShader);

		GetD3D9DxDevive()->SetPixelShader(m_pPiexelShader);
	}

	void D3D9ShaderProgram::BindSampler(const Sampler* pSampler)
	{
		// Bind the sampler - this binds the texture and applies sampler state
		//const_cast<Sampler*>(sampler)->Bind();
		ASSERT(pSampler);
		if (pSampler == NULL)
			return;



	}


}
