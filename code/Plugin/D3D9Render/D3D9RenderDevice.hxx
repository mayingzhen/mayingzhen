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
	}

	D3D9RenderDevice::~D3D9RenderDevice()
	{
	}


	Texture* D3D9RenderDevice::CreateTexture(const char* pszPath)
	{
		return new D3D9Texture(pszPath);
	}

	Texture* D3D9RenderDevice::CreateTexture(int nWidth,int nHeight,FORMAT format,bool bDepthStencil)
	{
		return new D3D9Texture(nWidth,nHeight,format,bDepthStencil);
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

	ShaderProgram*		D3D9RenderDevice::CreateShaderProgram(Technique* pTech,const char* pVSFile,const char* pPSFile,const char* pszDefine)
	{
		return new D3D9ShaderProgram(pTech,pVSFile,pPSFile,pszDefine);
	}

	const char*	D3D9RenderDevice::GetShaderPath()
	{
		return "shader/d3d9/";
	}

	void D3D9RenderDevice::ConvertUV(float& fTop,float& fLeft,float& fRight,float& fBottom)
	{
		return;
	}

	float D3D9RenderDevice::GetHalfPixelOffset(float fHalfPiexl)
	{
		return fHalfPiexl;
	}

	void D3D9RenderDevice::Init(HWND wndhandle)
	{
		m_hWnd = wndhandle;

		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if(NULL == m_pD3D)
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
		for (UINT Adapter = 0; Adapter < m_pD3D->GetAdapterCount(); Adapter++)
		{
			D3DADAPTER_IDENTIFIER9 Identifier;
			HRESULT Res;
			Res = m_pD3D->GetAdapterIdentifier(Adapter, 0, &Identifier);
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
		hr = m_pD3D->CreateDevice(m_nAdapterToUse,DeviceType,m_hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&m_d3dpp,&m_pD3DDevice);

		if( FAILED(hr) )
		{
			ASSERT(FALSE && "m_pD3D->CreateDevice()");
			return;
		}
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
		ASSERT_MSG(hr == D3D_OK, "set render target failed.");
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
		ASSERT_MSG(hr == D3D_OK, "set render target failed.");
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

	void D3D9RenderDevice::SetRenderState(const RenderState& state)
	{
		if (m_curState.m_eCullMode != state.m_eCullMode)
		{
			m_curState.m_eCullMode = state.m_eCullMode;

			if (state.m_eCullMode == CULL_FACE_SIDE_BACK)
			{
				GetD3D9DxDevive()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
			else if (state.m_eCullMode == CULL_FACE_SIDE_FRONT)
			{
				GetD3D9DxDevive()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);	
			}
			else if (state.m_eCullMode == CULL_FACE_SIDE_NONE)
			{
				GetD3D9DxDevive()->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
			}
		}

		//GetD3D9DxDevive()->SetRenderState(D3DRS_FILLMODE, state.fillMode);

		//mD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, state.colorWrite);

		if (m_curState.m_bDepthWrite != state.m_bDepthWrite)
		{
			m_curState.m_bDepthWrite = state.m_bDepthWrite;
			m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,state.m_bDepthWrite);
			
		}

		if (m_curState.m_bColorWrite != state.m_bColorWrite)
		{
			m_curState.m_bColorWrite = state.m_bColorWrite;
			m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE,state.m_bColorWrite);
		}

		if ( abs(m_curState.m_fDepthBias - state.m_fDepthBias) > 0.0001f )
		{
			m_curState.m_fDepthBias = state.m_fDepthBias;
			m_pD3DDevice->SetRenderState(D3DRS_DEPTHBIAS,*(DWORD*)&m_curState.m_fDepthBias);
		}
		
		if (m_curState.m_eDepthCheckMode != state.m_eDepthCheckMode)
		{
			m_curState.m_eDepthCheckMode = state.m_eDepthCheckMode;
			switch (state.m_eDepthCheckMode)
			{
			case DCM_NONE:
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
				break;

			case DCM_LESS_EQUAL:
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
				break;

			case DCM_LESS:
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
				break;

			case DCM_GREATER_EQUAL:
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
				break;

			case DCM_GREATER:
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
				break;

			case DCM_EQUAL:
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
				break;

			case DCM_ALWAYS:
				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
				break;
			}
		}


		if (m_curState.m_eBlendMode != state.m_eBlendMode)
		{
			m_curState.m_eBlendMode = state.m_eBlendMode;
			switch (state.m_eBlendMode)
			{
			case BM_OPATICY:
				GetD3D9DxDevive()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				break;

			case BM_TRANSPARENT:
				GetD3D9DxDevive()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				GetD3D9DxDevive()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				GetD3D9DxDevive()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				break;

			case BM_ADD:
				GetD3D9DxDevive()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				GetD3D9DxDevive()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				GetD3D9DxDevive()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
				break;

			case BM_MULTIPLY:
				GetD3D9DxDevive()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				GetD3D9DxDevive()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
				GetD3D9DxDevive()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
				break;
			}
		}
	}

	void D3D9RenderDevice::DrawRenderable(const Renderable* pRenderable,Technique* pTech)
	{
		if (pRenderable == NULL)
			return;

		HRESULT hr = D3D_OK;

		D3D9VertexDeclaration* d3dvd = (D3D9VertexDeclaration*)pRenderable->m_pDeclaration.get();

		hr = m_pD3DDevice->SetVertexDeclaration(d3dvd->GetD3DVertexDeclaration());
		ASSERT(hr == D3D_OK);

		if (pRenderable->m_pIndexBuffer)
		{	
			D3D9IndexBuffer* pIndxBuffer = (D3D9IndexBuffer*)pRenderable->m_pIndexBuffer.get();
			hr = m_pD3DDevice->SetIndices(pIndxBuffer->GetD3DIndexBuffer());
			ASSERT(hr == D3D_OK);
		}

		D3D9VertexBuffer* pVertexBuffer =(D3D9VertexBuffer*)pRenderable->m_pVertexBuffers.get();
		hr = m_pD3DDevice->SetStreamSource(0,pVertexBuffer->GetD3DVertexBuffer(), 0, d3dvd->GetStreanmStride() );

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

		hr = m_pD3DDevice->DrawIndexedPrimitive(ePrimitiveType,
			0,
			pSubMeshData->m_nVertexStart,
			pSubMeshData->m_nVertexCount,
			pSubMeshData->m_nIndexStart,
			nPrimCount);
		ASSERT(hr == D3D_OK && "DrawIndexedPrimitive");

	}

	void D3D9RenderDevice::DrawDyRenderable(const Renderable* pRenderable,Technique* pTech)
	{
		if (pRenderable == NULL)
			return;

		HRESULT hr = D3D_OK;

		D3D9VertexDeclaration* d3dvd = (D3D9VertexDeclaration*)pRenderable->m_pDeclaration.get();

		hr = m_pD3DDevice->SetVertexDeclaration(d3dvd->GetD3DVertexDeclaration());
		ASSERT(hr == D3D_OK);

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

		hr = m_pD3DDevice->DrawIndexedPrimitiveUP(ePrimitiveType,
			pSubMeshData->m_nVertexStart,
			pSubMeshData->m_nVertexCount,
			nPrimCount,
			pRenderable->m_pIndexBuffer->GetData(),
			D3D9Mapping::GetD3DIndexType(pRenderable->m_pIndexBuffer->GetIndexType()),
			pRenderable->m_pVertexBuffers->GetData(),
			pRenderable->m_pVertexBuffers->GetStride());
		ASSERT(hr == D3D_OK && "DrawIndexedPrimitive");
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
// 		float fHalf = fovy * 0.5f;
// 		float sine = Math::Sin(fHalf), cosi = Math::Cos(fHalf);
// 		float fCotangent = cosi / sine;
// 
// 		out[0][0] = fCotangent / Aspect;
// 		out[1][1] = fCotangent;
// 
// 		out[2][2] =  (zn + zf) / (zn - zf);
// 		out[3][2] = -1.0f;
// 		out[2][3] =  2.0f * zn * zf / (zn - zf);
// 
// 		out[1][0] = out[2][0] = out[3][0] = 
// 			out[0][1] = out[2][1] = out[3][1] = 
// 			out[0][2] = out[1][2] =
// 			out[0][3] = out[1][3] = out[3][3] = 0.0f;
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

	bool D3D9RenderDevice::CheckTextureFormat(FORMAT eFormat,USAGE eUsage)
	{
		DWORD D3DUsage =  D3D9Mapping::GetD3DUsage(eUsage); 
		D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(eFormat);

		HRESULT hr = D3D_OK;
		hr = m_pD3D->CheckDeviceFormat(m_nAdapterToUse, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8,
			D3DUsage, D3DRTYPE_TEXTURE, D3DFormat);
		if (hr == D3D_OK || hr == D3DOK_NOAUTOGEN)
		{
			return true;
		}

		return false;
	}
}



