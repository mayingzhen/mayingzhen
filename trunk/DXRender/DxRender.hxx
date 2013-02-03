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

		const char* defaultSkinfx = "../bin/shader/Shading@Skin.fx";
		hr = D3DXCreateEffectFromFile( pRenderDevice->GetDXDevive(),
			defaultSkinfx, NULL, NULL, dwShaderFlags, NULL, &m_pDefaultSkin, NULL );


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

		//pDxDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

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

	void DxRender::RenderSkelMesh(const D3DXMATRIX* arrSkinMatrix,xmUint nSkinMaxtrixNum,
		D3DXMATRIX* pWordMat,IRendMesh* pMesh,IRendTexture* pTexture)
	{
		DxRendMesh* pDxMesh = (DxRendMesh*)pMesh;
		DxRendTexture* pDxTexure = (DxRendTexture*)(pTexture);

		D3DXMATRIX matBonePalatte[256];
		SubMeshData* pSubMeshData = pDxMesh->GetMeshData()->GetSubMesh(0);
		UINT uBonePaletteSize = pSubMeshData->GetBonePaletteSize();
		for (xmUint nCnt = 0; nCnt < uBonePaletteSize; ++nCnt)
		{
			matBonePalatte[nCnt] = arrSkinMatrix[pSubMeshData->GetBonePalette()[nCnt]];
		}

		DxRenderDevice* pRenderDevice = (DxRenderDevice*)GetRenderDevice();
		LPDIRECT3DDEVICE9 pDxDevice = pRenderDevice->GetDXDevive();
		pDxDevice->SetTransform(D3DTS_WORLD,pWordMat);

		D3DXMATRIX matView,matProject;
		pDxDevice->GetTransform(D3DTS_VIEW,&matView);
		pDxDevice->GetTransform(D3DTS_PROJECTION,&matProject);
		D3DXMATRIX matWVP = *pWordMat * matView * matProject;

		HRESULT hr;
		hr = m_pDefaultSkin->SetTexture("g_TextureSrcDiffuse",pDxTexure->GetD3DTexture());
		hr = m_pDefaultSkin->SetMatrix("worldviewprojection",&matWVP);
		hr = m_pDefaultSkin->SetMatrixArray("mSkinMatrixArray",matBonePalatte,uBonePaletteSize);
		hr = m_pDefaultSkin->SetMatrix("worldview",&matView);

		hr = m_pDefaultSkin->SetTechnique("SkinShading");
		UINT cPasses = 0; 
		hr = m_pDefaultSkin->Begin(&cPasses, 0 );
		for (UINT i = 0; i < cPasses; ++i)
		{
			hr = m_pDefaultSkin->BeginPass(i);
			hr = m_pDefaultSkin->CommitChanges();
			pDxMesh->GetD3DXMesh()->DrawSubset(0/*nSubInd*/);
			m_pDefaultSkin->EndPass();
		}	
		m_pDefaultSkin->End();
	}

	void DxRender::DrawBox(const D3DXMATRIX* pWordMat,const D3DXVECTOR3* boxSize, D3DCOLOR color)
	{

	}

	void DxRender::DrawWireSphere(const D3DXMATRIX* pWordMat,float fRadius, D3DCOLOR color)
	{

	}

}


