#include "D3D9RenderDevice.h"
//#include "D3D9RendMesh.h"
#include "D3D9Texture.h"


namespace ma
{

	LPDIRECT3DDEVICE9 GetD3D9DxDevive()
	{
		D3D9RenderDevice* pRender = (D3D9RenderDevice*)GetRenderDevice();
		ASSERT(pRender);
		return pRender->GetDXDevive();
	}

	D3D9RenderDevice::D3D9RenderDevice()
	{
		m_bZEnable = true;
		ClearAllStates();
	}

	D3D9RenderDevice::~D3D9RenderDevice()
	{
	}

	void D3D9RenderDevice::ClearAllStates()
	{
		for (int i = 0; i < MAX_CONSTANTS_VS; ++i)
		{
			m_CurVSParams[i].x = -99999.9f;
			m_CurVSParams[i].y = -99999.9f;
			m_CurVSParams[i].z = -99999.9f;
			m_CurVSParams[i].w = -99999.9f;
			m_VSParamsToCommit[i] = 0;
		}

		for (int i=0; i<MAX_CONSTANTS_PS; ++i)
		{
			m_CurPSParams[i].x = -99999.9f;
			m_CurPSParams[i].y = -99999.9f;
			m_CurPSParams[i].z = -99999.9f;
			m_CurPSParams[i].w = -99999.9f;
			m_PSParamsToCommit[i] = 0;
		}

		m_NumVSParamsToCommit = 0;
		m_NumPSParamsToCommit = 0;
	}



	Texture* D3D9RenderDevice::CreateTexture(const char* pszPath)
	{
		return new D3D9Texture(pszPath);
	}

	Texture* D3D9RenderDevice::CreateTexture(int nWidth,int nHeight,PixelFormat format,USAGE eUsage)
	{
		return new D3D9Texture(nWidth,nHeight,format,eUsage);
	}

	VertexDeclaration* D3D9RenderDevice::CreateVertexDeclaration()
	{
		return new D3D9VertexDeclaration(m_pD3DDevice);
	}

	VertexBuffer*	D3D9RenderDevice::CreateVertexBuffer()
	{
		return new D3D9VertexBuffer();
	}

	IndexBuffer*	D3D9RenderDevice::CreateIndexBuffer()
	{
		return new D3D9IndexBuffer();
	}

	ShaderProgram*	D3D9RenderDevice::CreateShaderProgram()
	{
		return new D3D9ShaderProgram();
	}

	void D3D9RenderDevice::ConvertUV(float& fTop,float& fLeft,float& fRight,float& fBottom)
	{
		return;
	}

	float D3D9RenderDevice::GetHalfPixelOffset(float fHalfPiexl)
	{
		return fHalfPiexl;
	}

	void D3D9RenderDevice::ShutDown()
	{
		m_hWnd = NULL;

		SAFE_RELEASE(m_pD3DDevice);
		SAFE_RELEASE(m_pD3D9);
	}

	void D3D9RenderDevice::Init(HWND wndhandle)
	{
		m_hWnd = wndhandle;

		m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		if(NULL == m_pD3D9)
		{
			ASSERT(FALSE && "Direct3DCreate9(D3D_SDK_VERSION)");
			return ;
		}

		RECT rect;
		GetClientRect(m_hWnd,&rect);

		ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
		m_d3dpp.Windowed = TRUE;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		m_d3dpp.EnableAutoDepthStencil = TRUE;
		m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Disable vertical synchronization
		m_d3dpp.BackBufferWidth = rect.right - rect.left;
		m_d3dpp.BackBufferHeight = rect.bottom - rect.top;

		m_nAdapterToUse = D3DADAPTER_DEFAULT;
		D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;

#if SHIPPING_VERSION
		// When building a shipping version, disable NVPerfHUD (opt-out)
#else
		// Look for 'NVIDIA NVPerfHUD' adapter
		// If it is present, override default settings
		for (UINT Adapter = 0; Adapter < m_pD3D9->GetAdapterCount(); Adapter++)
		{
			D3DADAPTER_IDENTIFIER9 Identifier;
			HRESULT Res;
			Res = m_pD3D9->GetAdapterIdentifier(Adapter, 0, &Identifier);
			if ( strcmp(Identifier.Description, "NVIDIA NVPerfHUD") == 0 )
			{
				m_nAdapterToUse = Adapter;
				DeviceType = D3DDEVTYPE_REF;
				//Logger::info(String("Using ") + Identifier.Description + " adapter for debug purposes.");
				break;
			}
		}
#endif

		HRESULT hr = S_OK;
		hr = m_pD3D9->CreateDevice(m_nAdapterToUse,DeviceType,m_hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&m_d3dpp,&m_pD3DDevice);

		if( FAILED(hr) )
		{
			ASSERT(FALSE && "m_pD3D->CreateDevice()");
			return;
		}

		BuildDeviceCapabilities();
	}
	
	bool D3D9RenderDevice::TestDeviceLost()
	{
		HRESULT hr = m_pD3DDevice->TestCooperativeLevel();
		if (hr == D3DERR_DEVICELOST)
		{
			return true;
		}

		if (hr == D3DERR_DEVICENOTRESET)
		{
			Rest();
			hr = m_pD3DDevice->TestCooperativeLevel();
		}

		return FAILED(hr);
	}

	bool D3D9RenderDevice::Rest()
	{
		// reset resources
		for (VEC_D3D9RESOURCE::iterator iter = m_vecD3D9Resource.begin();iter != m_vecD3D9Resource.end();++iter)
		{
			(*iter)->NotifyOnDeviceLost();
		}

		if(FAILED(m_pD3DDevice->Reset(&m_d3dpp)))
		{
			ASSERT(false && "failed to reset a D3D device");
			return false;
		}

		// reset resources
		for (VEC_D3D9RESOURCE::iterator iter = m_vecD3D9Resource.begin();iter != m_vecD3D9Resource.end();++iter)
		{
			(*iter)->NotifyOnDeviceReset();
		}

		return true;
	}

	void D3D9RenderDevice::BeginRender()
	{
  		HRESULT hr = D3D_OK;	
		hr = m_pD3DDevice->BeginScene();
		ASSERT( SUCCEEDED(hr) );
	}

	void D3D9RenderDevice::EndRender()
	{
		HRESULT hr = D3D_OK;
		hr = m_pD3DDevice->EndScene();
		ASSERT( SUCCEEDED(hr) );

		hr = m_pD3DDevice->Present(NULL,NULL,NULL,NULL);
		ASSERT( SUCCEEDED(hr) );
	}

	void D3D9RenderDevice::SetRenderTarget(Texture* pTexture,int index)
	{
		D3D9Texture* pD3D9Target = pTexture ? static_cast<D3D9Texture*>(pTexture) : NULL;
		IDirect3DSurface9* target = pD3D9Target ? pD3D9Target->GetD3DSurface() : NULL;

		HRESULT hr = D3D_OK;
		hr = m_pD3DDevice->SetRenderTarget(index, target);
		ASSERT(hr == D3D_OK, "set render target failed.");
	}

	Texture* D3D9RenderDevice::GetRenderTarget(int index)
	{
		HRESULT hr = D3D_OK;

		LPDIRECT3DSURFACE9 surface = NULL;
		hr = m_pD3DDevice->GetRenderTarget(index,&surface);
		if (hr != D3D_OK)
			return NULL;

		D3D9Texture* pD3D9Target = new D3D9Texture(-1,-1);

		pD3D9Target->SetD3DSurface(surface);

		return pD3D9Target;
	}


	void D3D9RenderDevice::SetDepthStencil(Texture* pTexture)
	{
		D3D9Texture* pD3D9Target =  pTexture ? static_cast<D3D9Texture*>(pTexture) : NULL;
		IDirect3DSurface9* target = pD3D9Target ? pD3D9Target->GetD3DSurface() : NULL;

		HRESULT hr = D3D_OK;
		hr = m_pD3DDevice->SetDepthStencilSurface(target);
		ASSERT(hr == D3D_OK, "set render target failed.");
	}

	Texture* D3D9RenderDevice::GetDepthStencil()
	{
		HRESULT hr = D3D_OK;

		LPDIRECT3DSURFACE9 surface = NULL;
		hr = m_pD3DDevice->GetDepthStencilSurface(&surface);
		if (hr != D3D_OK)
			return NULL;

		D3D9Texture* pDepthStencil = new D3D9Texture(-1,-1);

		pDepthStencil->SetD3DSurface(surface);

		return pDepthStencil;
	}

	void D3D9RenderDevice::SetViewport(const Rectangle& rect)
	{
		D3DVIEWPORT9 vp;
		vp.X      = (DWORD)rect.x;
		vp.Y      = (DWORD)rect.y;
		vp.Width  = (DWORD)rect.width;
		vp.Height = (DWORD)rect.height;
		vp.MinZ   = 0.0f;
		vp.MaxZ   = 1.0f;

		m_pD3DDevice->SetViewport(&vp);
	}

	Rectangle D3D9RenderDevice::GetViewport()
	{
		Rectangle rect;

		D3DVIEWPORT9 vp;
		m_pD3DDevice->GetViewport(&vp);
		rect.x = (float)vp.X;
		rect.y = (float)vp.Y;
		rect.width = (float)vp.Width;
		rect.height = (float)vp.Height;

		return rect;
	}

	void D3D9RenderDevice::SetDepthBias(float constantBias, float slopeScaleBias/* = 0.0f*/)
	{

	}

	void D3D9RenderDevice::SetCullingMode(CULL_MODE mode)
	{
		if (mode== CULL_FACE_SIDE_BACK)
		{
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW) );
		}
		else if (mode == CULL_FACE_SIDE_FRONT)
		{
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW) );	
		}
		else if (mode == CULL_FACE_SIDE_NONE)
		{
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE) );
		}
	}

	void D3D9RenderDevice::SetBlendMode(BLEND_MODE mode)
	{
		switch (mode)
		{
		case BM_OPATICY:
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE) );
			break;

		case BM_TRANSPARENT:
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) );
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) );
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) );
			break;

		case BM_ADD:
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) );
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE) );
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE) );
			break;

		case BM_MULTIPLY:
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) );
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR) );
			D3D9Verify( m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO) );
			break;
		}
	}

	void D3D9RenderDevice::SetDepthWrite(bool b)
	{
		m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, b?TRUE:FALSE);
	}

	void D3D9RenderDevice::SetColorWrite(bool b)
	{
		uint32 val = 0;
		if (b/*red*/) 
			val |= D3DCOLORWRITEENABLE_RED;
		if (b/*green*/)
			val |= D3DCOLORWRITEENABLE_GREEN;
		if (b/*blue*/)
			val |= D3DCOLORWRITEENABLE_BLUE;
		if (b/*alpha*/)
			val |= D3DCOLORWRITEENABLE_ALPHA;

		m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, val); 
	}

	void D3D9RenderDevice::SetDepthCheckMode(DEPTH_CHECK_MODE mode)
	{
		switch (mode)
		{
		case DCM_NONE:
			m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
			m_bZEnable = false;
			break;

		case DCM_LESS_EQUAL:
			if (!m_bZEnable)
			{
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_bZEnable = true;
			}
			m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			break;

		case DCM_LESS:
			if (!m_bZEnable)
			{
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_bZEnable = true;
			}
			m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
			break;

		case DCM_GREATER_EQUAL:
			if (!m_bZEnable)
			{
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_bZEnable = true;
			}
			m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
			break;

		case DCM_GREATER:
			m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
			break;

		case DCM_EQUAL:
			if (!m_bZEnable)
			{
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_bZEnable = true;
			}
			m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
			break;

		case DCM_ALWAYS:
			if (!m_bZEnable)
			{
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_bZEnable = true;
			}
			m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			break;
		}
	}

	void D3D9RenderDevice::SetTexture(Uniform* uniform,Texture* pTexture)
	{
		D3D9Texture* pD3D9Texture = (D3D9Texture*)pTexture;

 		D3D9Verify( m_pD3DDevice->SetTexture(uniform->m_location, pD3D9Texture->GetD3DTexture()) );
	}

	void D3D9RenderDevice::SetTextureWrap(int index,Wrap eWrap)
	{
		DWORD wrapS = D3D9Mapping::GetD3D9Wrap(eWrap);
		DWORD wrapT = D3D9Mapping::GetD3D9Wrap(eWrap);

		//address mode
		D3D9Verify( m_pD3DDevice->SetSamplerState(index, D3DSAMP_ADDRESSU, wrapS) );
		D3D9Verify( m_pD3DDevice->SetSamplerState(index, D3DSAMP_ADDRESSV, wrapT) );
	}

	void D3D9RenderDevice::SetTextureFilter(int index,FilterOptions eFilter)
	{
		DWORD minFilter = 0,magFilter = 0,mipFilter = 0;
		D3D9Mapping::GetD3D9Filter(eFilter,minFilter,magFilter,mipFilter);

		//filter mode
		D3D9Verify( m_pD3DDevice->SetSamplerState(index, D3DSAMP_MAGFILTER, magFilter) );
		D3D9Verify( m_pD3DDevice->SetSamplerState(index, D3DSAMP_MINFILTER, minFilter) );
		D3D9Verify( m_pD3DDevice->SetSamplerState(index, D3DSAMP_MIPFILTER, mipFilter) );
	}

	void D3D9RenderDevice::mfSetPSConst(int nReg, const float *vData, const int nParams)
	{
#ifndef MERGE_SHADER_PARAMETERS
		D3D9Verify(m_pD3DDevice->SetPixelShaderConstantF(nReg,vData,nParams));
#else
		int i, nID;
		if ( (nID = m_NumPSParamsToCommit) + nParams > MAX_CONSTANTS_PS)
		{
			ASSERT(false);
			return;
		}

		const Vector4 *vSrc = (Vector4 *)vData;
		Vector4 *vDst = &m_CurPSParams[nReg];
		for (i=0; i<nParams; i++)
		{
			if (vSrc[i] != vDst[i])
			{
				memcpy(vDst, vSrc, sizeof(Vector4)*nParams);

				if (nID+nParams < MAX_CONSTANTS_PS)
				{
					for (i = 0; i < nParams; i++)
					{
						m_PSParamsToCommit[nID++] = i + nReg;
					}
					m_NumPSParamsToCommit = nID;
				}

				break;
			}
		}
#endif
	}

	void D3D9RenderDevice::mfSetVSConst(int nReg, const float *vData, const int nParams)
	{
#ifndef MERGE_SHADER_PARAMETERS
		D3D9Verify(m_pD3DDevice->SetVertexShaderConstantF(nReg,vData,nParams));
#else
		int i, nID;
		if ( (nID = m_NumVSParamsToCommit) + nParams > MAX_CONSTANTS_VS)
		{
			ASSERT(false);
			return;
		}

		const Vector4 *vSrc = (Vector4 *)vData;
		Vector4 *vDst = &m_CurVSParams[nReg];
		for (i = 0; i<nParams; i++)
		{
			assert(nReg + i >= 0 && nReg + i < MAX_CONSTANTS_VS);
			if (vSrc[i] != vDst[i])
			{
				memcpy(vDst, vSrc, sizeof(Vector4)*nParams);

				for (i = 0; i < nParams; i++)
				{
					m_VSParamsToCommit[nID++] = nReg+i;
				}
				m_NumVSParamsToCommit = nID;

				break;
			}
		}
#endif
	}
	
	void D3D9RenderDevice::CommitChanges()
	{
#ifndef MERGE_SHADER_PARAMETERS
		return;
#else	
		int i;
		if (m_NumPSParamsToCommit > 0)
		{
			int nFirst = m_PSParamsToCommit[0];
			int nParams = 1;
			assert(nFirst < MAX_CONSTANTS_PS);
			const int nCommitParms = m_NumPSParamsToCommit;
			for (i=1; i<nCommitParms; i++)
			{
				if (m_PSParamsToCommit[i] != m_PSParamsToCommit[i-1]+1)
				{
					D3D9Verify(m_pD3DDevice->SetPixelShaderConstantF(nFirst, &m_CurPSParams[nFirst].x, nParams));
					nFirst = m_PSParamsToCommit[i];
					nParams = 1;
				}
				else
				{
					nParams++;
				}
			}

			assert(nFirst < MAX_CONSTANTS_PS);
			D3D9Verify(m_pD3DDevice->SetPixelShaderConstantF(nFirst, &m_CurPSParams[nFirst].x, nParams));

			m_NumPSParamsToCommit = 0;
		}

		if (m_NumVSParamsToCommit > 0)
		{
			int nFirst = m_VSParamsToCommit[0];
			int nParams = 1;
			assert(nFirst < MAX_CONSTANTS_VS);
			const int nCommitParms = m_NumVSParamsToCommit;
			for (i=1; i<nCommitParms; i++)
			{
				if (m_VSParamsToCommit[i] != m_VSParamsToCommit[i-1]+1)
				{
					D3D9Verify(m_pD3DDevice->SetVertexShaderConstantF(nFirst, &m_CurVSParams[nFirst].x, nParams));
					nFirst = m_VSParamsToCommit[i];
					nParams = 1;
				}
				else
				{
					nParams++;
				}
			}

			assert(nFirst < MAX_CONSTANTS_VS);
			D3D9Verify(m_pD3DDevice->SetVertexShaderConstantF(nFirst, &m_CurVSParams[nFirst].x, nParams));

			m_NumVSParamsToCommit = 0;
		}
#endif
	}

	void D3D9RenderDevice::SetValue(Uniform* uniform, const float* values, UINT count)
	{
		ASSERT(uniform);
		ASSERT(values);

		UINT nCount = count > uniform->m_nCount ? uniform->m_nCount : count;

		if (uniform->m_vshShder)
		{
			mfSetVSConst(uniform->m_location, values, count);
		}
		else 
		{
			mfSetPSConst(uniform->m_location, values, count);
		}
	}

	void D3D9RenderDevice::SetValue(Uniform* uniform, float value)
	{
		SetValue(uniform,(const float*)&value,1);
	}

	void D3D9RenderDevice::SetValue(Uniform* uniform, const Matrix4& value)
	{
		SetValue(uniform,(const float*)&value,4);
	}

	void D3D9RenderDevice::SetValue(Uniform* uniform, const Vector2& value)
	{
		SetValue(uniform,(const float*)&value,1);
	}

	void D3D9RenderDevice::SetValue(Uniform* uniform, const Vector3& value)
	{
		SetValue(uniform,(const float*)&value,1);
	}

	void D3D9RenderDevice::SetValue(Uniform* uniform, const Vector4& value)
	{
		SetValue(uniform,(const float*)&value,1);
	}

	void D3D9RenderDevice::SetValue(Uniform* uniform, const Vector4* values, UINT count)
	{
		SetValue(uniform,(const float*)values,count);
	}

	void D3D9RenderDevice::SetValue(Uniform* uniform, const ColourValue& value)
	{
		SetValue(uniform,(const float*)&value,1);
	}

// 	void D3D9RenderDevice::SetRenderState(const RenderState& state)
// 	{
// 		//GetD3D9DxDevive()->SetRenderState(D3DRS_FILLMODE, state.fillMode);
// 
// 		//mD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, state.colorWrite);
// 
// 		if (m_curState.m_bDepthWrite != state.m_bDepthWrite)
// 		{
// 			m_curState.m_bDepthWrite = state.m_bDepthWrite;
// 			m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,state.m_bDepthWrite);
// 			
// 		}
// 
// 		if (m_curState.m_bColorWrite != state.m_bColorWrite)
// 		{
// 			m_curState.m_bColorWrite = state.m_bColorWrite;
// 			m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE,state.m_bColorWrite);
// 		}
// 
// 		if ( abs(m_curState.m_fDepthBias - state.m_fDepthBias) > 0.0001f )
// 		{
// 			m_curState.m_fDepthBias = state.m_fDepthBias;
// 			m_pD3DDevice->SetRenderState(D3DRS_DEPTHBIAS,*(DWORD*)&m_curState.m_fDepthBias);
// 		}
// 		
// 		if (m_curState.m_eDepthCheckMode != state.m_eDepthCheckMode)
// 		{
// 			m_curState.m_eDepthCheckMode = state.m_eDepthCheckMode;
// 			switch (state.m_eDepthCheckMode)
// 			{
// 			case DCM_NONE:
// 				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
// 				break;
// 
// 			case DCM_LESS_EQUAL:
// 				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
// 				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
// 				break;
// 
// 			case DCM_LESS:
// 				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
// 				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
// 				break;
// 
// 			case DCM_GREATER_EQUAL:
// 				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
// 				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
// 				break;
// 
// 			case DCM_GREATER:
// 				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
// 				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
// 				break;
// 
// 			case DCM_EQUAL:
// 				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
// 				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
// 				break;
// 
// 			case DCM_ALWAYS:
// 				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
// 				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
// 				break;
// 			}
// 		}
// 
// 
// 
// 	}

	void D3D9RenderDevice::SetVertexDeclaration(VertexDeclaration* pDec)
	{
		D3D9VertexDeclaration* d3dvd = (D3D9VertexDeclaration*)pDec;

		D3D9Verify( m_pD3DDevice->SetVertexDeclaration( d3dvd->GetD3DVertexDeclaration() ) ) ;
	}

	void D3D9RenderDevice::SetIndexBuffer(IndexBuffer* pIB)
	{
		D3D9IndexBuffer* pIndxBuffer = (D3D9IndexBuffer*)pIB;

		D3D9Verify( m_pD3DDevice->SetIndices(pIndxBuffer->GetD3DIndexBuffer() ) );
	}

	void D3D9RenderDevice::SetVertexBuffer(int index, VertexBuffer* pVB)
	{
		D3D9VertexBuffer* pVertexBuffer =(D3D9VertexBuffer*)pVB;
		IDirect3DVertexBuffer9* pD3D9VB = pVertexBuffer->GetD3DVertexBuffer();

		D3D9Verify( m_pD3DDevice->SetStreamSource(index, pD3D9VB, 0, pVertexBuffer->GetStride() ) );
	}

	void D3D9RenderDevice::DrawRenderable(const Renderable* pRenderable,Technique* pTech)
	{
		if (pRenderable == NULL)
			return;

		CommitChanges();	

		HRESULT hr = D3D_OK;

		D3DPRIMITIVETYPE ePrimitiveType = D3D9Mapping::GetD3DPrimitiveType(pRenderable->m_ePrimitiveType);

		const RefPtr<SubMeshData>& pSubMeshData = pRenderable->m_pSubMeshData;

		UINT nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : pRenderable->m_pIndexBuffer->GetNumber();
		UINT nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;
		
		UINT nVertexCount = pSubMeshData ? pSubMeshData->m_nVertexCount : pRenderable->m_pVertexBuffers->GetNumber();
		UINT nVertexStart = pSubMeshData ? pSubMeshData->m_nVertexStart : 0;

		UINT nPrimCount = 0;
		if (ePrimitiveType == D3DPT_TRIANGLELIST)
		{
			nPrimCount = nIndexCount / 3;
		}
		else if (ePrimitiveType == D3DPT_TRIANGLESTRIP)
		{
			nPrimCount = nIndexCount - 2;
		}
		else if (ePrimitiveType == D3DPT_LINELIST)
		{
			nPrimCount = nIndexCount / 2;
		}

		D3D9Verify( m_pD3DDevice->DrawIndexedPrimitive(ePrimitiveType,0,nVertexStart,nVertexCount,nIndexStart,nPrimCount) );
	}

	void D3D9RenderDevice::DrawDyRenderable(const Renderable* pRenderable,Technique* pTech)
	{
		if (pRenderable == NULL)
			return;

		CommitChanges();	

		D3DPRIMITIVETYPE ePrimitiveType = D3D9Mapping::GetD3DPrimitiveType(pRenderable->m_ePrimitiveType);

		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData.get();

		UINT nPrimCount = 0;
		if (ePrimitiveType == D3DPT_TRIANGLELIST)
		{
			nPrimCount = pSubMeshData->m_nIndexCount / 3;
		}
		else if (ePrimitiveType == D3DPT_TRIANGLESTRIP)
		{
			nPrimCount = pSubMeshData->m_nIndexCount - 2;
		}
		else if (ePrimitiveType == D3DPT_LINELIST)
		{
			nPrimCount = pSubMeshData->m_nIndexCount / 2;
		}

		D3D9Verify( m_pD3DDevice->DrawIndexedPrimitiveUP(ePrimitiveType,
			pSubMeshData->m_nVertexStart,
			pSubMeshData->m_nVertexCount,
			nPrimCount,
			pRenderable->m_pIndexBuffer->GetData(),
			D3D9Mapping::GetD3DIndexType(pRenderable->m_pIndexBuffer->GetIndexType()),
			pRenderable->m_pVertexBuffers->GetData(),
			pRenderable->m_pVertexBuffers->GetStride()) );
	}

	void D3D9RenderDevice::ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s)
	{
		HRESULT hr;

		DWORD Flags = 0;
		D3DCOLOR cVal = c.getAsARGB();
		float zVal = z;
		DWORD sVal = (DWORD)s;

		if (bColor)
			Flags |= D3DCLEAR_TARGET;
		if (bDepth)
			Flags |= D3DCLEAR_ZBUFFER;
		if (bStencil)
			Flags |= D3DCLEAR_STENCIL;

		hr = m_pD3DDevice->Clear(0, NULL, Flags, cVal, zVal, sVal);
		ASSERT(hr == D3D_OK && "Clear buffer failed.");
	}

	Matrix4 D3D9RenderDevice::MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf)
	{
		float yScale = Math::Tan(Math::HALF_PI - fovy*0.5f);
		float xScale = yScale/Aspect;
		float inv = 1.f/(zn - zf);

		out[0][0] = xScale; out[0][1] = 0.f;    out[0][2] = 0.f;    out[0][3] = 0.f;
		out[1][0] = 0.f;    out[1][1] = yScale; out[1][2] = 0.f;    out[1][3] = 0.f;
		out[2][0] = 0.f;    out[2][1] = 0.f;    out[2][2] = zf*inv; out[2][3] = zn*zf*inv;
		out[3][0] = 0.f;    out[3][1] = 0.f;    out[3][2] = -1;     out[3][3] = 0.f;

		 return out;
	}

	Matrix4 D3D9RenderDevice::MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf)
	{
		out[0][0] = 2/width;
		out[1][1] = 2/height;
		out[2][2] = 1/(zn-zf);
		out[3][3] = 1.f;
		out[2][3] = zn/(zn-zf);
		out[1][0] = out[2][0] = out[3][0] = 
			out[0][1] = out[0][2] = out[0][3] =
			out[1][2] = out[1][3] =
			out[2][1] = 
			out[3][1] = out[3][2] = 0;

		return out;
	}

	Matrix4 D3D9RenderDevice::MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf)
	{
		out[0][0] = 2/(right-left);
		out[1][1] = 2/(top-bottom);
		out[2][2] = 1/(zn-zf);
		out[3][3] = 1.f;
		out[2][3] = zn/(zn-zf);
		out[1][0] = out[2][0] = out[3][0] = 
			out[0][1] = out[0][2] = 
			out[1][2] = 
			out[2][1] = 
			out[3][1] = out[3][2] = 0;
		out[0][3] = (left+right)/(left-right);
		out[1][3] = (top+bottom)/(bottom-top);
		return out;
	}

	void D3D9RenderDevice::BeginProfile(const char* pszLale)
	{
		wchar_t buf[128]; 
		size_t outCount=0; 
		mbstowcs_s(&outCount, buf, pszLale, _countof(buf)-1); 
		D3DPERF_BeginEvent(0xff00ff00, buf); 
	}

	void D3D9RenderDevice::EndProfile()
	{
		D3DPERF_EndEvent();
	}

	bool D3D9RenderDevice::CheckTextureFormat(PixelFormat eFormat,USAGE eUsage)
	{
		DWORD D3DUsage =  D3D9Mapping::GetD3DUsage(eUsage); 
		D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(eFormat);

		HRESULT hr = D3DXCheckTextureRequirements(m_pD3DDevice, NULL, NULL, NULL, D3DUsage, &D3DFormat, D3DPOOL_DEFAULT);
		return hr == D3D_OK;
	}

	void D3D9RenderDevice::NotifyResourceCreated(D3D9Resource* pResource)
	{
		ASSERT(pResource != NULL);
		m_vecD3D9Resource.push_back(pResource);
	}

	void D3D9RenderDevice::NotifyResourceDestroyed(D3D9Resource* pResource)
	{
		ASSERT(pResource != NULL);
		VEC_D3D9RESOURCE::iterator iter = std::find(m_vecD3D9Resource.begin(), m_vecD3D9Resource.end(), pResource);
		if (iter == m_vecD3D9Resource.end())
		{
			ASSERT(false && "cannot find pResource in CD3D9ResourceManager::_notifyResourceDestroyed");
			return;
		}

		m_vecD3D9Resource.erase(iter);
	}

	bool D3D9RenderDevice::BuildDeviceCapabilities()
	{
		// Get current device caps.
		HRESULT hr = m_pD3DDevice->GetDeviceCaps(&m_d3dcaps);
		if( FAILED( hr ) )
		{
			LogError("Cannot get device caps!");
			return false;
		}

		D3DADAPTER_IDENTIFIER9 adapterIdentifier;
		D3DDISPLAYMODE d3ddm;
		D3DCAPS9 d3dcaps9 = m_d3dcaps;
		D3D9Verify(m_pD3D9->GetAdapterIdentifier( 0, 0, &adapterIdentifier ));
		D3D9Verify(m_pD3D9->GetAdapterDisplayMode( 0, &d3ddm ));

		// driver version
// 		DriverVersion mDriverVersion;
// 		mDriverVersion.major = HIWORD(adapterIdentifier.DriverVersion.HighPart);
// 		mDriverVersion.minor = LOWORD(adapterIdentifier.DriverVersion.HighPart);
// 		mDriverVersion.release = HIWORD(adapterIdentifier.DriverVersion.LowPart);
// 		mDriverVersion.build = LOWORD(adapterIdentifier.DriverVersion.LowPart);

		// driver description
// 		stringstream strTemp;
// 		strTemp << "Monitor-"  << "-" << adapterIdentifier.Description;
// 		string driverDescription(strTemp.str());
// 		StringUtil::trim(driverDescription);

		// build capabilities

		// determine vendor
		// Full list of vendors here: http://www.pcidatabase.com/vendors.php?sort=id
// 		switch(adapterIdentifier.VendorId)
// 		{
// 		case 0x10DE:
// 			GetDeviceCapabilities()->setVendor(GPU_NVIDIA);
// 			break;
// 		case 0x1002:
// 			GetDeviceCapabilities()->setVendor(GPU_ATI);
// 			break;
// 		case 0x163C:
// 		case 0x8086:
// 			GetDeviceCapabilities()->setVendor(GPU_INTEL);
// 			break;
// 		case 0x5333:
// 			GetDeviceCapabilities()->setVendor(GPU_S3);
// 			break;
// 		case 0x3D3D:
// 			GetDeviceCapabilities()->setVendor(GPU_3DLABS);
// 			break;
// 		case 0x102B:
// 			GetDeviceCapabilities()->setVendor(GPU_MATROX);
// 			break;
// 		case 0x1039:
// 			GetDeviceCapabilities()->setVendor(GPU_SIS);
// 			break;
// 		default:
// 			GetDeviceCapabilities()->setVendor(GPU_UNKNOWN);
// 			break;
// 		};

		//GetDeviceCapabilities()->setDriverVersion(mDriverVersion);
		//GetDeviceCapabilities()->setDeviceName(driverDescription.c_str());
		
// 		GetDeviceCapabilities()->setNumMultiRenderTargets(__min((uint8)d3dcaps9.NumSimultaneousRTs, (uint8)OGRE_MAX_MULTIPLE_RENDER_TARGETS));
// 		if(d3dcaps9.MaxPointSize < m_pDeviceCapabilities->getMaxPointSize())
// 			m_pDeviceCapabilities->setMaxPointSize(d3dcaps9.MaxPointSize);

		
		// Determine if any floating point texture format is supported
		D3DFORMAT floatFormats[6] = {D3DFMT_R16F, D3DFMT_G16R16F, 
			D3DFMT_A16B16G16R16F, D3DFMT_R32F, D3DFMT_G32R32F, 
			D3DFMT_A32B32G32R32F};
// 		IDirect3DSurface9* bbSurf;
// 		_pMainWindow->getCustomAttribute("DDBACKBUFFER", &bbSurf);
// 		D3DSURFACE_DESC bbSurfDesc;
// 		bbSurf->GetDesc(&bbSurfDesc);
		LPDIRECT3DSURFACE9 surface = NULL;
		hr = m_pD3DDevice->GetRenderTarget(0,&surface);
		if (hr != D3D_OK)
			return false;
		D3DSURFACE_DESC bbSurfDesc;
 		surface->GetDesc(&bbSurfDesc);

		for (int i = 0; i < 6; ++i)
		{
			if (SUCCEEDED(m_pD3D9->CheckDeviceFormat(D3DADAPTER_DEFAULT, 
				D3DDEVTYPE_HAL, bbSurfDesc.Format, 
				0, D3DRTYPE_TEXTURE, floatFormats[i])))
			{
				GetDeviceCapabilities()->setFloatTexturesSupported(true);
				break;
			}
		}

		if ((m_d3dcaps.PrimitiveMiscCaps & D3DPMISCCAPS_MRTINDEPENDENTBITDEPTHS) != 0)
		{
			GetDeviceCapabilities()->setMRTIndependentBitDepths(true);
		}
		else
		{
			GetDeviceCapabilities()->setMRTIndependentBitDepths(false);
		}

		// vs version && ps version
		if (d3dcaps9.VertexShaderVersion >= D3DVS_VERSION(3,0) && d3dcaps9.PixelShaderVersion >= D3DPS_VERSION(3,0))
		{
		}
		else if(d3dcaps9.VertexShaderVersion >= D3DVS_VERSION(2,0) && d3dcaps9.PixelShaderVersion >= D3DPS_VERSION(2,0))
		{
			LogInfo("Change into D3D9A as shader version < 3.0");
		}
		else
		{
			LogError("vs>=2.0 && ps>=2.0 not supported!!!");
			return false;
		}

		// vs texture
		if (SUCCEEDED(m_pD3D9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, bbSurfDesc.Format, D3DUSAGE_QUERY_VERTEXTEXTURE, D3DRTYPE_TEXTURE, D3D9Mapping::GetD3DFormat(PF_FLOAT32_R)))
			&& SUCCEEDED(m_pD3D9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, bbSurfDesc.Format, D3DUSAGE_QUERY_VERTEXTEXTURE, D3DRTYPE_TEXTURE, D3D9Mapping::GetD3DFormat(PF_FLOAT32_RGBA))))
		{
			GetDeviceCapabilities()->SetVSTextureSupported(true);
		}
		else
		{
			GetDeviceCapabilities()->SetVSTextureSupported(false);
		}

		SAFE_RELEASE(surface);

		GetDeviceCapabilities()->log();
		return true;
	}
}



