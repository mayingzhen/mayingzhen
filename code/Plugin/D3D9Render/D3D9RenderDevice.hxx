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

	Texture* D3D9RenderDevice::CreateTexture(int nWidth,int nHeight,FORMAT format)
	{
		return new D3D9Texture(nWidth,nHeight,format);
	}

	VertexDeclaration* D3D9RenderDevice::CreateVertexDeclaration()
	{
		return new D3D9VertexDeclaration(m_pD3DDevice);
	}

	VertexBuffer*	D3D9RenderDevice::CreateVertexBuffer(void* pData, int nsize, int nStride, USAGE Usgae)
	{
		return new D3D9VertexBuffer(pData,nsize,nStride,Usgae);
	}

	IndexBuffer*	D3D9RenderDevice::CreateIndexBuffer(void* Data, int nSize, INDEX_TYPE eIndexType, USAGE Usgae)
	{
		return new D3D9IndexBuffer(Data,nSize,eIndexType,Usgae);
	}

	ShaderProgram*		D3D9RenderDevice::CreateShaderProgram(const char* pszName,const char* pszDefine)
	{
		return new D3D9ShaderProgram(pszName,pszDefine);
	}

	const char*	D3D9RenderDevice::GetShaderPath()
	{
		return "/shader/d3d9/";
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
		InitD3D9(wndhandle);

		for (int i = 0; i < MAX_RENDER_TARGET; ++i)
		{
			D3D9RenderTarget* pRenderTarget =  new D3D9RenderTarget();
			LPDIRECT3DSURFACE9 surface = NULL;
			m_pD3DDevice->GetRenderTarget(i,&surface);
			pRenderTarget->SetD3DSurface(surface);
			m_pRenderTarget[i].push(pRenderTarget);
		}

		D3D9RenderTarget* pDepthStencil =  new D3D9RenderTarget();
		LPDIRECT3DSURFACE9 surface = NULL;
		m_pD3DDevice->GetDepthStencilSurface(&surface);
		pDepthStencil->SetD3DSurface(surface);
		m_pDepthStencil.push(pDepthStencil);

	
		D3DVIEWPORT9 vp;
		m_pD3DDevice->GetViewport(&vp);
		Rectangle curViewport;
		curViewport.x = (float)vp.X;
		curViewport.y = (float)vp.Y;
		curViewport.width = (float)vp.Width;
		curViewport.height = (float)vp.Height;
		m_viewport.push(curViewport);
	}

	void D3D9RenderDevice::InitD3D9(HWND hWnd)
	{
		m_hWnd = hWnd;

		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if(NULL == m_pD3D)
		{
			ASSERT(FALSE && "Direct3DCreate9(D3D_SDK_VERSION)");
			return ;
		}

		RECT rect;
		GetWindowRect(hWnd,&rect);

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

		UINT AdapterToUse = D3DADAPTER_DEFAULT;
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
				AdapterToUse = Adapter;
				DeviceType = D3DDEVTYPE_REF;
				//Logger::info(String("Using ") + Identifier.Description + " adapter for debug purposes.");
				break;
			}
		}
#endif

		HRESULT hr = S_OK;
		hr = m_pD3D->CreateDevice(AdapterToUse,DeviceType,hWnd,
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

	RenderTarget* D3D9RenderDevice::CreateRenderTarget(int nWidth,int nHeight,FORMAT format)
	{
		D3D9RenderTarget* pRT = new D3D9RenderTarget(nWidth,nHeight,format);
		return pRT;
	}

	void D3D9RenderDevice::PushRenderTarget(RenderTarget* pTexture,int index)
	{
		if (pTexture != NULL)
		{
			HRESULT hr = D3D_OK;
			D3D9RenderTarget* pD3D9Target = static_cast<D3D9RenderTarget*>(pTexture);
			IDirect3DSurface9* target = pD3D9Target->GetD3DSurface();

			hr = m_pD3DDevice->SetRenderTarget(index, target);
			ASSERT_MSG(hr == D3D_OK, "set render target failed.");

			m_pRenderTarget[index].push(pD3D9Target);
		}
		else
		{
			m_pD3DDevice->SetRenderTarget(index, NULL);
			m_pRenderTarget[index].push(NULL);

		}
	}

	void D3D9RenderDevice::PopRenderTarget(int index)
	{
		m_pRenderTarget[index].pop();
		D3D9RenderTarget* pD3D9Target = m_pRenderTarget[index].top();

		if (pD3D9Target != NULL)
		{
			HRESULT hr = D3D_OK;
			IDirect3DSurface9* target = pD3D9Target->GetD3DSurface();

			hr = m_pD3DDevice->SetRenderTarget(index, target);
			ASSERT_MSG(hr == D3D_OK, "set render target failed.");
		}
		else
		{
			m_pD3DDevice->SetRenderTarget(index, NULL);
		}
	}

	void D3D9RenderDevice::PushDepthStencil(RenderTarget* pTexture)
	{
		if (pTexture != NULL)
		{
			HRESULT hr = D3D_OK;
			D3D9RenderTarget* pD3D9Target = static_cast<D3D9RenderTarget*>(pTexture);
			IDirect3DSurface9* target = pD3D9Target->GetD3DSurface();

			hr = m_pD3DDevice->SetDepthStencilSurface(target);
			ASSERT_MSG(hr == D3D_OK, "set render target failed.");

			m_pDepthStencil.push(pD3D9Target);
		}
		else
		{
			m_pD3DDevice->SetDepthStencilSurface(NULL);
			m_pDepthStencil.push(NULL);
		}
	}

	void D3D9RenderDevice::PopDepthStencil()
	{
		m_pDepthStencil.pop();
		D3D9RenderTarget* pTexture = m_pDepthStencil.top();

		if (pTexture != NULL)
		{
			HRESULT hr = D3D_OK;
			D3D9RenderTarget* pD3D9Target = static_cast<D3D9RenderTarget*>(pTexture);
			IDirect3DSurface9* target = pD3D9Target->GetD3DSurface();

			hr = m_pD3DDevice->SetDepthStencilSurface(target);
			ASSERT_MSG(hr == D3D_OK, "set render target failed.");
		}
		else
		{
			m_pD3DDevice->SetDepthStencilSurface(NULL);
		}
	}

	void D3D9RenderDevice::PushViewport(const Rectangle& rect)
	{
		D3DVIEWPORT9 vp;
		vp.X      = (DWORD)rect.x;
		vp.Y      = (DWORD)rect.y;
		vp.Width  = (DWORD)rect.width;
		vp.Height = (DWORD)rect.height;
		vp.MinZ   = 0.0f;
		vp.MaxZ   = 1.0f;

		m_pD3DDevice->SetViewport(&vp);

		m_viewport.push(rect);
	}

	void D3D9RenderDevice::PopViewport()
	{
		m_viewport.pop();
		Rectangle rect = m_viewport.top();

		D3DVIEWPORT9 vp;
		vp.X      = (DWORD)rect.x;
		vp.Y      = (DWORD)rect.y;
		vp.Width  = (DWORD)rect.width;
		vp.Height = (DWORD)rect.height;
		vp.MinZ   = 0.0f;
		vp.MaxZ   = 1.0f;

		m_pD3DDevice->SetViewport(&vp);
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
				GetD3D9DxDevive()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
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

	void D3D9RenderDevice::DrawRenderable(const Renderable* pRenderable)
	{
		if (pRenderable == NULL)
			return;

		HRESULT hr = D3D_OK;

		D3D9VertexDeclaration* d3dvd = (D3D9VertexDeclaration*)pRenderable->m_pDeclaration;

		hr = m_pD3DDevice->SetVertexDeclaration(d3dvd->GetD3DVertexDeclaration());
		ASSERT(hr == D3D_OK);

		if (pRenderable->m_pIndexBuffer)
		{	
			D3D9IndexBuffer* pIndxBuffer = (D3D9IndexBuffer*)pRenderable->m_pIndexBuffer;
			hr = m_pD3DDevice->SetIndices(pIndxBuffer->GetD3DIndexBuffer());
			ASSERT(hr == D3D_OK);
		}

		D3D9VertexBuffer* pVertexBuffer =(D3D9VertexBuffer*)pRenderable->m_pVertexBuffers;
		hr = m_pD3DDevice->SetStreamSource(0,pVertexBuffer->GetD3DVertexBuffer(), 0, d3dvd->GetStreanmStride() );

		D3DPRIMITIVETYPE ePrimitiveType = D3D9Mapping::GetD3DPrimitiveType(pRenderable->m_ePrimitiveType);

		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData;

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

	void D3D9RenderDevice::DrawDyRenderable(const Renderable* pRenderable)
	{
		if (pRenderable == NULL)
			return;

		HRESULT hr = D3D_OK;

		D3D9VertexDeclaration* d3dvd = (D3D9VertexDeclaration*)pRenderable->m_pDeclaration;

		hr = m_pD3DDevice->SetVertexDeclaration(d3dvd->GetD3DVertexDeclaration());
		ASSERT(hr == D3D_OK);

		D3DPRIMITIVETYPE ePrimitiveType = D3D9Mapping::GetD3DPrimitiveType(pRenderable->m_ePrimitiveType);

		SubMeshData* pSubMeshData = pRenderable->m_pSubMeshData;

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

	void D3D9RenderDevice::ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s)
	{
		HRESULT hr;

		DWORD Flags = 0;
		D3DCOLOR cVal = c;
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

	Matrix4x4 D3D9RenderDevice::MakePerspectiveMatrix(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf)
	{
		 MatrixPerspectiveFovLH(pOut,fovy,Aspect,zn,zf);
		 return *pOut;
	}

	Matrix4x4 D3D9RenderDevice::MakeOrthoMatrix(Matrix4x4 *pOut, float width, float height, float zn, float zf)
	{
		D3DXMatrixOrthoLH((D3DXMATRIX*)pOut,width,height,zn,zf);
		return *pOut;
	}

	Matrix4x4 D3D9RenderDevice::MakeOrthoMatrixOffCenter(Matrix4x4 *pOut, float left, float right, float bottom, float top, float zn, float zf)
	{
		D3DXMatrixOrthoOffCenterLH((D3DXMATRIX*)pOut,left,right,bottom,top,zn,zf);
		return *pOut;
	}
}



