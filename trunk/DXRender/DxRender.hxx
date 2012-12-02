#include "DXRender/DxRender.h"
#include "DXRender/DxRenderDevice.h"

namespace ma
{
	DxRender::DxRender()
	{
		m_pDefault = NULL;
		m_pDefaultSkin = NULL;
// 		DxRenderDevice* pRenderDevice = new DxRenderDevice();
// 		SetRenderDevice(pRenderDevice);
	}

	DxRender::~DxRender()
	{
// 		DxRenderDevice* pRenderDevice = (DxRenderDevice*)GetRenderDevice();
// 		SAFE_DELETE(pRenderDevice);
	}

	void DxRender::InitDefaultShader()
	{
		DxRenderDevice* pRenderDevice = (DxRenderDevice*)GetRenderDevice();
		if (pRenderDevice == NULL || pRenderDevice->GetDXDevive() == NULL)
			return;

		DWORD dwShaderFlags = 0;
#ifdef _DEBUG
		dwShaderFlags |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif // _DEBUG

		const char* defaultfx = "../bin/shader/Shading.fx";
		HRESULT hr = D3DXCreateEffectFromFile( pRenderDevice->GetDXDevive(),
			defaultfx, NULL, NULL, dwShaderFlags, NULL, &m_pDefault, NULL ); 

// 		D3DXEFFECT_DESC EffectDesc;
// 		D3DXHANDLE hTechnique;
// 		D3DXTECHNIQUE_DESC techniqueDesc;
// 		m_pDefault->GetDesc(&EffectDesc); 
// 		hr = m_pDefault->FindNextValidTechnique(NULL, &hTechnique); 
// 		if( FAILED(result) )
// 		{
// 			assert(false);
// 			SAFE_RELEASE(m_pEffect);
// 			return false;
// 		}
// 
// 		hr = m_pDefault->GetTechniqueDesc(hTechnique, &techniqueDesc);
// 		if( FAILED(result) )
// 		{
// 			assert(false);
// 			SAFE_RELEASE(m_pEffect);
// 			return false;
// 		}
// 
// 		for (UINT i = 0; i < techniqueDesc.Passes; ++i)
// 		{
// 			D3DXHANDLE passHandle = m_pDefault->GetPass(hTechnique,i); 
// 
// 			D3DXPASS_DESC passDesc;
// 			hr = m_pDefault->GetPassDesc(passHandle, &passDesc);
// 			int p = 0;
// 		}

	}

	void DxRender::BeginRender()
	{
		DxRenderDevice* pRenderDevice = (DxRenderDevice*)GetRenderDevice();
		pRenderDevice->BeginRender();
		//m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,45,50,170), 1.0f, 0);
		//m_pd3dDevice->BeginScene();
	}

	void DxRender::EndRender()
	{
		DxRenderDevice* pRenderDevice = (DxRenderDevice*)GetRenderDevice();
		pRenderDevice->EndRender();
		//m_pd3dDevice->EndScene();
		//m_pd3dDevice->Present(NULL,NULL,NULL,NULL);
	}

	void DxRender::RenderSkelMesh(IRendMesh* pSkelMesh)
	{

	}
	
	void DxRender::SetViewMatrix(const D3DXMATRIX* viewMatrix)
	{
		DxRenderDevice* pRenderDevice = (DxRenderDevice*)GetRenderDevice();
		LPDIRECT3DDEVICE9 pDxDevice = pRenderDevice->GetDXDevive();
		
		pDxDevice->SetTransform(D3DTS_VIEW,viewMatrix);
	}

	void DxRender::SetProjMatrix(const D3DXMATRIX* projMatrix)
	{
		DxRenderDevice* pRenderDevice = (DxRenderDevice*)GetRenderDevice();
		LPDIRECT3DDEVICE9 pDxDevice = pRenderDevice->GetDXDevive();

		pDxDevice->SetTransform(D3DTS_PROJECTION,projMatrix);
	}

	void DxRender::RenderMesh(D3DXMATRIX* pWordMat,IRendMesh* pMesh,IRendTexture* pTexture)
	{
		DxRendMesh* pDxMesh = (DxRendMesh*)pMesh;
		DxRendTexture* pDxTexure = (DxRendTexture*)(pTexture);

		DxRenderDevice* pRenderDevice = (DxRenderDevice*)GetRenderDevice();
		LPDIRECT3DDEVICE9 pDxDevice = pRenderDevice->GetDXDevive();
		pDxDevice->SetTransform(D3DTS_WORLD,pWordMat);

		D3DXMATRIX matView,matProject;
		pDxDevice->GetTransform(D3DTS_VIEW,&matView);
		pDxDevice->GetTransform(D3DTS_PROJECTION,&matProject);
		D3DXMATRIX matWVP = *pWordMat * matView * matProject;

		HRESULT hr;
		hr = m_pDefault->SetTexture("g_TextureSrcDiffuse",pDxTexure->GetD3DTexture());
		hr = m_pDefault->SetMatrix("worldviewprojection",&matWVP);
		hr = m_pDefault->SetMatrix("worldview",&matView);

		//D3DXHANDLE tech = m_pDefault->GetTechniqueByName( "Shading" );

		hr = m_pDefault->SetTechnique("Shading");
		UINT cPasses = 0; 
		hr = m_pDefault->Begin(&cPasses, 0 );
		for (UINT i = 0; i < cPasses; ++i)
		{
			hr = m_pDefault->BeginPass(i);
			hr = m_pDefault->CommitChanges();
			pDxMesh->GetD3DXMesh()->DrawSubset(0/*nSubInd*/);
			m_pDefault->EndPass();
		}	
		m_pDefault->End();

// 		V( g_pEffect->SetTechnique( pMaterial->hTechnique ) );
// 		V( g_pEffect->Begin( &cPasses, 0 ) );
// 
// 		for( iPass = 0; iPass < cPasses; iPass++ )
// 		{
// 			V( g_pEffect->BeginPass( iPass ) );
// 
// 			// The effect interface queues up the changes and performs them 
// 			// with the CommitChanges call. You do not need to call CommitChanges if 
// 			// you are not setting any parameters between the BeginPass and EndPass.
// 			// V( g_pEffect->CommitChanges() );
// 
// 			// Render the mesh with the applied technique
// 			V( pMesh->DrawSubset( iSubset ) );
// 
// 			V( g_pEffect->EndPass() );
// 		}
// 		V( g_pEffect->End() );

		//ID3DXMesh* pD3DXMesh = pDxMesh->GetD3DXMesh();
		//pDxMesh->Dr	
		//for (xmUint nSubInd = 0; nSubInd < pDxMesh->GetSubMeshNumber(); ++nSubInd)
		//{

			//S3AUTMaterial* pMtl = pMeshDX->GetMaterial(nSubInd);


			//m_meshShader->SetTexture(pTexDX != NULL ? pTexDX->m_pTexDX : NULL);

			//m_meshShader->CommitChanges();

			//pDxMesh->GetD3DXMesh()->DrawSubset(0/*nSubInd*/);
		//}

	}


// 	bool DxRender::Init(HWND hWnd)
// 	{
// 		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
// 		if(NULL == m_pD3D)
// 		{
// 			assert(FALSE && "Direct3DCreate9(D3D_SDK_VERSION)");
// 			return false;
// 		}
// 
// 		RECT rect;
// 		GetWindowRect(hWnd,&rect);
// 
// 		ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
// 		m_d3dpp.Windowed = TRUE;
// 		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
// 		m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
// 		m_d3dpp.EnableAutoDepthStencil = TRUE;
// 		m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
// 		m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
// 		m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Disable vertical synchronization
// 		m_d3dpp.BackBufferWidth = rect.right - rect.left;
// 		m_d3dpp.BackBufferHeight = rect.bottom - rect.top;
// 
// 		UINT AdapterToUse = D3DADAPTER_DEFAULT;
// 		D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;
// 
// #if SHIPPING_VERSION
// 		// When building a shipping version, disable NVPerfHUD (opt-out)
// #else
// 		// Look for 'NVIDIA NVPerfHUD' adapter
// 		// If it is present, override default settings
// 		for (UINT Adapter = 0; Adapter < m_pD3D->GetAdapterCount(); Adapter++)
// 		{
// 			D3DADAPTER_IDENTIFIER9 Identifier;
// 			HRESULT Res;
// 			Res = m_pD3D->GetAdapterIdentifier(Adapter, 0, &Identifier);
// 			if ( strcmp(Identifier.Description, "NVIDIA NVPerfHUD") == 0 )
// 			{
// 				AdapterToUse = Adapter;
// 				DeviceType = D3DDEVTYPE_REF;
// 				//Logger::info(String("Using ") + Identifier.Description + " adapter for debug purposes.");
// 				break;
// 			}
// 		}
// #endif
// 
// 		HRESULT hr = S_OK;
// 		hr = m_pD3D->CreateDevice(AdapterToUse,DeviceType,hWnd,
// 			D3DCREATE_HARDWARE_VERTEXPROCESSING,
// 			&m_d3dpp,&m_pd3dDevice);
// 
// 		if( FAILED(hr) )
// 		{
// 			assert(FALSE && "m_pD3D->CreateDevice()");
// 			return false;
// 		}
// 
// 		return true;
// 	}
}


