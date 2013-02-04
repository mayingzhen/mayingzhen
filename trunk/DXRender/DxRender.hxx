#include "DXRender/DxRender.h"
#include "DXRender/DxRenderDevice.h"

namespace ma
{
	//namespace S3DebugRendererHelper
	//{
		enum {LINE_BATCH_MAX = 2048};


// 		//------------------------------------------------------------------------------
// 		//file scope variable
// 		//------------------------------------------------------------------------------
		const static D3DVERTEXELEMENT9 gs_primtiveVBElem[] = 
		{
			{ 0, 0,     D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
			{ 0, 3 * 4, D3DDECLTYPE_FLOAT4,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,		0 },
			{ 0, 7 * 4, D3DDECLTYPE_FLOAT1,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  0 },
			D3DDECL_END()
		};


		struct PrimitiveVertex
		{
			D3DXVECTOR3 pos;
			D3DXCOLOR   col;
			float instInd;//instance index
		};

		PrimitiveVertex gsLineBatch[LINE_BATCH_MAX*2];
		int				gsLineBatchSize = 0;


	//}//namespace S3ADebugRendererImpl

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

		const char* pszLineShader = "../bin/shader/ColorVertex.fx";
		hr = D3DXCreateEffectFromFile( pRenderDevice->GetDXDevive(), 
			pszLineShader, NULL, NULL, dwShaderFlags, NULL, &m_pLineShader, NULL);


		pRenderDevice->GetDXDevive()->CreateVertexDeclaration(gs_primtiveVBElem,&m_pPrimitiveVBDesc);
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
		FlushLine();

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

	void DxRender::RenderMesh(const D3DXMATRIX* pWordMat,const IRendMesh* pMesh,const IRendTexture* pTexture)
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
		const D3DXMATRIX* pWordMat,const IRendMesh* pMesh,const IRendTexture* pTexture)
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

	void RGBToColor(D3DXCOLOR* col,xmUint32 dwARGB)
	{
		col->a = ((dwARGB>>24)&0xff)/255.0f;
		col->r = ((dwARGB>>16)&0xff)/255.0f;
		col->g = ((dwARGB>> 8)&0xff)/255.0f;
		col->b = ((dwARGB>> 0)&0xff)/255.0f;
	}

	void DxRender::DrawLine(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,xmUint32 dwColor)
	{
		if (gsLineBatchSize < LINE_BATCH_MAX)
		{
			PrimitiveVertex& v0 = gsLineBatch[gsLineBatchSize * 2];
			PrimitiveVertex& v1 = gsLineBatch[gsLineBatchSize * 2 + 1];

			RGBToColor(&v0.col,dwColor);
			RGBToColor(&v1.col,dwColor);
			v0.pos = p0;
			v1.pos = p1;
			++gsLineBatchSize;
		}
	}

	void DxRender::FlushLine()
	{
		D3DPERF_BeginEvent(D3DCOLOR_RGBA(255,0,0,255),L"FlushLine");

		DxRenderDevice* pRenderDevice = (DxRenderDevice*)GetRenderDevice();
		LPDIRECT3DDEVICE9 pDxDevice = pRenderDevice->GetDXDevive();

		pDxDevice->SetVertexDeclaration( m_pPrimitiveVBDesc );

		D3DXMATRIX matView,matProject;
		pDxDevice->GetTransform(D3DTS_VIEW,&matView);
		pDxDevice->GetTransform(D3DTS_PROJECTION,&matProject);
		D3DXMATRIX matVP = matView * matProject;

		m_pLineShader->SetTechnique("LineShader");
		m_pLineShader->SetMatrix( "gmVP", (const D3DXMATRIX*)&matVP);
		
		pDxDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_ALWAYS);

		UINT cPasses = 0; 
		m_pLineShader->Begin(&cPasses, 0 );
		for (UINT i = 0; i < cPasses; ++i)
		{
			m_pLineShader->BeginPass(i);
			m_pLineShader->CommitChanges();
			pDxDevice->DrawPrimitiveUP(D3DPT_LINELIST,gsLineBatchSize,(void*)&gsLineBatch,sizeof(PrimitiveVertex));
			m_pLineShader->EndPass();
		}	
		m_pLineShader->End();

		pDxDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESS);

		gsLineBatchSize = 0;

		D3DPERF_EndEvent();

	}
}

