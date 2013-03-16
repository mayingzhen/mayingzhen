#include "D3D9RenderDevice.h"
#include "D3D9RendMesh.h"
#include "D3D9RendTexture.h"

namespace ma
{

	D3D9RenderDevice::D3D9RenderDevice()
	{
		m_pDefault = NULL;
		m_pDefaultSkin = NULL;
		m_pGBufferTech = NULL;
		m_pShadingTech = NULL;

		m_mainLigt = NULL;

		m_pLineRender = new LineRender();
		m_pScreenQuad = new ScreenQuad();

		memset(m_arrShadowMap,0,sizeof(m_arrShadowMap));
	}

	D3D9RenderDevice::~D3D9RenderDevice()
	{
		SAFE_DELETE(m_pLineRender);
		SAFE_DELETE(m_pScreenQuad);
	}

	IRendMesh* D3D9RenderDevice::CreateRendMesh()
	{
		return new D3D9RendMesh();
	}

	IRendTexture* D3D9RenderDevice::CreateRendTexture()
	{
		return new D3D9RendTexture();
	}

	void D3D9RenderDevice::Init(int wndhandle)
	{
		InitD3D9((HWND)wndhandle);

		InitRender();

		m_pLineRender->Init(m_pD3DDevice);

		m_pScreenQuad->Init(m_pD3DDevice);
	}

	void D3D9RenderDevice::InitD3D9(HWND hWnd)
	{
		m_hWnd = hWnd;

		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if(NULL == m_pD3D)
		{
			assert(FALSE && "Direct3DCreate9(D3D_SDK_VERSION)");
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
			assert(FALSE && "m_pD3D->CreateDevice()");
			return ;
		}

		return ;
	}

	void D3D9RenderDevice::InitRender()
	{
		assert(m_pD3DDevice);
		if (m_pD3DDevice == NULL)
			return;

		DWORD dwShaderFlags = 0;
#ifdef _DEBUG
		dwShaderFlags |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif // _DEBUG

		HRESULT hr = D3D_OK;

		hr = D3DXCreateEffectFromFile( m_pD3DDevice, "../Data/shader/Shading.fx",
			NULL, NULL, dwShaderFlags, NULL, &m_pDefault, NULL ); 

		hr = D3DXCreateEffectFromFile( m_pD3DDevice, "../Data/shader/Shading@Skin.fx", 
			NULL, NULL, dwShaderFlags, NULL, &m_pDefaultSkin, NULL );

		hr = D3DXCreateEffectFromFile( m_pD3DDevice, "../Data/shader/Gbuffer.fx", 
			NULL, NULL, dwShaderFlags, NULL, &m_pGBufferTech, NULL);

		hr = D3DXCreateEffectFromFile(m_pD3DDevice, "../Data/shader/DeferredLight.fx", 
			NULL, NULL, dwShaderFlags, NULL, &m_DeferredLightTech, NULL);

		hr = D3DXCreateEffectFromFile(m_pD3DDevice, "../Data/shader/shadowmap.fx", 
			NULL, NULL, dwShaderFlags, NULL, &m_pShdowMapTech, NULL);
		
		hr = D3DXCreateEffectFromFile( m_pD3DDevice, "../Data/shader/Shading.fx", 
			NULL, NULL, dwShaderFlags, NULL, &m_pShadingTech, NULL);

		UINT uBackBuffW = m_d3dpp.BackBufferWidth;
		UINT uBackBuffH = m_d3dpp.BackBufferHeight;

		hr = m_pD3DDevice->CreateTexture( uBackBuffW, uBackBuffH, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, 
			D3DPOOL_DEFAULT, &m_pDepthTex, NULL);
		assert( SUCCEEDED(hr) && "D3DFMT_R32F Render Target 创建失败" );

		hr = m_pD3DDevice->CreateTexture( uBackBuffW, uBackBuffH, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
			D3DPOOL_DEFAULT, &m_pNormalTex, NULL);
		assert( SUCCEEDED(hr) && "D3DFMT_A8R8G8B8 Render Target 创建失败" );

		hr = m_pD3DDevice->CreateTexture( uBackBuffW, uBackBuffW, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, 
			D3DPOOL_DEFAULT, &m_pShadowTex, NULL);
		assert( SUCCEEDED(hr) && "D3DFMT_R16F Render Target 创建失败" );

		hr = m_pD3DDevice->CreateTexture( uBackBuffW, uBackBuffW, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
			D3DPOOL_DEFAULT, &m_pDiffuseTex, NULL);
		assert( SUCCEEDED(hr) && "D3DFMT_A8R8G8B8 Render Target 创建失败" );

		hr = m_pD3DDevice->CreateTexture( uBackBuffW, uBackBuffW, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
			D3DPOOL_DEFAULT, &m_pSpeculaTex, NULL);
		assert( SUCCEEDED(hr) && "D3DFMT_A8R8G8B8 Render Target 创建失败" );
	}

	void D3D9RenderDevice::BeginRender()
	{
		m_pD3DDevice->BeginScene();

		//m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,45,50,170), 1.0f, 0);
	}

	void D3D9RenderDevice::EndRender()
	{
		FlushRenderQueue();

		FlushLine();

		m_pD3DDevice->EndScene();
		m_pD3DDevice->Present(NULL,NULL,NULL,NULL);
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
		UINT uNumber = m_SolidEntry.size();
		if (uNumber <= 0)
			return;

		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"ShadingPass");

		HRESULT hr = S_OK;

		m_pD3DDevice->Clear( 0, NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
			D3DCOLOR_ARGB(0,45,50,170), 1.0f, 0);

		for(UINT i = 0; i < uNumber; ++i)
		{
			IRenderItem* pRenderItem = m_SolidEntry[i];
			D3D9RendMesh* pDxMesh = (D3D9RendMesh*)pRenderItem->m_pMesh;
			D3D9RendTexture* pDxTexture = (D3D9RendTexture*)pRenderItem->m_pTex;

			ID3DXEffect* pCurEffect = m_pShadingTech;

			if (pRenderItem->m_nSkinMatrixNum != 0)
			{
				hr = pCurEffect->SetTechnique("SkinShadinTech");
				hr = pCurEffect->SetMatrixArray("mSkinMatrixArray",pRenderItem->m_arrSkinMatrix,pRenderItem->m_nSkinMatrixNum);
			}
			else
			{
				hr = pCurEffect->SetTechnique("Shading");
			}

			D3DXMATRIX matWVP = *(pRenderItem->m_pMatWorld) * m_pMainCamera->GetViewProjMatrix();
			D3DXMATRIX matWV = *(pRenderItem->m_pMatWorld) * m_pMainCamera->GetViewMatrix();
			hr = pCurEffect->SetMatrix("worldviewprojection",&matWVP);
			hr = pCurEffect->SetMatrix("worldview",&matWV);

			hr = pCurEffect->SetTexture("g_TextureSrcDiffuse",pDxTexture->GetD3DTexture());
			hr = pCurEffect->SetTexture( "g_TextureLightDiffuse", m_pDiffuseTex );
			hr = pCurEffect->SetTexture( "g_TextureLightSpecular", m_pSpeculaTex ); 

			UINT cPasses = 0; 
			hr = pCurEffect->Begin(&cPasses, 0 );
			for (UINT i = 0; i < cPasses; ++i)
			{
				hr = pCurEffect->BeginPass(i);
				hr = pCurEffect->CommitChanges();
				pDxMesh->GetD3DXMesh()->DrawSubset(0);
				pCurEffect->EndPass();
			}	
			pCurEffect->End();
		}

		D3DPERF_EndEvent();
	}

	void D3D9RenderDevice::GBufferPass()
	{
		UINT uNumber = m_SolidEntry.size();
		if (uNumber <= 0)
			return;

		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"GBufferPass");

		HRESULT hr = S_OK;

		LPDIRECT3DSURFACE9 pOldRT0 = NULL;
		m_pD3DDevice->GetRenderTarget(0, &pOldRT0);
		LPDIRECT3DSURFACE9 pOldRT1 = NULL;
		m_pD3DDevice->GetRenderTarget(1, &pOldRT1);

		LPDIRECT3DSURFACE9 pSurfDepth = NULL;
		hr = m_pDepthTex->GetSurfaceLevel(0,&pSurfDepth);
		hr = m_pD3DDevice->SetRenderTarget(0,pSurfDepth);
		SAFE_RELEASE(pSurfDepth);

		LPDIRECT3DSURFACE9 pSurfNormal = NULL;
		hr = m_pNormalTex->GetSurfaceLevel(0,&pSurfNormal);
		hr = m_pD3DDevice->SetRenderTarget(1,pSurfNormal);
		SAFE_RELEASE(pSurfNormal)

		m_pD3DDevice->Clear(0, NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		float fNearClip = m_pMainCamera->GetNearClip();
		float fFarClip = m_pMainCamera->GetFarClip();
		D3DXVECTOR4 depth_near_far_invfar = D3DXVECTOR4(fNearClip, fFarClip, 1 / fFarClip, 0);

		for(UINT i = 0; i < uNumber; ++i)
		{
			IRenderItem* pRenderItem = m_SolidEntry[i];
			if (pRenderItem == NULL)
				continue;

			D3D9RendMesh* pD3D9RenderMesh = (D3D9RendMesh*)pRenderItem->m_pMesh; 

			ID3DXEffect* pCurEffect = m_pGBufferTech;

			if (pRenderItem->m_nSkinMatrixNum != 0)
			{
				hr = pCurEffect->SetTechnique("SkinGBufferTech");
				hr = pCurEffect->SetMatrixArray("mSkinMatrixArray",pRenderItem->m_arrSkinMatrix,pRenderItem->m_nSkinMatrixNum);
			}
			else
			{
				hr = pCurEffect->SetTechnique("GBufferTech");
			}

			D3DXMATRIX matWVP = *(pRenderItem->m_pMatWorld) * m_pMainCamera->GetViewProjMatrix();
			D3DXMATRIX matWV = *(pRenderItem->m_pMatWorld) * m_pMainCamera->GetViewMatrix();
			hr = pCurEffect->SetMatrix("worldviewprojection",&matWVP);
			hr = pCurEffect->SetMatrix("worldview",&matWV);
			hr = pCurEffect->SetVector("depth_near_far_invfar",&depth_near_far_invfar);

			UINT cPasses = 0; 
			hr = pCurEffect->Begin(&cPasses, 0 );
			for (UINT i = 0; i < cPasses; ++i)
			{
				hr = pCurEffect->BeginPass(i);
				hr = pCurEffect->CommitChanges();
				pD3D9RenderMesh->GetD3DXMesh()->DrawSubset(0);
				pCurEffect->EndPass();
			}	
			pCurEffect->End();
		}

		hr = m_pD3DDevice->SetRenderTarget(0, pOldRT0);
		SAFE_RELEASE(pOldRT0);
		hr = m_pD3DDevice->SetRenderTarget(1, pOldRT1);
		SAFE_RELEASE(pOldRT1);

		D3DPERF_EndEvent();
	}

	void D3D9RenderDevice::ShadowMapPass()
	{
		if (m_mainLigt == NULL || m_pMainCamera == NULL)
			return;

		HRESULT hr = D3D_OK;

		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"ShadowMapPass");

		float fDepthBias = 0.002f;
		m_pD3DDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&fDepthBias);

		UINT uNumber = m_SolidEntry.size();
		if (uNumber <= 0)
			return;

		for (UINT i = 0; i != 4; ++i)
		{
			ShadowMap* pShadowMap = m_arrShadowMap[i];
			if(pShadowMap == NULL)
			{
				m_arrShadowMap[i] = new ShadowMap();
				pShadowMap = m_arrShadowMap[i];
				pShadowMap->Init(m_pD3DDevice);
			}

			pShadowMap->CalculateMatrix(m_pMainCamera,i,m_mainLigt);

			pShadowMap->ClearCaster();
			pShadowMap->FindCaster((IRenderItem**)&m_SolidEntry[0],m_SolidEntry.size());
			pShadowMap->FindCaster((IRenderItem**)&m_TransEntry[0],m_TransEntry.size());


			pShadowMap->BeginRender();

			m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00FFFFFF, 1.0f, 0L);

			UINT nCaster = pShadowMap->GetCasterNumber();
			for(UINT j = 0; j < uNumber; ++j)
			{
				IRenderItem* pRenderItem = pShadowMap->GetCasterByIndex(j);
				if (pRenderItem == NULL)
					continue;

				D3D9RendMesh* pD3D9RenderMesh = (D3D9RendMesh*)pRenderItem->m_pMesh; 

				ID3DXEffect* pCurEffect = m_pGBufferTech;

				if (pRenderItem->m_nSkinMatrixNum != 0)
				{
					hr = pCurEffect->SetTechnique("SkinGBufferTech");
					hr = pCurEffect->SetMatrixArray("mSkinMatrixArray",pRenderItem->m_arrSkinMatrix,pRenderItem->m_nSkinMatrixNum);
				}
				else
				{
					hr = pCurEffect->SetTechnique("GBufferTech");
				}

				D3DXMATRIX matWVP = *(pRenderItem->m_pMatWorld) * m_pMainCamera->GetViewProjMatrix();
				D3DXMATRIX matWV = *(pRenderItem->m_pMatWorld) * m_pMainCamera->GetViewMatrix();
				pCurEffect->SetMatrix("worldviewprojection",&matWVP);
				pCurEffect->SetMatrix("worldview",&matWV);

				UINT cPasses = 0; 
				hr = pCurEffect->Begin(&cPasses, 0 );
				for (UINT i = 0; i < cPasses; ++i)
				{
					hr = pCurEffect->BeginPass(i);
					hr = pCurEffect->CommitChanges();
					pD3D9RenderMesh->GetD3DXMesh()->DrawSubset(0);
					pCurEffect->EndPass();
				}	
				pCurEffect->End();
			}

			pShadowMap->EndRender();	
		}

		m_pD3DDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);

		D3DPERF_EndEvent();
	}

	void D3D9RenderDevice::DefferdLigtPass()
	{
		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"DeferredLight");

		HRESULT hr = S_OK;

		LPDIRECT3DSURFACE9 pOldRT0 = NULL;
		m_pD3DDevice->GetRenderTarget(0, &pOldRT0);

		LPDIRECT3DSURFACE9 pOldRT1 = NULL;
		m_pD3DDevice->GetRenderTarget(1, &pOldRT1);

		LPDIRECT3DSURFACE9 pSurfDiffuse = NULL;
		hr = m_pDiffuseTex->GetSurfaceLevel(0,&pSurfDiffuse);
		hr = m_pD3DDevice->SetRenderTarget(0,pSurfDiffuse);
		SAFE_RELEASE(pSurfDiffuse);

		LPDIRECT3DSURFACE9 pSurfSpecula = NULL;
		hr = m_pSpeculaTex->GetSurfaceLevel(0,&pSurfSpecula);
		hr = m_pD3DDevice->SetRenderTarget(1,pSurfSpecula);
		SAFE_RELEASE(pSurfSpecula)

		m_pD3DDevice->Clear(0,NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);


		ID3DXEffect* pCurEffect = m_DeferredLightTech;

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

		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"DiffuseLight");

		for (UINT i = 0; i < m_Ligts.size(); ++i)
		{
			Light* pLight = m_Ligts[i];
			if (pLight == NULL)
				continue;

			hr = pCurEffect->SetTechnique("DiffuseLight");

			hr = pCurEffect->SetTexture( "g_TextureSrcPos", m_pDepthTex );
			hr = pCurEffect->SetTexture( "g_TextureSrcNormal", m_pNormalTex ) ;
			hr = pCurEffect->SetTexture( "g_TextureShadow", m_pShadowTex );

			//D3DXVECTOR4 lightColor =  pLight->GetDiffuse(); //
			D3DXVECTOR4 lightColor = D3DXVECTOR4( 2.0f, 2.0f, 2.0f, 1.0f );
			D3DXVECTOR3 lightPos = pLight->GetPositionWS();//D3DXVECTOR3( lightIt->m_vSource.x, lightIt->m_vSource.y, lightIt->m_vSource.z );
			float lightRadius =  pLight->GetRadius();
			D3DXVECTOR3 lightDir = pLight->GetDir();

			D3DXMATRIX matTrans,matScal,matWorld;
			D3DXMatrixTranslation( &matTrans, lightPos.x, lightPos.y, lightPos.z );
			D3DXMatrixScaling( &matScal, 0.5f * lightRadius, 0.5f * lightRadius, 0.5f * lightRadius  );
			matWorld = matScal * matTrans;

			D3DXVec3TransformCoord(&lightPos, &lightPos, &m_pMainCamera->GetViewMatrix());
			D3DXVec3TransformNormal(&lightDir, &lightDir, &m_pMainCamera->GetViewMatrix());
			D3DXVECTOR4 lightPosView(lightPos.x,lightPos.y,lightPos.z,1);
			D3DXVECTOR4 ligtDirView(lightDir.x,lightDir.y,lightDir.z,0);


			hr = pCurEffect->SetVector( "light_color", &lightColor );
			hr = pCurEffect->SetVector( "light_pos_es", &lightPosView );
			hr = pCurEffect->SetVector( "light_dir_es", &ligtDirView );
			hr = pCurEffect->SetFloat( "lightRadius", lightRadius) ;
			hr = pCurEffect->SetFloat( "g_OneOverSqrLightRadius", 1 / (lightRadius * lightRadius) ) ;

			float fNearClip = m_pMainCamera->GetNearClip();
			float fFarClip = m_pMainCamera->GetFarClip();
			D3DXVECTOR4 depth_near_far_invfar = D3DXVECTOR4(fNearClip, fFarClip, 1 / fFarClip, 0 );
			hr = pCurEffect->SetVector( "depth_near_far_invfar", &depth_near_far_invfar );

			///////////
			//  	m_DeferredLightTech.BeginPass("StencilVolumeMask");
			//  	m_LightModel.Render(&matWorld);
			//  	m_DeferredLightTech.endPass();
			///////////


			UINT cPasses = 0; 
			hr = pCurEffect->Begin(&cPasses, 0 );
			for (UINT i = 0; i < cPasses; ++i)
			{
				hr = pCurEffect->BeginPass(i);
				//hr = pCurEffect->CommitChanges();
				m_pScreenQuad->Render(pCurEffect);
				pCurEffect->EndPass();
			}	
			pCurEffect->End();

		}

		D3DPERF_EndEvent();


		hr = m_pD3DDevice->SetRenderTarget(0, pOldRT0);
		hr = m_pD3DDevice->SetRenderTarget(1, pOldRT1);
		SAFE_RELEASE(pOldRT0);
		SAFE_RELEASE(pOldRT1);

		D3DPERF_EndEvent();	
	}

	void D3D9RenderDevice::DefferdShadowPass()
	{
// 		PROFILE_LABEL_PUSH("DefferdShadow");
// 
// 		HRESULT hr = S_OK;
// 
// 		LPDIRECT3DSURFACE9 pOldRT = NULL;
// 		m_pD3DDevice->GetRenderTarget(0, &pOldRT);	
// 
// 		LPDIRECT3DSURFACE9 pSurfShadowTex;
// 		V( m_pShadowTex->GetSurfaceLevel( 0, &pSurfShadowTex) );
// 		V( m_pD3DDevice->SetRenderTarget( 0, pSurfShadowTex ) );
// 		SAFE_RELEASE( pSurfShadowTex );
// 
// 		m_pD3DDevice->Clear(0, NULL,
// 			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
// 			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
// 
// 		D3DXMATRIXA16 invView;
// 		D3DXMatrixInverse(&invView, NULL, &g_Camera.m_mView);
// 
// 		const std::list<Light>& lightList = g_SceneMng.GetLigtList();
// 		std::list<Light>::const_iterator lightIt = lightList.begin();
// 		for (; lightIt != lightList.end(); ++lightIt)
// 		{
// 			m_DefferedShdowTech.begin();
// 			m_DefferedShdowTech.BeginPass(0);
// 
// 			LPD3DXEFFECT effect = m_DefferedShdowTech.effect();
// 
// 			D3DXVECTOR4 depth_near_far_invfar = D3DXVECTOR4(g_Camera.m_fNearClip, 
// 				g_Camera.m_fFarClip, 1 / g_Camera.m_fFarClip, lightIt->m_fFar );
// 			V( effect->SetVector( "depth_near_far_invfar", &depth_near_far_invfar ) );
// 
// 			D3DXMATRIX viwToLightProjArray[CCamera::NUM_PSSM];
// 			D3DXMATRIX wordToLightView[CCamera::NUM_PSSM];
// 			for (int i = 0; i < CCamera::NUM_PSSM; ++i) 
// 			{
// 				ShadowMap* pShadowMap = lightIt->m_pShadowMap[i];
// 				if (pShadowMap == NULL)
// 					continue;
// 
// 				viwToLightProjArray[i] = invView * pShadowMap->m_TexMat;
// 				wordToLightView[i] = invView * pShadowMap->m_viewMat;
// 
// 				char strTexShadowMap[MAX_PATH] = {0};
// 				_snprintf(strTexShadowMap, sizeof(strTexShadowMap), "TexShadowMap%d", i);
// 				V( effect->SetTexture( strTexShadowMap, pShadowMap->GetDepthTexture() ) );
// 			}
// 
// 			V( effect->SetTexture( "g_TextureSrcPos", m_pDepthTex ) );
// 
// 			//V( effect->SetTexture("Jitter", m_pJitterTexture) );
// 
// 			V( effect->SetMatrixArray( "viwToLightProjArray", viwToLightProjArray, CCamera::NUM_PSSM ) );
// 			V( effect->SetMatrixArray( "wordLightView", wordToLightView, CCamera::NUM_PSSM ) );
// 			V ( effect->SetFloatArray( "splitPos", g_Camera.m_fSplitPos, CCamera::NUM_PSSM ) );
// 
// 			if (lightIt->m_Type == Light::TYPE_ORTHOGRAPHIC)
// 			{
// 				m_DefferedShdowTech.CommitChanges();
// 				g_SceneQuad.Render();
// 			}
// 			else
// 			{
// 				D3DXVECTOR3 lightPos = D3DXVECTOR3( lightIt->m_vSource.x, lightIt->m_vSource.y, lightIt->m_vSource.z );
// 				float lightRadius = lightIt->m_fFar - lightIt->m_fNear;
// 
// 				D3DXMATRIX matTrans,matScal,matWorld;
// 				D3DXMatrixTranslation( &matTrans, lightPos.x, lightPos.y, lightPos.z );
// 				D3DXMatrixScaling( &matScal, 0.5f * lightRadius, 0.5f * lightRadius, 0.5f * lightRadius );
// 				matWorld = matScal * matTrans;
// 
// 				//m_LightModel.Render(&matWorld);
// 			}
// 
// 			for (int i = 0; i < CCamera::NUM_PSSM; ++i) // mutli pass
// 			{
// 				m_pD3DDevice->SetTexture( i, NULL );
// 			}
// 
// 			m_DefferedShdowTech.endPass();
// 			m_DefferedShdowTech.end();
// 
// 		}
// 
// 		hr = m_pD3DDevice->SetRenderTarget(0, pOldRT);
// 		SAFE_RELEASE(pOldRT);
// 
// 		PROFILE_LABEL_POP("DefferdShadow");
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
			assert(m_mainLigt == NULL);
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

	void D3D9RenderDevice::RenderMesh(IRendMesh* pMesh)
	{
		m_SolidEntry.push_back(pMesh);

		return;

		// 		D3D9RendMesh* pDxMesh = (D3D9RendMesh*)pMesh;
		// 		D3D9RendTexture* pDxTexure = (D3D9RendTexture*)(pTexture);
		// 
		// 		D3D9RenderDevice* pRenderDevice = (D3D9RenderDevice*)GetRenderDevice();
		// 		LPDIRECT3DDEVICE9 pDxDevice = pRenderDevice->GetDXDevive();
		// 		//pDxDevice->SetTransform(D3DTS_WORLD,pWordMat);
		// 
		// 		//pDxDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
		// 
		// // 		D3DXMATRIX matView,matProject;
		// // 		pDxDevice->GetTransform(D3DTS_VIEW,&matView);
		// // 		pDxDevice->GetTransform(D3DTS_PROJECTION,&matProject);
		//  		D3DXMATRIX matWVP = *pWordMat * m_matViewProj;
		// 
		// 		HRESULT hr;
		// 		hr = m_pDefault->SetTexture("g_TextureSrcDiffuse",pDxTexure->GetD3DTexture());
		// 		hr = m_pDefault->SetMatrix("worldviewprojection",&matWVP);
		// 		hr = m_pDefault->SetMatrix("worldview",&m_matView);
		// 
		// 		//D3DXHANDLE tech = m_pDefault->GetTechniqueByName( "Shading" );
		// 
		// 		hr = m_pDefault->SetTechnique("Shading");
		// 		UINT cPasses = 0; 
		// 		hr = m_pDefault->Begin(&cPasses, 0 );
		// 		for (UINT i = 0; i < cPasses; ++i)
		// 		{
		// 			hr = m_pDefault->BeginPass(i);
		// 			hr = m_pDefault->CommitChanges();
		// 			pDxMesh->GetD3DXMesh()->DrawSubset(0/*nSubInd*/);
		// 			m_pDefault->EndPass();
		// 		}	
		// 		m_pDefault->End();

	}

	// 	void D3D9Render::RenderSkelMesh(const D3DXMATRIX* arrSkinMatrix,UINT nSkinMaxtrixNum,
	// 		const D3DXMATRIX* pWordMat,const IRendMesh* pMesh,const IRendTexture* pTexture)
	// 	{
	// 		D3D9RendMesh* pDxMesh = (D3D9RendMesh*)pMesh;
	// 		D3D9RendTexture* pDxTexure = (D3D9RendTexture*)(pTexture);
	// 
	// 		D3DXMATRIX matBonePalatte[256];
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
	// 		//D3DXMATRIX matView,matProject;
	// 		//pDxDevice->GetTransform(D3DTS_VIEW,&matView);
	// 		//pDxDevice->GetTransform(D3DTS_PROJECTION,&matProject);
	// 		D3DXMATRIX matWVP = *pWordMat * m_matViewProj;
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
	// 			pDxMesh->GetD3DXMesh()->DrawSubset(0/*nSubInd*/);
	// 			m_pDefaultSkin->EndPass();
	// 		}	
	// 		m_pDefaultSkin->End();
	// 	}



	void D3D9RenderDevice::DrawLine(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,xmUint32 dwColor)
	{
		if (m_pLineRender == NULL)
			return;

		m_pLineRender->DrawLine(p0,p1,dwColor);
	}

	void D3D9RenderDevice::FlushLine()
	{
		if (m_pLineRender == NULL)
			return;

		m_pLineRender->FlushLine();
	}

}



