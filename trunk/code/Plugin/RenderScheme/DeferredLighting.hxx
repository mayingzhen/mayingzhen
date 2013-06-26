#include "DeferredLighting.h"


namespace ma
{

	void DeferredLighting::Init()
	{
		// For GBuffer
		m_pDepthTex = GetRenderDevice()->CreateRenderTarget();
		m_pDepthTex->Create(-1,-1,FMT_R32F);

		m_pNormalTex = GetRenderDevice()->CreateRenderTarget();
		m_pNormalTex->Create(-1,-1,FMT_A8R8G8B8);

		// Light
		m_pDiffuseTex = GetRenderDevice()->CreateRenderTarget();
		m_pDiffuseTex->Create(-1,-1,D3DFMT_A8R8G8B8);

		m_pSpeculaTex = GetRenderDevice()->CreateRenderTarget();
		m_pSpeculaTex->Create(-1,-1,D3DFMT_A8R8G8B8);
		
	}



	void DeferredLighting::ShadingPass()
	{
		UINT uNumber = m_SolidEntry.size();
		if (uNumber <= 0)
			return;

		for(UINT i = 0; i < uNumber; ++i)
		{
			Renderable* pRenderItem = m_SolidEntry[i];
			if (pRenderItem == NULL)
				continue;

			pRenderItem->m_pMaterial->SetShaderProgram("default","DIFFUSE;DeferredLight");

			GetRenderDevice()->DrawRenderable(pRenderItem);
		}

	}

	void DeferredLighting::GBufferPass()
	{
		UINT uNumber = m_SolidEntry.size();
		if (uNumber <= 0)
			return;

		RenderTarget* preTarget0 = GetRenderDevice()->SetRenderTarget(m_pDepthTex,0);
		RenderTarget* preTarget1 = GetRenderDevice()->SetRenderTarget(m_pNormalTex,1);

		GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0),1.0f,0.0f);

		for(UINT i = 0; i < uNumber; ++i)
		{
			Renderable* pRenderItem = m_SolidEntry[i];
			if (pRenderItem == NULL)
				continue;

			Material* pMaterial = pRenderItem->m_pMaterial;
			ASSERT(pMaterial);
			if (pMaterial == NULL)
				continue;

			const char* pDefine = pMaterial->GetShaderDefine();
			pMaterial->SetShaderProgram("gbuffer",pDefine);

			GetRenderDevice()->DrawRenderable(pRenderItem);
		}

		GetRenderDevice()->SetRenderTarget(preTarget0,0);
		GetRenderDevice()->SetRenderTarget(preTarget1,1);
	}

	void DeferredLighting::DefferdLigtPass()
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

	void DeferredLighting::SetCamera(Camera* pCamera)
	{
		if (pCamera == NULL)
			return;

		m_pMainCamera = pCamera;
	}

	void DeferredLighting::ClearLightList()
	{
		m_Ligts.clear();
	}

	void DeferredLighting::AddLight(Light* pLight)
	{
		m_Ligts.push_back(pLight);
	}

}



