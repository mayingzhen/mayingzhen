#include "D3DRender.h"
#include "GameApp.h"
#include "OctreeSceneManager.h"
#include "CObject.h"
#include "HDRPostProcess.h"

CD3DRender g_D3DRender;

CD3DRender::CD3DRender()
{
	m_pDepthTex = NULL;
	m_pNormalTex = NULL;
	m_pShadowTex = NULL;
	m_pDiffuseTex = NULL;
	m_pSpeculaTex = NULL;
	m_pSSAOTex = NULL;
	m_pBlurSSAOTex = NULL;
	//m_pJitterTexture = NULL;

	m_bIsHDRRending = false;
}

CD3DRender::~CD3DRender()
{
	SAFE_RELEASE(m_pDepthTex);
	SAFE_RELEASE(m_pNormalTex);
	SAFE_RELEASE(m_pShadowTex);
	SAFE_RELEASE(m_pDiffuseTex);
	SAFE_RELEASE(m_pSpeculaTex);
	SAFE_RELEASE(m_pSSAOTex);
	SAFE_RELEASE(m_pBlurSSAOTex);
	//SAFE_RELEASE(m_pJitterTexture);

	for(int i = 0; i < NUM_PSSM;++i)
	{
		m_ShadowMap[i].Destroy();
	}
}

void CD3DRender::Init()
{
	g_SceneQuad.Create();

	/*
	static const int JITTER_SIZE = 32;
	static const int JITTER_SAMPLES = 8;
	if( FAILED(g_pD3DDevice->CreateVolumeTexture( JITTER_SIZE, JITTER_SIZE, 
		JITTER_SAMPLES*JITTER_SAMPLES/2, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT, &m_pJitterTexture, NULL) ) )
		return ;

	// Build the jitter texture
	D3DLOCKED_BOX lb;
	m_pJitterTexture->LockBox(0, &lb, NULL, 0);

	unsigned char *data = (unsigned char *)lb.pBits;

	for (int i = 0; i<JITTER_SIZE; i++) {
		for (int j = 0; j<JITTER_SIZE; j++) {
			float rot_offset = ((float)rand() / RAND_MAX - 1) * 2 * 3.1415926f;

			for (int k = 0; k<JITTER_SAMPLES*JITTER_SAMPLES/2; k++) {

				int x, y;
				float v[4];

				x = k % (JITTER_SAMPLES / 2);
				y = (JITTER_SAMPLES - 1) - k / (JITTER_SAMPLES / 2);

				v[0] = (float)(x * 2 + 0.5f) / JITTER_SAMPLES;
				v[1] = (float)(y + 0.5f) / JITTER_SAMPLES;
				v[2] = (float)(x * 2 + 1 + 0.5f) / JITTER_SAMPLES;
				v[3] = v[1];

				// jitter
				v[0] += ((float)rand() * 2 / RAND_MAX - 1) / JITTER_SAMPLES;
				v[1] += ((float)rand() * 2 / RAND_MAX - 1) / JITTER_SAMPLES;
				v[2] += ((float)rand() * 2 / RAND_MAX - 1) / JITTER_SAMPLES;
				v[3] += ((float)rand() * 2 / RAND_MAX - 1) / JITTER_SAMPLES;

				// warp to disk
				float d[4];
				d[0] = sqrtf(v[1]) * cosf(2 * 3.1415926f * v[0] + rot_offset);
				d[1] = sqrtf(v[1]) * sinf(2 * 3.1415926f * v[0] + rot_offset);
				d[2] = sqrtf(v[3]) * cosf(2 * 3.1415926f * v[2] + rot_offset);
				d[3] = sqrtf(v[3]) * sinf(2 * 3.1415926f * v[2] + rot_offset);

				d[0] = (d[0] + 1.0) / 2.0;
				data[k*lb.SlicePitch + j*lb.RowPitch + i*4 + 0] = (unsigned char)(d[0] * 255);
				d[1] = (d[1] + 1.0) / 2.0;
				data[k*lb.SlicePitch + j*lb.RowPitch + i*4 + 1] = (unsigned char)(d[1] * 255);
				d[2] = (d[2] + 1.0) / 2.0;
				data[k*lb.SlicePitch + j*lb.RowPitch + i*4 + 2] = (unsigned char)(d[2] * 255);
				d[3] = (d[3] + 1.0) / 2.0;
				data[k*lb.SlicePitch + j*lb.RowPitch + i*4 + 3] = (unsigned char)(d[3] * 255);
			}
		}
	}
	m_pJitterTexture->UnlockBox(0);
*/
}

void CD3DRender::Destroy()
{

}

void CD3DRender::GBufferPass()
{
	HRESULT hr = S_OK;

	PROFILE_LABEL_PUSH("RenderGBuffer");

	LPDIRECT3DSURFACE9 pOldRT0 = NULL;
	g_pD3DDevice->GetRenderTarget(0, &pOldRT0);
	LPDIRECT3DSURFACE9 pOldRT1 = NULL;
	g_pD3DDevice->GetRenderTarget(1, &pOldRT1);

	LPDIRECT3DSURFACE9 pSurfDepth = NULL;
	hr = m_pDepthTex->GetSurfaceLevel(0,&pSurfDepth);
	hr = g_pD3DDevice->SetRenderTarget(0,pSurfDepth);
	SAFE_RELEASE(pSurfDepth);

	LPDIRECT3DSURFACE9 pSurfNormal = NULL;
	hr = m_pNormalTex->GetSurfaceLevel(0,&pSurfNormal);
	hr = g_pD3DDevice->SetRenderTarget(1,pSurfNormal);
	SAFE_RELEASE(pSurfNormal)

	g_pD3DDevice->Clear(0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	D3DXVECTOR4 depth_near_far_invfar = D3DXVECTOR4(g_Camera.m_fNearClip, 
		g_Camera.m_fFarClip, 1 / g_Camera.m_fFarClip, 0 );

	const std::list<CObject*>& ObjList = g_SceneMng.GetRenderObjList();
	//for (int i = 0; i < ObjList.size(); ++i)
	for( std::list<CObject*>::const_iterator it = ObjList.begin(); it != ObjList.end(); ++it )
	{
		CObject* renderObj = *it;
		if (renderObj == NULL)
		{
			continue;
		}

		if (renderObj->m_ObjFlag & FOB_IS_SKIN)
		{
			m_SkinGBufferTech.begin();
			m_SkinGBufferTech.BeginPass(0);
		}
		else
		{
			m_gBufferTech.begin();
			m_gBufferTech.BeginPass(0);
		}

		LPD3DXEFFECT effect = g_pCurCEffect->effect();
		V( effect->SetVector( "depth_near_far_invfar", &depth_near_far_invfar ) );

		renderObj->Render();

		if (renderObj->m_ObjFlag & FOB_IS_SKIN)
		{
			m_SkinGBufferTech.endPass();
			m_SkinGBufferTech.end();
		}
		else
		{
			m_gBufferTech.endPass();
			m_gBufferTech.end();
		}
	}

	hr = g_pD3DDevice->SetRenderTarget(0, pOldRT0);
	SAFE_RELEASE(pOldRT0);
	hr = g_pD3DDevice->SetRenderTarget(1, pOldRT1);
	SAFE_RELEASE(pOldRT1);

	PROFILE_LABEL_POP("RenderGBuffer");

	int test = 0;	
	if (test)
	{
		D3DXSaveTextureToFile("F:\\testZ.bmp",D3DXIFF_BMP,m_pDepthTex,NULL);
		D3DXSaveTextureToFile("F:\\testN.bmp",D3DXIFF_BMP,m_pNormalTex,NULL);
	}
}



float g_fSplitPos[10]; // g_iNumSplits + 1 elements needed

void CD3DRender::ShadowMapPass()
{
	HRESULT hr = S_OK;

	PROFILE_LABEL_PUSH("RenderShadowMap");

	//// create shadow map

	g_Camera.CalculateSplitPositions(g_fSplitPos,NUM_PSSM);

	D3DXVECTOR4 depth_near_far_invfar = D3DXVECTOR4(g_SceneMng.m_Light.m_fNear, 
		g_SceneMng.m_Light.m_fFar, 1 / g_SceneMng.m_Light.m_fFar, 0);

	float fDepthBias = 0.003f;
	g_pD3DDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&fDepthBias);

	for(int i = 0; i < NUM_PSSM; i++)
	{
		// calculate frustum
		CFrustum splitFrustum;
		splitFrustum = g_Camera.CalculateFrustum(g_fSplitPos[i], g_fSplitPos[i+1]);

		// find casters
		//casters = g_Light.FindCasters(splitFrustum); 

		// calculate crop matrix
		D3DXMATRIX mCropMatrix = g_SceneMng.m_Light.CalculateCropMatrix(splitFrustum);

		D3DXMATRIX mProj = g_SceneMng.m_Light.m_mProj * mCropMatrix;

		D3DXMATRIX mSplitViewProj = g_SceneMng.m_Light.m_mView * mProj;

		m_ShadowMap[i].m_TexMat = mSplitViewProj * *m_ShadowMap->GetTexScaleBiasMat();

		g_pD3DDevice->SetTransform(D3DTS_VIEW,&g_SceneMng.m_Light.m_mView);
		g_pD3DDevice->SetTransform(D3DTS_PROJECTION,&mProj);

		m_ShadowMap[i].EnableRendering();

		g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00FFFFFF, 1.0f, 0L);

		//m_pRenderQueue->Render();
		const std::list<CObject*>& ObjList = g_SceneMng.GetRenderObjList();
		//for (int i = 0; i < ObjList.size(); ++i)
		for( std::list<CObject*>::const_iterator it = ObjList.begin(); it != ObjList.end(); ++it )
		{
			CObject* renderObj = *it;
			if (renderObj == NULL || renderObj->m_ObjFlag & FOB_NOT_CAST_SHADOW)
			{
				continue;
			}

			if (renderObj->m_ObjFlag & FOB_IS_SKIN)
			{
				m_SkinShadowMapTech.begin();
				m_SkinShadowMapTech.BeginPass(0);
			}
			else
			{
				m_shadowMapTech.begin();
				m_shadowMapTech.BeginPass(0);
			}	

			LPD3DXEFFECT effect = g_pCurCEffect->effect();
			/*V( */effect->SetVector( "depth_near_far_invfar", &depth_near_far_invfar )/* )*/;

			renderObj->Render();

			if (renderObj->m_ObjFlag & FOB_IS_SKIN)
			{
				m_SkinShadowMapTech.endPass();
				m_SkinShadowMapTech.end();
			}
			else
			{
				m_shadowMapTech.endPass();
				m_shadowMapTech.end();
			}
		}

		m_ShadowMap[i].DisableRendering();
	}

	g_pD3DDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);

	PROFILE_LABEL_POP("RenderShadowMap");

	int test = 0;	
	if (test)
	{
		//D3DXSaveTextureToFile("F:\\shadowmap.bmp",D3DXIFF_BMP,m_pShadowDepthMap,NULL);
	}
}


static D3DVIEWPORT9 _OriginalCameraViewport;
static float _fOriginalCameraNear;
static float _fOriginalCameraFar;
static void SetDepthRange(float fNear, float fFar)
{
	HRESULT hr = D3D_OK;
	D3DVIEWPORT9 CameraViewport;
	g_pD3DDevice->GetViewport(&CameraViewport);

	// store original values
	_OriginalCameraViewport = CameraViewport;
	_fOriginalCameraNear = g_Camera.m_fNearClip;
	_fOriginalCameraFar = g_Camera.m_fFarClip;

	// set new depth value range
	CameraViewport.MinZ = (fNear - g_Camera.m_fNearClip) / (g_Camera.m_fFarClip - g_Camera.m_fNearClip);
	CameraViewport.MaxZ = (fFar - g_Camera.m_fNearClip) / (g_Camera.m_fFarClip - g_Camera.m_fNearClip);
	hr = g_pD3DDevice->SetViewport(&CameraViewport);

	// set new far and near plane
	//g_Camera.m_fNearClip = fNear;
	//g_Camera.m_fFarClip = fFar;
	//g_Camera.CalculateMatrices();
}


// Resets original depth range settings
//
//
static void RestoreDepthRange(void)
{
	g_pD3DDevice->SetViewport(&_OriginalCameraViewport);
	//g_Camera.m_fNearClip = _fOriginalCameraNear;
	//g_Camera.m_fFarClip = _fOriginalCameraFar;
}



void CD3DRender::DefferdShadow()
{
	PROFILE_LABEL_PUSH("DefferdShadow");

	HRESULT hr = S_OK;

	m_DefferedShdowTech.begin();
	m_DefferedShdowTech.BeginPass(0);

	LPD3DXEFFECT effect = m_DefferedShdowTech.effect();

	LPDIRECT3DSURFACE9 pOldRT = NULL;
	g_pD3DDevice->GetRenderTarget(0, &pOldRT);	

	LPDIRECT3DSURFACE9 pSurfShadowTex;
	V( m_pShadowTex->GetSurfaceLevel( 0, &pSurfShadowTex) );
	V( g_pD3DDevice->SetRenderTarget( 0, pSurfShadowTex ) );
	SAFE_RELEASE( pSurfShadowTex );

	g_pD3DDevice->Clear(0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	D3DXVECTOR4 depth_near_far_invfar = D3DXVECTOR4(g_Camera.m_fNearClip, 
		g_Camera.m_fFarClip, 1 / g_Camera.m_fFarClip, g_SceneMng.m_Light.m_fFar );
	V( effect->SetVector( "depth_near_far_invfar", &depth_near_far_invfar ) );

	D3DXMATRIXA16 invView;
	D3DXMatrixInverse(&invView, NULL, &g_Camera.m_mView);

	D3DXMATRIX viwToLightProjArray[NUM_PSSM];
	D3DXMATRIX wordToLightView[NUM_PSSM];

	float viewMaxDepth[NUM_PSSM];

	for (int i = 0; i < NUM_PSSM; ++i) 
	{
		viwToLightProjArray[i] = invView * m_ShadowMap[i].m_TexMat;
		wordToLightView[i] = invView * g_SceneMng.m_Light.m_mView;

		char strTexShadowMap[MAX_PATH] = {0};
		_snprintf(strTexShadowMap, sizeof(strTexShadowMap), "TexShadowMap%d", i);
		V( effect->SetTexture( strTexShadowMap, m_ShadowMap[i].GetDepthTexture() ) );

		viewMaxDepth[i] = g_fSplitPos[i + 1] - g_fSplitPos[i];
	}

	V( effect->SetTexture( "g_TextureSrcPos", m_pDepthTex ) );

	//V( effect->SetTexture("Jitter", m_pJitterTexture) );

	V( effect->SetMatrixArray( "viwToLightProjArray", viwToLightProjArray, NUM_PSSM ) );
	V( effect->SetMatrixArray( "wordLightView", wordToLightView, NUM_PSSM ) );
	V ( effect->SetFloatArray( "splitPos", g_fSplitPos, NUM_PSSM ) );

	if (g_SceneMng.m_Light.m_Type == Light::TYPE_ORTHOGRAPHIC)
	{
		m_DefferedShdowTech.CommitChanges();
		g_SceneQuad.Render();
	}
	else
	{
		D3DXVECTOR3 lightPos = D3DXVECTOR3( g_SceneMng.m_Light.m_vSource.x, g_SceneMng.m_Light.m_vSource.y, g_SceneMng.m_Light.m_vSource.z );
		float lightRadius = g_SceneMng.m_Light.m_fFar - g_SceneMng.m_Light.m_fNear;

		D3DXMATRIX matTrans,matScal,matWorld;
		D3DXMatrixTranslation( &matTrans, lightPos.x, lightPos.y, lightPos.z );
		D3DXMatrixScaling( &matScal, 0.5 * lightRadius, 0.5 * lightRadius, 0.5 * lightRadius );
		matWorld = matScal * matTrans;

		//m_LightModel.Render(&matWorld);
	}

	for (int i = 0; i < NUM_PSSM; ++i) // mutli pass
	{
		g_pD3DDevice->SetTexture( i, NULL );
	}

	m_DefferedShdowTech.endPass();
	m_DefferedShdowTech.end();

	hr = g_pD3DDevice->SetRenderTarget(0, pOldRT);
	SAFE_RELEASE(pOldRT);

	PROFILE_LABEL_POP("DefferdShadow");

	int test = 0;	
	if (test)
	{
		D3DXSaveTextureToFile("F:\\testShadowTex.bmp",D3DXIFF_BMP,m_pShadowTex,NULL);
	}
}


void CD3DRender::DeferredLight()
{
	PROFILE_LABEL_PUSH("DeferredLight");

	HRESULT hr = S_OK;

	LPDIRECT3DSURFACE9 pOldRT0 = NULL;
	g_pD3DDevice->GetRenderTarget(0, &pOldRT0);

	LPDIRECT3DSURFACE9 pOldRT1 = NULL;
	g_pD3DDevice->GetRenderTarget(1, &pOldRT1);

	LPDIRECT3DSURFACE9 pSurfDiffuse = NULL;
	hr = m_pDiffuseTex->GetSurfaceLevel(0,&pSurfDiffuse);
	hr = g_pD3DDevice->SetRenderTarget(0,pSurfDiffuse);
	SAFE_RELEASE(pSurfDiffuse);

	LPDIRECT3DSURFACE9 pSurfSpecula = NULL;
	hr = m_pSpeculaTex->GetSurfaceLevel(0,&pSurfSpecula);
	hr = g_pD3DDevice->SetRenderTarget(1,pSurfSpecula);
	SAFE_RELEASE(pSurfSpecula)

		g_pD3DDevice->Clear(0,NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	// clear do after SetRenderTarget;

	m_DeferredLightTech.begin();
	LPD3DXEFFECT effect = m_DeferredLightTech.effect();

	// Ambient lighting pass (single full screen quad)
	m_DeferredLightTech.BeginPass("AmbientLight");

	m_DeferredLightTech.CommitChanges();

	g_SceneQuad.Render();

	m_DeferredLightTech.endPass();
	///////////////////////////

	V( effect->SetTexture( "g_TextureSrcPos", m_pDepthTex ) );
	V( effect->SetTexture( "g_TextureSrcNormal", m_pNormalTex ) );
	V( effect->SetTexture( "g_TextureShadow", m_pShadowTex ) );

	D3DXVECTOR4 lightColor = D3DXVECTOR4( 2.0f, 2.0f, 2.0f, 1.0f );
	D3DXVECTOR3 lightPos = D3DXVECTOR3( g_SceneMng.m_Light.m_vSource.x, g_SceneMng.m_Light.m_vSource.y, g_SceneMng.m_Light.m_vSource.z );
	float lightRadius = g_SceneMng.m_Light.m_fFar - g_SceneMng.m_Light.m_fNear;

	D3DXMATRIX matTrans,matScal,matWorld;
	D3DXMatrixTranslation( &matTrans, lightPos.x, lightPos.y, lightPos.z );
	D3DXMatrixScaling( &matScal, 0.5 * lightRadius, 0.5 * lightRadius, 0.5 * lightRadius  );
	matWorld = matScal * matTrans;

	D3DXVECTOR4 lightPosView;
	D3DXVec3Transform(&lightPosView, &lightPos, &g_Camera.m_mView);

	V( effect->SetVector( "light_color", &lightColor) );
	V( effect->SetVector( "light_pos_es", &lightPosView ) );
	V( effect->SetFloat( "lightRadius", lightRadius) );
	V( effect->SetFloat( "g_OneOverSqrLightRadius", 1 / (lightRadius * lightRadius) ) );

	D3DXVECTOR4 depth_near_far_invfar = D3DXVECTOR4(g_Camera.m_fNearClip, g_Camera.m_fFarClip, 1 / g_Camera.m_fFarClip, 0 );
	V( effect->SetVector( "depth_near_far_invfar", &depth_near_far_invfar ) );

	///////////
	//  	m_DeferredLightTech.BeginPass("StencilVolumeMask");
	//  	m_LightModel.Render(&matWorld);
	//  	m_DeferredLightTech.endPass();
	///////////

	/////////// 
	//m_DeferredLightTech.BeginPass("DiffuseLightStencil");
	m_DeferredLightTech.BeginPass("DiffuseLight");
	//m_LightModel.Render(&matWorld);

	m_DeferredLightTech.CommitChanges();
	g_SceneQuad.Render();

	m_DeferredLightTech.endPass();
	///////////

	m_DeferredLightTech.end();	

	hr = g_pD3DDevice->SetRenderTarget(0, pOldRT0);
	hr = g_pD3DDevice->SetRenderTarget(1, pOldRT1);
	SAFE_RELEASE(pOldRT0);
	SAFE_RELEASE(pOldRT1);

	PROFILE_LABEL_POP("DeferredLight");

	int test = 0;	
	if (test)
	{
		D3DXSaveTextureToFile("F:\\testDiffuse.bmp",D3DXIFF_BMP,m_pDiffuseTex,NULL);
		D3DXSaveTextureToFile("F:\\testSpecular.bmp",D3DXIFF_BMP,m_pSpeculaTex,NULL);
	}
}

void CD3DRender::ShadingPass()
{
	PROFILE_LABEL_PUSH("ShadingPass");

	HRESULT hr;

	LPDIRECT3DSURFACE9 pOldRT0 = NULL;
	if (m_bIsHDRRending)
	{
		g_pD3DDevice->GetRenderTarget(0, &pOldRT0);
	 
	 	LPDIRECT3DSURFACE9 pSurfShading = NULL;
	 	hr = m_pShdingTex->GetSurfaceLevel(0,&pSurfShading);
	 	hr = g_pD3DDevice->SetRenderTarget(0,pSurfShading);
	 	SAFE_RELEASE(pSurfShading);
	}

	g_pD3DDevice->Clear( 0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	D3DXMATRIX matView = g_Camera.m_mView;  
	D3DXMATRIX matProj = g_Camera.m_mProj;
	g_pD3DDevice->SetTransform(D3DTS_VIEW,&matView);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION,&matProj);

	const std::list<CObject*>& ObjList = g_SceneMng.GetRenderObjList();
	//for (int i = 0; i < ObjList.size(); ++i)
	for( std::list<CObject*>::const_iterator it = ObjList.begin(); it != ObjList.end(); ++it )
	{
		CObject* renderObj = *it;
		if (renderObj == NULL)
		{
			continue;
		}

		if (renderObj->m_ObjFlag & FOB_IS_SKIN)
		{
			m_SkinShadingTech.begin();
			m_SkinShadingTech.BeginPass(0);
		}
		else if (renderObj->m_ObjFlag & FOB_IS_TERRAIN)
		{
			m_TerrainTech.begin();
			m_TerrainTech.BeginPass(0);
		}
		else
		{
			m_ShadingTech.begin();
			m_ShadingTech.BeginPass(0);
		}

		LPD3DXEFFECT effect = g_pCurCEffect->effect();
		V( effect->SetTexture( "g_TextureLightDiffuse", m_pDiffuseTex ) );
		V( effect->SetTexture( "g_TextureLightSpecular", m_pSpeculaTex ) );

		renderObj->Render();

		if (renderObj->m_ObjFlag & FOB_IS_SKIN)
		{
			m_SkinShadingTech.endPass();
			m_SkinShadingTech.end();
		}
		else if (renderObj->m_ObjFlag & FOB_IS_TERRAIN)
		{	
			m_TerrainTech.endPass();
			m_TerrainTech.end();
		}
		else
		{
			m_ShadingTech.endPass();
			m_ShadingTech.end();
		}
	}

	PROFILE_LABEL_POP("ShadingPass");

	if (m_bIsHDRRending)
	{
		g_pD3DDevice->SetRenderTarget(0, pOldRT0);
		SAFE_RELEASE(pOldRT0);

		g_HDRPostProcess.Render(m_pShdingTex,NULL);	
	}
}

void CD3DRender::RenderWater()
{
/*
	for (int i = 0; i < m_pTerrainLiquids.size(); ++i)
	{
		m_pTerrainLiquids[i]->BuildReflectionTexture();
	}

	m_WaterTech.begin();
	m_WaterTech.BeginPass(0);

	if (g_pCurCEffect)
	{
		D3DXMATRIX matView,matProj;
		g_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION,&matProj);
		//matView = g_Camera.m_mView;
		//matProj = g_Camera.m_mProj;
		D3DXMATRIX matWorldView = matView;
		D3DXMATRIX matWorldViewProj = matWorldView * matProj;
		g_pCurCEffect->setMatrix(CEffectFile::k_worldViewMatrix,&matWorldView);
		g_pCurCEffect->setMatrix(CEffectFile::k_worldViewProjMatrix,&matWorldViewProj);
	}

	for (int i = 0; i < m_pTerrainLiquids.size(); ++i)
	{
		m_pTerrainLiquids[i]->render();
	}

	m_WaterTech.endPass();
	m_WaterTech.end();
*/
}


void CD3DRender::RenderAABB()
{
// 	D3DXMATRIX matInit;
// 	D3DXMatrixIdentity(&matInit);
// 
// 	for (int i = 0; i < m_pRenderQueue->m_NormalRQ.size(); ++i)
// 	{
// 		CMapObj* renderObj = (CMapObj*)m_pRenderQueue->m_NormalRQ[i];
// 
// 		D3DXMATRIX matRy,matSc,matTrworld;
// 		D3DXMatrixRotationY(&matRy,renderObj->m_angle);
// 		D3DXMatrixScaling(&matSc,renderObj->m_scale,renderObj->m_scale,renderObj->m_scale);
// 		D3DXMatrixTranslation( &matTrworld, renderObj->m_wordPos.x, renderObj->m_wordPos.y, renderObj->m_wordPos.z );
// 		matTrworld = matRy * matSc * matTrworld;
// 
// 		//renderObj->m_pSkinMesk->GetBoundingBox().Render(renderObj->m_pSkinMesk->GetRootMatrix() * matTrworld);
// 
// 		renderObj->m_WorldAABB.Render(matInit);
// 	}

	// 	for (int y = 0; y < CTerrain::GetSingleton().m_sectorCountY; ++y)
	// 	{
	// 		for (int x = 0; x < CTerrain::GetSingleton().m_sectorCountX; ++x)
	// 		{
	// 			uint16 index = (y * CTerrain::GetSingleton().m_sectorCountX) + x;
	// 
	// 			CTerrain::GetSingleton().m_pSectorArray[index].m_WorldAABB.Render(matInit);
	// 		}
	// 	}

}

void CD3DRender::RenderSSAO()
{
	PROFILE_LABEL_PUSH("RenderSSAO");

	HRESULT hr;

	D3DXVECTOR4 depth_near_far_invfar = D3DXVECTOR4(g_Camera.m_fNearClip, g_Camera.m_fFarClip, 1 / g_Camera.m_fFarClip, 0 );

	LPDIRECT3DSURFACE9 pOldRT0 = NULL;
	g_pD3DDevice->GetRenderTarget(0, &pOldRT0);

	LPDIRECT3DSURFACE9 pSurfSSAO = NULL;
	hr = m_pSSAOTex->GetSurfaceLevel(0,&pSurfSSAO);
	hr = g_pD3DDevice->SetRenderTarget(0,pSurfSSAO);
	SAFE_RELEASE(pSurfSSAO);
	g_pD3DDevice->Clear( 0, NULL,D3DCLEAR_TARGET , D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	m_SSAOTech.begin();
	m_SSAOTech.BeginPass(0);

	LPD3DXEFFECT effect = m_SSAOTech.effect();
	V( effect->SetMatrix( "proj", &g_Camera.m_mProj ) );
	V( effect->SetTexture( "g_TextureSrcPos", m_pDepthTex ) );
	V( effect->SetTexture( "g_TextureSrcNormal", m_pNormalTex ) );
	V( effect->SetVector( "depth_near_far_invfar", &depth_near_far_invfar ) );

	g_SceneQuad.Render();

	m_SSAOTech.endPass();
	m_SSAOTech.end();

	PROFILE_LABEL_POP("RenderSSAO");

	LPDIRECT3DSURFACE9 pSurfBlurTemp = NULL;
	hr = m_pBlurTempTex->GetSurfaceLevel(0,&pSurfBlurTemp);
	hr = g_pD3DDevice->SetRenderTarget(0,pSurfBlurTemp);
	SAFE_RELEASE(pSurfBlurTemp);
	g_pD3DDevice->Clear( 0, NULL,D3DCLEAR_TARGET , D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);


	int kernel_radius_ = 8;
	float multiplier_ = 1;

	float rho = kernel_radius_ / 4.0f;
	float init_g = 1 / sqrt(2.0f * D3DX_PI * rho * rho);
	float f = 1 / (2 * rho * rho);
	float blur_factor = f;
	float sharpness_factor = f;

	float tex_size = (float)g_nWndWidth;
	D3DXVECTOR2 src_tex_size = D3DXVECTOR2(tex_size, 1.0f / tex_size);

	////// blur x ssao tex
	m_BilateralBlurX.begin();
	m_BilateralBlurX.BeginPass(0);

	effect = m_BilateralBlurX.effect();
	V( effect->SetTexture( "g_TextureSrc", m_pSSAOTex ) );
	V( effect->SetFloatArray("src_tex_size", (float*)&src_tex_size, 2) );
	V( effect->SetFloat("init_g", init_g) );
	V( effect->SetFloat("blur_factor", blur_factor) );
	V( effect->SetFloat("sharpness_factor", sharpness_factor) );
	//V( effect->SetVector( "depth_near_far_invfar", &depth_near_far_invfar ) );

	g_SceneQuad.Render();

	m_BilateralBlurX.endPass();
	m_BilateralBlurX.end();

	LPDIRECT3DSURFACE9 pSurfBlurSSAO = NULL;
	hr = m_pBlurSSAOTex->GetSurfaceLevel(0,&pSurfBlurSSAO);
	hr = g_pD3DDevice->SetRenderTarget(0,pSurfBlurSSAO);
	SAFE_RELEASE(pSurfBlurSSAO);
	g_pD3DDevice->Clear( 0, NULL,D3DCLEAR_TARGET , D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	tex_size = (float)g_nWndHeigh;
	src_tex_size = D3DXVECTOR2(tex_size, 1.0f / tex_size);

	////// blur y ssao tex
	m_BilateralBlurY.begin();
	m_BilateralBlurY.BeginPass(0);

	effect = m_BilateralBlurY.effect();
	V( effect->SetTexture( "g_TextureSrc", m_pSSAOTex ) );
	V( effect->SetFloatArray("src_tex_size", (float*)&src_tex_size, 2) );
	V( effect->SetFloat("init_g", init_g) );
	V( effect->SetFloat("blur_factor", blur_factor) );
	V( effect->SetFloat("sharpness_factor", sharpness_factor) );
	//V( effect->SetVector( "depth_near_far_invfar", &depth_near_far_invfar ) );

	g_SceneQuad.Render();

	m_BilateralBlurY.endPass();
	m_BilateralBlurY.end();

	g_pD3DDevice->SetRenderTarget(0, pOldRT0);
	SAFE_RELEASE(pOldRT0);
}


void CD3DRender::CreateRT()
{
	HRESULT  hr  = S_OK;

	hr = g_pD3DDevice->CreateTexture( g_nWndWidth, g_nWndHeigh, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, 
		D3DPOOL_DEFAULT, &m_pDepthTex, NULL);

	hr = g_pD3DDevice->CreateTexture( g_nWndWidth, g_nWndHeigh,1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT, &m_pNormalTex, NULL);

	for(int i = 0; i < NUM_PSSM;++i)
	{
		m_ShadowMap[i].Create(1024, 1024);
	}

	hr = g_pD3DDevice->CreateTexture( g_nWndWidth, g_nWndHeigh, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, 
		D3DPOOL_DEFAULT, &m_pShadowTex, NULL);

	hr = g_pD3DDevice->CreateTexture( g_nWndWidth, g_nWndHeigh, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT, &m_pDiffuseTex, NULL);

	hr = g_pD3DDevice->CreateTexture( g_nWndWidth, g_nWndHeigh, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT, &m_pSpeculaTex, NULL);

	 hr = g_pD3DDevice->CreateTexture( g_nWndWidth, g_nWndHeigh, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, 
	 	D3DPOOL_DEFAULT, &m_pShdingTex, NULL);

	 hr = g_pD3DDevice->CreateTexture( g_nWndWidth, g_nWndHeigh, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8/*D3DFMT_A16B16G16R16F*/,
	 	D3DPOOL_DEFAULT, &m_pSSAOTex, NULL);
	 
	 hr = g_pD3DDevice->CreateTexture( g_nWndWidth, g_nWndHeigh, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8/*D3DFMT_A16B16G16R16F*/,
	 	D3DPOOL_DEFAULT, &m_pBlurSSAOTex, NULL);
	 
	 hr = g_pD3DDevice->CreateTexture( g_nWndWidth, g_nWndHeigh, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8/*D3DFMT_A16B16G16R16F*/,
	 	D3DPOOL_DEFAULT, &m_pBlurTempTex, NULL);

	 //g_HDRPostProcess.CreateRTtexture();	 
}

void CD3DRender::LoadShader()
{
	HRESULT  hr  = S_OK;
	m_gBufferTech.loadResource("shader\\Gbuffer.fx");
	m_SkinGBufferTech.loadResource("Shader\\Gbuffer@Skin.fx");
	m_shadowMapTech.loadResource("shader\\shadowmap.fx");
	m_SkinShadowMapTech.loadResource("shader\\shadowmap@Skin.fx");
	m_DefferedShdowTech.loadResource("shader\\DefferedShadow.fx");
	m_DeferredLightTech.loadResource("shader\\DeferredLight.fx");
	m_ShadingTech.loadResource("shader\\Shading.fx");
	m_SkinShadingTech.loadResource("shader\\Shading@Skin.fx");
	m_TerrainTech.loadResource("shader\\Shading@terrain.fx");

	m_WaterTech.loadResource("shader\\water.fx");

	/// ssao
	m_SSAOTech.loadResource("shader\\SSAO\\SSAO.fx");
	m_BilateralBlurX.loadResource("shader\\SSAO\\BlurX.fx");
	m_BilateralBlurY.loadResource("shader\\SSAO\\BlurY.fx");

	//g_HDRPostProcess.LoadShader();
}