#include "D3D9RenderDevice.h"
//#include "D3D9RendMesh.h"
#include "D3D9Texture.h"


namespace ma
{

	D3D9RenderDevice::D3D9RenderDevice()
	{
		m_pGBufferTech = NULL;
		m_pShadingTech = NULL;

		m_mainLigt = NULL;

		m_pLineRender = new LineRender();
		m_pScreenQuad = new ScreenQuad();
		m_pUnitSphere = new UnitSphere();

		memset(m_arrShadowMap,0,sizeof(m_arrShadowMap));
	}

	D3D9RenderDevice::~D3D9RenderDevice()
	{
		SAFE_DELETE(m_pLineRender);
		SAFE_DELETE(m_pScreenQuad);
		SAFE_DELETE(m_pUnitSphere);
	}


	Texture* D3D9RenderDevice::CreateRendTexture()
	{
		return new D3D9Texture();
	}

// 	RenderTarget* D3D9RenderDevice::CreateRenderTarget()
// 	{
// 		return new RenderTarget();
// 	}

	VertexDeclaration* D3D9RenderDevice::CreateVertexDeclaration()
	{
		return new D3D9VertexDeclaration(m_pD3DDevice);
	}

	VertexBuffer*	D3D9RenderDevice::CreateVertexBuffer()
	{
		return new D3D9VertexBuffer(m_pD3DDevice);
	}

	IndexBuffer*	D3D9RenderDevice::CreateIndexBuffer()
	{
		return new D3D9IndexBuffer(m_pD3DDevice);
	}

	Technique*		D3D9RenderDevice::CreateTechnique()
	{
		return new D3D9Technique(m_pD3DDevice);
	}

	void D3D9RenderDevice::Init(HWND wndhandle)
	{
		InitD3D9(wndhandle);

		InitRender();

		m_pLineRender->Init(m_pD3DDevice);

		m_pScreenQuad->Init(m_pD3DDevice);

		m_pUnitSphere->Init(m_pD3DDevice);
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
			return ;
		}

		return ;
	}

	void D3D9RenderDevice::InitRender()
	{
		ASSERT(m_pD3DDevice);
		if (m_pD3DDevice == NULL)
			return;

		DWORD dwShaderFlags = 0;
#ifdef _DEBUG
		dwShaderFlags |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif // _DEBUG

		HRESULT hr = D3D_OK;

		ID3DXEffect* m_pDefaultEffect = NULL;
		hr = D3DXCreateEffectFromFile( m_pD3DDevice, "../Data/shader/Default.fx",
			NULL, NULL, dwShaderFlags, NULL, &m_pDefaultEffect, NULL ); 
		

		hr = D3DXCreateEffectFromFile( m_pD3DDevice, "../Data/shader/Gbuffer.fx", 
			NULL, NULL, dwShaderFlags, NULL, &m_pGBufferTech, NULL);

		hr = D3DXCreateEffectFromFile(m_pD3DDevice, "../Data/shader/DeferredLight.fx", 
			NULL, NULL, dwShaderFlags, NULL, &m_DeferredLightTech, NULL);

		hr = D3DXCreateEffectFromFile(m_pD3DDevice, "../Data/shader/DefferedShadow.fx", 
			NULL, NULL, dwShaderFlags, NULL, &m_DefferedShdowTech, NULL);
		
		hr = D3DXCreateEffectFromFile(m_pD3DDevice, "../Data/shader/shadowmap.fx", 
			NULL, NULL, dwShaderFlags, NULL, &m_pShdowMapTech, NULL);
		
		hr = D3DXCreateEffectFromFile( m_pD3DDevice, "../Data/shader/Shading.fx", 
			NULL, NULL, dwShaderFlags, NULL, &m_pShadingTech, NULL);

		UINT uBackBuffW = m_d3dpp.BackBufferWidth;
		UINT uBackBuffH = m_d3dpp.BackBufferHeight;

		hr = m_pD3DDevice->CreateTexture( uBackBuffW, uBackBuffH, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, 
			D3DPOOL_DEFAULT, &m_pDepthTex, NULL);
		ASSERT( SUCCEEDED(hr) && "D3DFMT_R32F Render Target 创建失败" );

		hr = m_pD3DDevice->CreateTexture( uBackBuffW, uBackBuffH, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
			D3DPOOL_DEFAULT, &m_pNormalTex, NULL);
		ASSERT( SUCCEEDED(hr) && "D3DFMT_A8R8G8B8 Render Target 创建失败" );

		hr = m_pD3DDevice->CreateTexture( uBackBuffW, uBackBuffW, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, 
			D3DPOOL_DEFAULT, &m_pShadowTex, NULL);
		ASSERT( SUCCEEDED(hr) && "D3DFMT_R16F Render Target 创建失败" );

		hr = m_pD3DDevice->CreateTexture( uBackBuffW, uBackBuffW, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
			D3DPOOL_DEFAULT, &m_pDiffuseTex, NULL);
		ASSERT( SUCCEEDED(hr) && "D3DFMT_A8R8G8B8 Render Target 创建失败" );

		hr = m_pD3DDevice->CreateTexture( uBackBuffW, uBackBuffW, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
			D3DPOOL_DEFAULT, &m_pSpeculaTex, NULL);
		ASSERT( SUCCEEDED(hr) && "D3DFMT_A8R8G8B8 Render Target 创建失败" );
	}

	void D3D9RenderDevice::BeginRender()
	{
 		HRESULT hr = D3D_OK;
		Color clearColor(0,45.0f / 255.0f,50.0f/255.0f,170.0f/255.0f);
		ClearBuffer(true,true,true,clearColor,1.0f,0);
// 		hr = m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,45,50,170), 1.0f, 0);
// 		ASSERT( SUCCEEDED(hr) );
		
		hr = m_pD3DDevice->BeginScene();
		ASSERT( SUCCEEDED(hr) );
	}

	void D3D9RenderDevice::EndRender()
	{
		//FlushRenderQueue();

		//FlushLine();
	
		HRESULT hr = D3D_OK;
		hr = m_pD3DDevice->EndScene();
		ASSERT( SUCCEEDED(hr) );

		hr = m_pD3DDevice->Present(NULL,NULL,NULL,NULL);
		ASSERT( SUCCEEDED(hr) );
	}

	void D3D9RenderDevice::SetRenderTarget(int index,Texture* pTexture)
	{
		if (pTexture != NULL)
		{
			HRESULT hr = D3D_OK;
			D3D9Texture* pD3D9Texture = static_cast<D3D9Texture*>(pTexture);
			IDirect3DSurface9* target = pD3D9Texture->GetD3DSurface();

			hr = m_pD3DDevice->SetRenderTarget(index, target);
			ASSERT_MSG(hr == D3D_OK, "set render target failed.");
		}
		else
		{
			m_pD3DDevice->SetRenderTarget(index, NULL);
		}

		//mRenderTarget[index] = target;
	}

	void D3D9RenderDevice::FlushRenderQueue()
	{
		ShadowMapPass();

		GBufferPass();	

		DefferdShadowPass();

		DefferdLigtPass();

		ShadingPass();

		m_SolidEntry.clear();
		m_TransEntry.clear();
		//m_Ligts.clear();
	}

	void D3D9RenderDevice::ShadingPass()
	{
// 		UINT uNumber = m_SolidEntry.size();
// 		if (uNumber <= 0)
// 			return;
// 
// 		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"ShadingPass");
// 
// 		HRESULT hr = S_OK;
// 
// 		m_pD3DDevice->Clear( 0, NULL,
// 			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
// 			D3DCOLOR_ARGB(0,45,50,170), 1.0f, 0);
// 
// 		for(UINT i = 0; i < uNumber; ++i)
// 		{
// 			IRenderItem* pRenderItem = m_SolidEntry[i];
// 			//D3D9RendMesh* pDxMesh = (D3D9RendMesh*)pRenderItem->m_pMesh;
// 
// 			D3D9Texture* pDxTexture = (D3D9Texture*)pRenderItem->m_pTex;
// 			
// 
// 			ID3DXEffect* pCurEffect = m_pShadingTech;
// 
// 			if (pRenderItem->m_nSkinMatrixNum != 0)
// 			{
// 				hr = pCurEffect->SetTechnique("SkinShadinTech");
// 				hr = pCurEffect->SetMatrixArray("mSkinMatrixArray",(const D3DXMATRIX*)pRenderItem->m_arrSkinMatrix,pRenderItem->m_nSkinMatrixNum);
// 			}
// 			else
// 			{
// 				hr = pCurEffect->SetTechnique("Shading");
// 			}
// 
// 			Matrix4x4 matWVP = *(pRenderItem->m_pMatWorld) * m_pMainCamera->GetViewProjMatrix();
// 			Matrix4x4 matWV = *(pRenderItem->m_pMatWorld) * m_pMainCamera->GetViewMatrix();
// 			hr = pCurEffect->SetMatrix("worldviewprojection",(const D3DXMATRIX*)&matWVP);
// 			hr = pCurEffect->SetMatrix("worldview",(const D3DXMATRIX*)&matWV);
// 
// 			hr = pCurEffect->SetTexture("g_TextureSrcDiffuse",pDxTexture->GetD3DTexture());
// 			hr = pCurEffect->SetTexture( "g_TextureLightDiffuse", m_pDiffuseTex );
// 			hr = pCurEffect->SetTexture( "g_TextureLightSpecular", m_pSpeculaTex ); 
// 
// 
// 			UINT cPasses = 0; 
// 			hr = pCurEffect->Begin(&cPasses, 0 );
// 			for (UINT i = 0; i < cPasses; ++i)
// 			{
// 				hr = pCurEffect->BeginPass(i);
// 				hr = pCurEffect->CommitChanges();
// 				//pRenderItem->Render();
// 				//Render(NULL,pRenderItem);
// 				//pDxMesh->GetD3DXMesh()->DrawSubset(0);
// 				pCurEffect->EndPass();
// 			}	
// 			pCurEffect->End();
// 		}
// 
// 		D3DPERF_EndEvent();
	}

	void D3D9RenderDevice::GBufferPass()
	{
// 		UINT uNumber = m_SolidEntry.size();
// 		if (uNumber <= 0)
// 			return;
// 
// 		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"GBufferPass");
// 
// 		HRESULT hr = S_OK;
// 
// 		LPDIRECT3DSURFACE9 pOldRT0 = NULL;
// 		m_pD3DDevice->GetRenderTarget(0, &pOldRT0);
// 		LPDIRECT3DSURFACE9 pOldRT1 = NULL;
// 		m_pD3DDevice->GetRenderTarget(1, &pOldRT1);
// 
// 		LPDIRECT3DSURFACE9 pSurfDepth = NULL;
// 		hr = m_pDepthTex->GetSurfaceLevel(0,&pSurfDepth);
// 		hr = m_pD3DDevice->SetRenderTarget(0,pSurfDepth);
// 		SAFE_RELEASE(pSurfDepth);
// 
// 		LPDIRECT3DSURFACE9 pSurfNormal = NULL;
// 		hr = m_pNormalTex->GetSurfaceLevel(0,&pSurfNormal);
// 		hr = m_pD3DDevice->SetRenderTarget(1,pSurfNormal);
// 		SAFE_RELEASE(pSurfNormal)
// 
// 		m_pD3DDevice->Clear(0, NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
// 			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
// 
// 		float fNearClip = m_pMainCamera->GetNearClip();
// 		float fFarClip = m_pMainCamera->GetFarClip();
// 		Vector4 depth_near_far_invfar = Vector4(fNearClip, fFarClip, 1 / fFarClip, 0);
// 
// 		for(UINT i = 0; i < uNumber; ++i)
// 		{
// 			IRenderItem* pRenderItem = m_SolidEntry[i];
// 			if (pRenderItem == NULL)
// 				continue;
// 
// 			//D3D9RendMesh* pD3D9RenderMesh = (D3D9RendMesh*)pRenderItem->m_pMesh; 
// 
// 			ID3DXEffect* pCurEffect = m_pGBufferTech;
// 
// 			if (pRenderItem->m_nSkinMatrixNum != 0)
// 			{
// 				hr = pCurEffect->SetTechnique("SkinGBufferTech");
// 				hr = pCurEffect->SetMatrixArray("mSkinMatrixArray",(const D3DXMATRIX*)pRenderItem->m_arrSkinMatrix,pRenderItem->m_nSkinMatrixNum);
// 			}
// 			else
// 			{
// 				hr = pCurEffect->SetTechnique("GBufferTech");
// 			}
// 
// 			Matrix4x4 matWVP = *(pRenderItem->m_pMatWorld) * m_pMainCamera->GetViewProjMatrix();
// 			Matrix4x4 matWV = *(pRenderItem->m_pMatWorld) * m_pMainCamera->GetViewMatrix();
// 			hr = pCurEffect->SetMatrix("worldviewprojection",(const D3DXMATRIX*)&matWVP);
// 			hr = pCurEffect->SetMatrix("worldview",(const D3DXMATRIX*)&matWV);
// 			hr = pCurEffect->SetVector("depth_near_far_invfar",(const D3DXVECTOR4*)&depth_near_far_invfar);
// 
// 			UINT cPasses = 0; 
// 			hr = pCurEffect->Begin(&cPasses, 0 );
// 			for (UINT i = 0; i < cPasses; ++i)
// 			{
// 				hr = pCurEffect->BeginPass(i);
// 				hr = pCurEffect->CommitChanges();
// 				//pD3D9RenderMesh->GetD3DXMesh()->DrawSubset(0);
// 				pCurEffect->EndPass();
// 			}	
// 			pCurEffect->End();
// 		}
// 
// 		hr = m_pD3DDevice->SetRenderTarget(0, pOldRT0);
// 		SAFE_RELEASE(pOldRT0);
// 		hr = m_pD3DDevice->SetRenderTarget(1, pOldRT1);
// 		SAFE_RELEASE(pOldRT1);
// 
// 		D3DPERF_EndEvent();
	}

	void D3D9RenderDevice::ShadowMapPass()
	{
// 		if (m_mainLigt == NULL || m_pMainCamera == NULL)
// 			return;
// 
// 		HRESULT hr = D3D_OK;
// 
// 		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"ShadowMapPass");
// 
// 		float fDepthBias = 0.002f;
// 		m_pD3DDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&fDepthBias);
// 
// 		UINT uNumber = m_SolidEntry.size();
// 		if (uNumber <= 0)
// 			return;
// 
// 		for (UINT i = 0; i < 4; ++i)
// 		{
// 			//D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"ShadowMapPass");
// 
// 
// 			ShadowMap* pShadowMap = m_arrShadowMap[i];
// 			if(pShadowMap == NULL)
// 			{
// 				m_arrShadowMap[i] = new ShadowMap();
// 				pShadowMap = m_arrShadowMap[i];
// 				pShadowMap->Init(m_pD3DDevice);
// 			}
// 
// 			pShadowMap->CalculateMatrix(m_pMainCamera,i,m_mainLigt);
// 
// 			pShadowMap->ClearCaster();
// 			if (!m_SolidEntry.empty())
// 				pShadowMap->FindCaster((IRenderItem**)&m_SolidEntry[0],m_SolidEntry.size());
// 			if (!m_TransEntry.empty())
// 				pShadowMap->FindCaster((IRenderItem**)&m_TransEntry[0],m_TransEntry.size());
// 
// 			pShadowMap->BeginRender();
// 
// 			m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00FFFFFF, 1.0f, 0L);
// 
// 			UINT nCaster = pShadowMap->GetCasterNumber();
// 			for(UINT j = 0; j < nCaster; ++j)
// 			{
// 				IRenderItem* pRenderItem = pShadowMap->GetCasterByIndex(j);
// 				if (pRenderItem == NULL)
// 					continue;
// 
// 				//D3D9RendMesh* pD3D9RenderMesh = (D3D9RendMesh*)pRenderItem->m_pMesh; 
// 
// 				ID3DXEffect* pCurEffect = m_pShdowMapTech;
// 
// 				if (pRenderItem->m_nSkinMatrixNum != 0)
// 				{
// 					hr = pCurEffect->SetTechnique("SkinHWRenderShadow");
// 					hr = pCurEffect->SetMatrixArray("mSkinMatrixArray",(const D3DXMATRIX*)pRenderItem->m_arrSkinMatrix,pRenderItem->m_nSkinMatrixNum);
// 				}
// 				else
// 				{
// 					hr = pCurEffect->SetTechnique("HWRenderShadow");
// 				}
// 
// 				Matrix4x4 matWVP = *(pRenderItem->m_pMatWorld) * pShadowMap->GetViewMatrix() * pShadowMap->GetProjMatrix();
// 				Matrix4x4 matWV = *(pRenderItem->m_pMatWorld) * pShadowMap->GetViewMatrix();
// 				pCurEffect->SetMatrix("worldviewprojection",(const D3DXMATRIX*)&matWVP);
// 				pCurEffect->SetMatrix("worldview",(const D3DXMATRIX*)&matWV);
// 
// 				UINT cPasses = 0; 
// 				hr = pCurEffect->Begin(&cPasses, 0 );
// 				for (UINT i = 0; i < cPasses; ++i)
// 				{
// 					hr = pCurEffect->BeginPass(i);
// 					hr = pCurEffect->CommitChanges();
// 					//pD3D9RenderMesh->GetD3DXMesh()->DrawSubset(0);
// 					pCurEffect->EndPass();
// 				}	
// 				pCurEffect->End();
// 			}
// 
// 			pShadowMap->EndRender();	
// 		}
// 
// 		m_pD3DDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);
// 
// 		D3DPERF_EndEvent();
	}

	void D3D9RenderDevice::DefferdLigtPass()
	{
// 		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"DeferredLight");
// 
// 		HRESULT hr = S_OK;
// 
// 		LPDIRECT3DSURFACE9 pOldRT0 = NULL;
// 		m_pD3DDevice->GetRenderTarget(0, &pOldRT0);
// 
// 		LPDIRECT3DSURFACE9 pOldRT1 = NULL;
// 		m_pD3DDevice->GetRenderTarget(1, &pOldRT1);
// 
// 		LPDIRECT3DSURFACE9 pSurfDiffuse = NULL;
// 		hr = m_pDiffuseTex->GetSurfaceLevel(0,&pSurfDiffuse);
// 		hr = m_pD3DDevice->SetRenderTarget(0,pSurfDiffuse);
// 		SAFE_RELEASE(pSurfDiffuse);
// 
// 		LPDIRECT3DSURFACE9 pSurfSpecula = NULL;
// 		hr = m_pSpeculaTex->GetSurfaceLevel(0,&pSurfSpecula);
// 		hr = m_pD3DDevice->SetRenderTarget(1,pSurfSpecula);
// 		SAFE_RELEASE(pSurfSpecula)
// 
// 		m_pD3DDevice->Clear(0,NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
// 			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
// 
// 
// 		ID3DXEffect* pCurEffect = m_DeferredLightTech;
// 
// 		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"AmbientLight");
// 
// 		pCurEffect->SetTechnique("AmbientLight");
// 
// 		UINT cPasses = 0; 
// 		hr = pCurEffect->Begin(&cPasses, 0 );
// 		for (UINT i = 0; i < cPasses; ++i)
// 		{
// 			hr = pCurEffect->BeginPass(i);
// 			//hr = pCurEffect->CommitChanges();
// 			m_pScreenQuad->Render(pCurEffect);
// 			pCurEffect->EndPass();
// 		}	
// 		pCurEffect->End();
// 
// 		D3DPERF_EndEvent();
// 
// 		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"DiffuseLight");
// 
// 		hr = pCurEffect->SetTexture( "g_TextureSrcPos", m_pDepthTex );
// 		hr = pCurEffect->SetTexture( "g_TextureSrcNormal", m_pNormalTex ) ;
// 		hr = pCurEffect->SetTexture( "g_TextureShadow", m_pShadowTex );
// 
// 		for (UINT i = 0; i < m_Ligts.size(); ++i)
// 		{
// 			Light* pLight = m_Ligts[i];
// 			if (pLight == NULL)
// 				continue;
// 
// 			Vector4 lightColor =  pLight->GetDiffuse(); 
// 			hr = pCurEffect->SetVector( "light_color", (const D3DXVECTOR4*)&lightColor );
// 
// 			float fNearClip = m_pMainCamera->GetNearClip();
// 			float fFarClip = m_pMainCamera->GetFarClip();
// 			Vector4 depth_near_far_invfar = Vector4(fNearClip, fFarClip, 1 / fFarClip, 0 );
// 			hr = pCurEffect->SetVector( "depth_near_far_invfar", (const D3DXVECTOR4*)&depth_near_far_invfar );
// 			
// 			if (pLight->GetLightType() == LIGHT_POINT)
// 			{
// 				if ( !pLight->IsCreateShadow() )
// 				{
// 					hr = pCurEffect->SetTechnique("DiffusePointLight");
// 				}
// 				else
// 				{
// 					hr = pCurEffect->SetTechnique("DiffuseShadowPointLight");
// 				}
// 
// 				Vector3 lightPosWS = pLight->GetPositionWS();
// 				Vector3 lightPosES;
// 				Vec3TransformCoord(&lightPosES, &lightPosWS, &m_pMainCamera->GetViewMatrix());
// 				Vector4 lightPosView(lightPosES.x,lightPosES.y,lightPosES.z,1);
// 				hr = pCurEffect->SetVector( "light_pos_es", (const D3DXVECTOR4*)&lightPosView );
// 			
// 				float fExpensionRadius = pLight->GetRadius() * 1.08f;
// 			
// 				Matrix4x4 matTrans,matScal,matWorld;
// 				MatrixTranslation( &matTrans, lightPosWS.x, lightPosWS.y, lightPosWS.z );
// 				MatrixScaling( &matScal,fExpensionRadius, fExpensionRadius, fExpensionRadius );
// 				matWorld = matScal * matTrans;
// 
// 				Matrix4x4 matWVP = matWorld * m_pMainCamera->GetViewProjMatrix();
// 				pCurEffect->SetMatrix("worldviewprojection",(const D3DXMATRIX*)&matWVP);
// 
// 				UINT cPasses = 0; 
// 				hr = pCurEffect->Begin(&cPasses, 0 );
// 				for (UINT i = 0; i < cPasses; ++i)
// 				{
// 					hr = pCurEffect->BeginPass(i);
// 					m_pUnitSphere->Render(pCurEffect);
// 					pCurEffect->EndPass();
// 				}	
// 				pCurEffect->End();
// 
// 			}
// 			else if (pLight->GetLightType() == LIGHT_DIRECTIONAL)
// 			{
// 				if ( !pLight->IsCreateShadow() )
// 				{
// 					hr = pCurEffect->SetTechnique("DiffuseDirectLight");
// 				}
// 				else
// 				{
// 					hr = pCurEffect->SetTechnique("DiffuseShadowDirectLight");
// 				}
// 			
// 				Vector3 lightDir = pLight->GetDirection();	
// 				Vec3TransformNormal(&lightDir, &lightDir, &m_pMainCamera->GetViewMatrix());
// 				Vector4 ligtDirView(lightDir.x,lightDir.y,lightDir.z,0);
// 				hr = pCurEffect->SetVector( "light_dir_es", (const D3DXVECTOR4*)&ligtDirView );
// 
// 				UINT cPasses = 0; 
// 				hr = pCurEffect->Begin(&cPasses, 0 );
// 				for (UINT i = 0; i < cPasses; ++i)
// 				{
// 					hr = pCurEffect->BeginPass(i);
// 					m_pScreenQuad->Render(pCurEffect);
// 					pCurEffect->EndPass();
// 				}	
// 				pCurEffect->End();
// 			}
// 		}
// 
// 		D3DPERF_EndEvent();
// 
// 
// 		hr = m_pD3DDevice->SetRenderTarget(0, pOldRT0);
// 		hr = m_pD3DDevice->SetRenderTarget(1, pOldRT1);
// 		SAFE_RELEASE(pOldRT0);
// 		SAFE_RELEASE(pOldRT1);
// 
// 		D3DPERF_EndEvent();	
	}

	void D3D9RenderDevice::DefferdShadowPass()
	{

// 		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"DefferdShadow");
// 
// 		HRESULT hr = S_OK;
// 
// 		LPDIRECT3DSURFACE9 pOldRT = NULL;
// 		m_pD3DDevice->GetRenderTarget(0, &pOldRT);	
// 
// 		LPDIRECT3DSURFACE9 pSurfShadowTex;
// 		hr = m_pShadowTex->GetSurfaceLevel( 0, &pSurfShadowTex);
// 		hr = m_pD3DDevice->SetRenderTarget( 0, pSurfShadowTex );
// 		SAFE_RELEASE( pSurfShadowTex );
// 
// 		m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_RGBA(255, 255, 255, 0), 1.0f, 0);
// 
// 		Matrix4x4 invView;
// 		MatrixInverse(&invView, NULL, &m_pMainCamera->GetViewMatrix());
// 
// 		//const std::list<Light>& lightList = g_SceneMng.GetLigtList();
// 		//std::list<Light>::const_iterator lightIt = lightList.begin();
// 		//for (; lightIt != lightList.end(); ++lightIt)
// 		if (m_mainLigt)
// 		{
// 			Light* pLigt = m_mainLigt;
// 
// 			LPD3DXEFFECT pCurEffect = m_DefferedShdowTech;
// 
// 			float fNearClip = m_pMainCamera->GetNearClip();
// 			float fFarClip = m_pMainCamera->GetFarClip();
// 			Vector4 depth_near_far_invfar(fNearClip, fFarClip, 1 / fFarClip, 0 );
// 			hr = pCurEffect->SetVector( "depth_near_far_invfar", (const D3DXVECTOR4 *)&depth_near_far_invfar );
// 
// 			Matrix4x4 viwToLightProjArray[Camera::NUM_PSSM];
// 			Matrix4x4 wordToLightView[Camera::NUM_PSSM];
// 			for (int i = 0; i < Camera::NUM_PSSM; ++i) 
// 			{
// 				ShadowMap* pShadowMap = m_arrShadowMap[i];
// 				if (pShadowMap == NULL)
// 					continue;
// 
// 				viwToLightProjArray[i] = invView * pShadowMap->GetTexMatrix();
// 				wordToLightView[i] = invView * pShadowMap->GetViewMatrix();
// 
// 				char strTexShadowMap[MAX_PATH] = {0};
// 				_snprintf(strTexShadowMap, sizeof(strTexShadowMap), "TexShadowMap%d", i);
// 				hr = pCurEffect->SetTexture( strTexShadowMap, pShadowMap->GetDepthTexture() ) ;
// 			}
// 
// 			hr = pCurEffect->SetTexture( "g_TextureSrcPos", m_pDepthTex );
// 
// 			//V( effect->SetTexture("Jitter", m_pJitterTexture) );
// 
// 			hr = pCurEffect->SetMatrixArray( "viwToLightProjArray", (const D3DXMATRIX*)viwToLightProjArray, Camera::NUM_PSSM );
// 			hr = pCurEffect->SetMatrixArray( "wordLightView", (const D3DXMATRIX *)wordToLightView, Camera::NUM_PSSM );
// 			hr = pCurEffect->SetFloatArray( "splitPos", m_pMainCamera->GetSplitPos(), Camera::NUM_PSSM );
// 
// 			//if (lightIt->m_Type == Light::TYPE_ORTHOGRAPHIC)
// 			//{
// 			//	m_DefferedShdowTech.CommitChanges();
// 			//	g_SceneQuad.Render();
// 			//}
// 			//else
// 			//{
// 			//	Vector3 lightPos = Vector3( lightIt->m_vSource.x, lightIt->m_vSource.y, lightIt->m_vSource.z );
// 			//	float lightRadius = lightIt->m_fFar - lightIt->m_fNear;
// 
// 			//	Matrix4x4 matTrans,matScal,matWorld;
// 			//	MatrixTranslation( &matTrans, lightPos.x, lightPos.y, lightPos.z );
// 			//	MatrixScaling( &matScal, 0.5f * lightRadius, 0.5f * lightRadius, 0.5f * lightRadius );
// 			//	matWorld = matScal * matTrans;
// 
// 				//m_LightModel.Render(&matWorld);
// 			//}
// 
// // 			for (int i = 0; i < Camera::NUM_PSSM; ++i) // mutli pass
// // 			{
// // 				m_pD3DDevice->SetTexture( i, NULL );
// // 			}
// 
// 			//m_DefferedShdowTech.endPass();
// 			//m_DefferedShdowTech.end();
// 
// 			UINT cPasses = 0; 
// 			hr = pCurEffect->Begin(&cPasses, 0 );
// 			for (UINT i = 0; i < cPasses; ++i)
// 			{
// 				hr = pCurEffect->BeginPass(i);
// 				//hr = pCurEffect->CommitChanges();
// 				m_pScreenQuad->Render(pCurEffect);
// 				pCurEffect->EndPass();
// 			}	
// 			pCurEffect->End();
// 
// 
// 		}
// 
// 		hr = m_pD3DDevice->SetRenderTarget(0, pOldRT);
// 		SAFE_RELEASE(pOldRT);
// 
// 		D3DPERF_EndEvent();
	}

	void D3D9RenderDevice::SetCamera(Camera* pCamera)
	{
		if (pCamera == NULL)
			return;

		m_pMainCamera = pCamera;
	}

	Camera* D3D9RenderDevice::GetCamera()
	{
		return m_pMainCamera;
	}	

	void D3D9RenderDevice::ClearLightList()
	{
		m_Ligts.clear();
	}

	void D3D9RenderDevice::AddLight(Light* pLigt)
	{
		if (pLigt == NULL)
			return;

		std::vector<Light*>::iterator it = std::find(m_Ligts.begin(),m_Ligts.end(),pLigt);
		if (it != m_Ligts.end())
			return;

		m_Ligts.push_back(pLigt);

		if ( pLigt->IsCreateShadow() )
		{
			ASSERT(m_mainLigt == NULL);
			m_mainLigt = pLigt;
		}
	}

	void D3D9RenderDevice::GetRenderWndSize(int& Width,int& Heigh)
	{
		RECT rect;
		GetWindowRect(m_hWnd,&rect);
		Width = rect.right - rect.left;
		Heigh = rect.bottom - rect.top;
	}

	void D3D9RenderDevice::DrawRenderMesh(RenderMesh* pRenderMesh,Technique* pTech)
	{
		if (pRenderMesh == NULL)
			return;

		HRESULT hr = D3D_OK;

		D3D9VertexDeclaration* d3dvd = (D3D9VertexDeclaration*)pRenderMesh->m_pDeclaration;

		hr = m_pD3DDevice->SetVertexDeclaration(d3dvd->GetD3DVertexDeclaration());
		ASSERT(hr == D3D_OK);

		D3D9IndexBuffer* pIndxBuffer = (D3D9IndexBuffer*)pRenderMesh->m_pIndexBuffer;
		hr = m_pD3DDevice->SetIndices(pIndxBuffer->GetD3DIndexBuffer());
		ASSERT(hr == D3D_OK);

		D3D9VertexBuffer* pVertexBuffer =(D3D9VertexBuffer*)pRenderMesh->m_pVertexBuffers;
		hr = m_pD3DDevice->SetStreamSource(0,pVertexBuffer->GetD3DVertexBuffer(), 0, d3dvd->GetStreanmStride() );

		D3DPRIMITIVETYPE ePrimitiveType = D3D9Mapping::GetD3DPrimitiveType(pRenderMesh->m_ePrimitiveType);
		
		for (UINT i = 0; i < pRenderMesh->m_pMesData->GetSubMeshNumber(); ++i)
		{
			SubMeshData* pSubMesh = pRenderMesh->m_pMesData->GetSubMeshByIndex(i);
			if (pSubMesh == NULL)
				continue;

			UINT nPrimCount = 0;
			if (ePrimitiveType == D3DPT_TRIANGLELIST)
			{
				nPrimCount = pSubMesh->m_nIndexCount / 3;
			}
			else if (ePrimitiveType == D3DPT_TRIANGLESTRIP)
			{
				nPrimCount = pSubMesh->m_nIndexCount - 2;
			}

			HRESULT hr = D3D_OK;
			hr = m_pD3DDevice->DrawIndexedPrimitive(ePrimitiveType,0,pSubMesh->m_nVertexStart,pSubMesh->m_nVertexCount,
				pSubMesh->m_nIndexStart,nPrimCount);
			ASSERT(hr == D3D_OK && "DrawIndexedPrimitive");
		}	
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


// 	void D3D9RenderDevice::Render(Technique* tech, Renderer * obj)
// 	{
// 		ID3DXEffect* pTech = (ID3DXEffect*)tech;
// 
// 		//RenderRegister::Instance()->Reset();
// 
// 
// 		HRESULT hr = D3D_OK;
// 
// 		// draw
// 		VertexStream * vstream = obj->GetVertexStream();
// 		IndexStream * istream = obj->GetIndexStream();
// 
// 		VertexDeclarationPtr decl = vstream->GetDeclaration();
// 		D3D9VertexDeclaration* d3dvd = (D3D9VertexDeclaration*)decl.c_ptr();
// 
// 		hr = mD3DDevice->SetVertexDeclaration(d3dvd->GetD3DVertexDeclaration());
// 
// 		for (int i = 0; i < MAX_VERTEX_STREAM; ++i)
// 		{
// 			int stride = vstream->GetStreamStride(i);
// 			int instances = vstream->GetStreamInstance(i);
// 			VertexBufferPtr vb = vstream->GetStream(i);
// 
// 			if (stride && vb.NotNull())
// 			{
// 				D3D9VertexBuffer * d3dvb = (D3D9VertexBuffer*)vb.c_ptr();
// 				hr = mD3DDevice->SetStreamSource(i, d3dvb->GetD3DVertexBuffer(), 0, stride);
// 			}
// 			else
// 			{
// 				hr = mD3DDevice->SetStreamSource(i, NULL, 0, 0);
// 			}
// 
// 			if (instances > 1)
// 			{
// 				mD3DDevice->SetStreamSourceFreq(i, D3DSTREAMSOURCE_INDEXEDDATA | instances);
// 			}
// 			else if (instances == 1)
// 			{
// 				mD3DDevice->SetStreamSourceFreq(i, D3DSTREAMSOURCE_INSTANCEDATA | 1);
// 			}
// 			else
// 			{
// 				mD3DDevice->SetStreamSourceFreq(i, 1);
// 			}
// 
// 			D3DErrorExceptionFunction(SetStreamSource, hr);
// 		}
// 
// 
// 		IndexBufferPtr ib = istream->GetStream();
// 
// 		int startVertex = vstream->GetStart();
// 		int vertexCount = vstream->GetCount();
// 		int startIndex = istream->GetStart();
// 		int primCount = obj->GetPrimitiveCount();
// 		D3DPRIMITIVETYPE primType = (D3DPRIMITIVETYPE)obj->GetPrimitiveType();
// 
// 		if (ib.NotNull())
// 		{
// 			D3D9IndexBuffer * d3dib = (D3D9IndexBuffer*)(ib.c_ptr());
// 
// 			hr = mD3DDevice->SetIndices(d3dib->GetD3DIndexBuffer());
// 			hr = mD3DDevice->DrawIndexedPrimitive(primType, 0, startVertex, vertexCount, startIndex, primCount);
// 
// 			D3DErrorExceptionFunction(DrawIndexedPrimitive, hr);
// 		}
// 		else
// 		{
// 			hr = mD3DDevice->DrawPrimitive(primType, startVertex, primCount);
// 
// 			D3DErrorExceptionFunction(DrawPrimitive, hr);
// 		}
// 
// 		RenderRegister::Instance()->End();
// 
// 		mBatchCount += 1;
// 		mPrimitivCount += primCount;
// 
// 	}

	//void D3D9RenderDevice::MeshData(RendMesh* pMesh)
	//{
		//m_SolidEntry.push_back(pMesh);

	//	return;

		// 		D3D9RendMesh* pDxMesh = (D3D9RendMesh*)pMesh;
		// 		D3D9Texture* pDxTexure = (D3D9Texture*)(pTexture);
		// 
		// 		D3D9RenderDevice* pRenderDevice = (D3D9RenderDevice*)GetRenderDevice();
		// 		LPDIRECT3DDEVICE9 pDxDevice = pRenderDevice->GetDXDevive();
		// 		//pDxDevice->SetTransform(D3DTS_WORLD,pWordMat);
		// 
		// 		//pDxDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
		// 
		// // 		Matrix4x4 matView,matProject;
		// // 		pDxDevice->GetTransform(D3DTS_VIEW,&matView);
		// // 		pDxDevice->GetTransform(D3DTS_PROJECTION,&matProject);
		//  		Matrix4x4 matWVP = *pWordMat * m_matViewProj;
		// 
		// 		HRESULT hr;
		// 		hr = m_pDefault->SetTexture("g_TextureSrcDiffuse",pDxTexure->GetD3DTexture());
		// 		hr = m_pDefault->SetMatrix("worldviewprojection",&matWVP);
		// 		hr = m_pDefault->SetMatrix("worldview",&m_matView);
		// 
		// 		//HANDLE tech = m_pDefault->GetTechniqueByName( "Shading" );
		// 
		// 		hr = m_pDefault->SetTechnique("Shading");
		// 		UINT cPasses = 0; 
		// 		hr = m_pDefault->Begin(&cPasses, 0 );
		// 		for (UINT i = 0; i < cPasses; ++i)
		// 		{
		// 			hr = m_pDefault->BeginPass(i);
		// 			hr = m_pDefault->CommitChanges();
		// 			pDxMesh->GetMesh()->DrawSubset(0/*nSubInd*/);
		// 			m_pDefault->EndPass();
		// 		}	
		// 		m_pDefault->End();

	//}

	// 	void D3D9Render::RenderSkelMesh(const Matrix4x4* arrSkinMatrix,UINT nSkinMaxtrixNum,
	// 		const Matrix4x4* pWordMat,const RendMesh* pMesh,const Texture* pTexture)
	// 	{
	// 		D3D9RendMesh* pDxMesh = (D3D9RendMesh*)pMesh;
	// 		D3D9Texture* pDxTexure = (D3D9Texture*)(pTexture);
	// 
	// 		Matrix4x4 matBonePalatte[256];
	// 		SubMeshData* pSubMeshData = pDxMesh->GetMeshData()->GetSubMesh(0);
	// 		UINT uBonePaletteSize = pSubMeshData->GetBonePaletteSize();
	// 		for (UINT nCnt = 0; nCnt < uBonePaletteSize; ++nCnt)
	// 		{
	// 			matBonePalatte[nCnt] = arrSkinMatrix[pSubMeshData->GetBonePalette()[nCnt]];
	// 		}
	// 
	// 		D3D9RenderDevice* pRenderDevice = (D3D9RenderDevice*)GetRenderDevice();
	// 		LPDIRECT3DDEVICE9 pDxDevice = pRenderDevice->GetDXDevive();
	// 		pDxDevice->SetTransform(D3DTS_WORLD,pWordMat);
	// 
	// 		//Matrix4x4 matView,matProject;
	// 		//pDxDevice->GetTransform(D3DTS_VIEW,&matView);
	// 		//pDxDevice->GetTransform(D3DTS_PROJECTION,&matProject);
	// 		Matrix4x4 matWVP = *pWordMat * m_matViewProj;
	// 
	// 		HRESULT hr;
	// 		hr = m_pDefaultSkin->SetTexture("g_TextureSrcDiffuse",pDxTexure->GetD3DTexture());
	// 		hr = m_pDefaultSkin->SetMatrix("worldviewprojection",&matWVP);
	// 		hr = m_pDefaultSkin->SetMatrixArray("mSkinMatrixArray",matBonePalatte,uBonePaletteSize);
	// 		hr = m_pDefaultSkin->SetMatrix("worldview",&m_matView);
	// 
	// 		hr = m_pDefaultSkin->SetTechnique("SkinShading");
	// 		UINT cPasses = 0; 
	// 		hr = m_pDefaultSkin->Begin(&cPasses, 0 );
	// 		for (UINT i = 0; i < cPasses; ++i)
	// 		{
	// 			hr = m_pDefaultSkin->BeginPass(i);
	// 			hr = m_pDefaultSkin->CommitChanges();
	// 			pDxMesh->GetMesh()->DrawSubset(0/*nSubInd*/);
	// 			m_pDefaultSkin->EndPass();
	// 		}	
	// 		m_pDefaultSkin->End();
	// 	}


	void D3D9RenderDevice::DrawLine(const Vector3& p0,const Vector3& p1,Uint32 dwColor)
	{
		if (m_pLineRender == NULL)
			return;

		m_pLineRender->DrawLine(p0,p1,dwColor);
	}

	Matrix4x4 D3D9RenderDevice::MakeProjectionMatrix(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf)
	{
		 D3DXMatrixPerspectiveFovLH((D3DXMATRIX*)pOut,fovy,Aspect,zn,zf);
		 return *pOut;
	}

	void D3D9RenderDevice::FlushLine()
	{
		if (m_pLineRender == NULL)
			return;

		m_pLineRender->FlushLine();
	}

}



