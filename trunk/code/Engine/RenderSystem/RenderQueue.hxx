#include "RenderQueue.h"


namespace ma
{
	std::vector<Renderable*>	RenderQueue::m_SolidEntry;
	std::vector<Renderable*>	RenderQueue::m_TransEntry;
	RenderTarget*				RenderQueue::m_pShadowTex = NULL;
	RenderTarget*				RenderQueue::m_pDepthTex = NULL;
	RenderTarget*				RenderQueue::m_pNormalTex = NULL;
	RenderTarget*				RenderQueue::m_pDiffuse = NULL;
	RenderTarget*				RenderQueue::m_pSpecular = NULL;
	ScreenQuad*					RenderQueue::m_pScreenQuad = NULL;	
	UnitSphere*					RenderQueue::m_pUnitSphere;
	Material*					RenderQueue::m_pMaterDeferred;
	Technique*					RenderQueue::m_pTechGBuffer;
	Technique*					RenderQueue::m_pTechShading;

	std::vector<Light*>			RenderQueue::m_arrLight;
	Camera*						RenderQueue::m_pCamera;
	Sampler*					RenderQueue::m_pDiffuseTex;
	Sampler*					RenderQueue::m_pSpecularTex;
	bool						RenderQueue::m_bUseDeferred = true;

	ShadowMapFrustum*			RenderQueue::m_arrSMF[4];
	Light*						RenderQueue::m_pSun;



	void RenderQueue::Init()
	{
		m_pDepthTex = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_R32F);
		m_pNormalTex = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);
		m_pDiffuse = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);
		m_pSpecular = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);
		

		m_pScreenQuad = new ScreenQuad();
		m_pScreenQuad->Init();

		m_pMaterDeferred = new Material("AMBIENT_LIGHT","DeferredLight");

		m_pDiffuseTex = Sampler::create(m_pDiffuse->GetTexture());
		m_pSpecularTex = Sampler::create(m_pSpecular->GetTexture());

		m_pShadowTex = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_R16F);

		for (UINT i = 0; i < Camera::NUM_PSSM; ++i)
		{
			m_arrSMF[i] = new ShadowMapFrustum();
			m_arrSMF[i]->Init();
		}
	}

	void RenderQueue::AddRenderable(Renderable* pRenderable,bool bTrans)
	{
		if (bTrans)
		{
			m_TransEntry.push_back(pRenderable);
		}
		else
		{
			m_SolidEntry.push_back(pRenderable);
		}
	}

	void RenderQueue::Clear()
	{
		m_SolidEntry.clear();
		m_TransEntry.clear();
	}

	void RenderQueue::GBufferPass()
	{
		RenderTarget* pPreTar1 = GetRenderDevice()->SetRenderTarget(m_pDepthTex,0);
		RenderTarget* pPreTar2 = GetRenderDevice()->SetRenderTarget(m_pNormalTex,1);

		GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

		for (UINT i = 0; i < m_SolidEntry.size(); ++i)
		{
			Material* pMaterial = m_SolidEntry[i]->m_pMaterial;
			pMaterial->SetCurTechnqiue("gbuffer",NULL);
			GetRenderDevice()->DrawRenderable(m_SolidEntry[i]);
		}

		GetRenderDevice()->SetRenderTarget(pPreTar1,0);
		GetRenderDevice()->SetRenderTarget(pPreTar2,1);
	}

	void RenderQueue::DefferedLighting()
	{
		RenderTarget* pPreTar1 = GetRenderDevice()->SetRenderTarget(m_pDiffuse,0);
		RenderTarget* pPreTar2 = GetRenderDevice()->SetRenderTarget(m_pSpecular,1);

		GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

	
		m_pScreenQuad->Render(m_pMaterDeferred);	

		for (UINT i = 0; i < m_arrLight.size(); ++i)
		{
			Light* pLight = m_arrLight[i];
			if (pLight == NULL)
				continue;

			Material::SetAutoBingLight(pLight);

			if (pLight->GetLightType() == LIGHT_DIRECTIONAL)
			{
				//m_pMaterDeferred->SetCurTechnqiue("DirectLight");
				
				//m_pMaterDeferred->GetParameter("");
				m_pScreenQuad->Render(m_pMaterDeferred);		
			}
			else if (pLight->GetLightType() == LIGHT_POINT || pLight->GetLightType() == LIGHT_SPOT)
			{
				//Vector3 camePos = m_pCamera->GetTransform().m_vPos;
				//if (camePos.InLight)
				//{
				//	
				//}

				//m_pMaterDeferred->SetCurTechnqiue("DirectLight");
				//Material::SetAutoBingLight(pLight);
				//m_pMaterDeferred->GetParameter("");
			}

			Material::SetAutoBingLight(NULL);
		}
		

		GetRenderDevice()->SetRenderTarget(pPreTar1,0);
		GetRenderDevice()->SetRenderTarget(pPreTar2,1);
	}

	void RenderQueue::DefferedShading()
	{
		GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

		for (UINT i = 0; i < m_SolidEntry.size(); ++i)
		{
			Material* pMaterial = m_SolidEntry[i]->m_pMaterial;
			pMaterial->SetCurTechnqiue("default","DeferredLight");
			pMaterial->GetParameter("u_textureLightDiffuse")->setSampler(m_pDiffuseTex);
			pMaterial->GetParameter("u_textureLightSpecular")->setSampler(m_pSpecularTex);
			GetRenderDevice()->DrawRenderable(m_SolidEntry[i]);
		}

		for (UINT i = 0; i < m_TransEntry.size(); ++i)
		{
			GetRenderDevice()->DrawRenderable(m_TransEntry[i]);
		}
	}

	void RenderQueue::DeferredRending()
	{
		GBufferPass();
		
		//DeferredShadow();

		DefferedLighting();

		DefferedShading();
	}

	void RenderQueue::ForwarRending()
	{
		GetRenderDevice()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

		for (UINT i = 0; i < m_SolidEntry.size(); ++i)
		{
			Material* pMaterial = m_SolidEntry[i]->m_pMaterial;
			pMaterial->SetCurTechnqiue("default",NULL);
			GetRenderDevice()->DrawRenderable(m_SolidEntry[i]);
		}

		for (UINT i = 0; i < m_TransEntry.size(); ++i)
		{
			Material* pMaterial = m_TransEntry[i]->m_pMaterial;
			pMaterial->SetCurTechnqiue("default",NULL);
			GetRenderDevice()->DrawRenderable(m_TransEntry[i]);
		}
	}

	void RenderQueue::ShadowDepthPass()
	{
		for (UINT i = 0; i < 4; ++i)
		{
			ShadowMapFrustum* pSMF = m_arrSMF[i];
			if (pSMF == NULL)
				continue;

			pSMF->ShadowDepthPass();
		}
	}
    
//#include <stdio.h>

	void RenderQueue::DeferredShadow()
	{
		RenderTarget* pPreRT = GetRenderDevice()->SetRenderTarget(m_pShadowTex);
		
		GetRenderDevice()->ClearBuffer(true,true,true,Color(1,1,1,0),1,0);

		Matrix4x4 invView;
		MatrixInverse(&invView, NULL, &m_pCamera->GetViewMatrix());
		 
		Matrix4x4 viwToLightProjArray[Camera::NUM_PSSM];
		Matrix4x4 wordToLightView[Camera::NUM_PSSM];

		for (UINT i = 0; i < Camera::NUM_PSSM; ++i)
		{
	 		ShadowMapFrustum* pSMF = m_arrSMF[i];
	 		if (pSMF == NULL)
	 			continue;
	
	 		viwToLightProjArray[i] = invView * pSMF->GetViewProjMarix() * pSMF->GetTexScaleBiasMatrix();
	 
	 		char strTexShadowMap[MAX_PATH] = {0};
	 		snprintf(strTexShadowMap, sizeof(strTexShadowMap), "TexShadowMap%d", i);
	 		//hr = pCurEffect->SetTexture( strTexShadowMap, pShadowMap->GetDepthTexture() ) ;
	 	}

		m_pScreenQuad->Render(m_pMaterDeferred);

		
		GetRenderDevice()->SetRenderTarget(pPreRT);
	}


	void RenderQueue::Fulsh()
	{
		ShadowDepthPass();

		if (0/*m_bUseDeferred*/)
		{
			DeferredRending();
		}
		else
		{
			ForwarRending();
		}
	}

	void RenderQueue::SetCamera(Camera* pCamera)
	{	
		m_pCamera = pCamera;
	}

}

