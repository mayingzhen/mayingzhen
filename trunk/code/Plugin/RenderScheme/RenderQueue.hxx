#include "RenderQueue.h"


namespace ma
{
	std::vector<Renderable*>	RenderQueue::m_SolidEntry;
	std::vector<Renderable*>	RenderQueue::m_TransEntry;
	RenderTarget*				RenderQueue::m_pDepthTex = NULL;
	RenderTarget*				RenderQueue::m_pNormalTex = NULL;
	RenderTarget*				RenderQueue::m_pDiffuse = NULL;
	RenderTarget*				RenderQueue::m_pSpecular = NULL;
	ScreenQuad*					RenderQueue::m_pScreenQuad = NULL;
	ShadowMapFrustum*			RenderQueue::m_arrSMF[Camera::NUM_PSSM];

	void RenderQueue::Init()
	{
		m_pDepthTex = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_R32F);
		m_pNormalTex = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);
		m_pDiffuse = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);
		m_pSpecular = GetRenderDevice()->CreateRenderTarget(-1,-1,FMT_A8R8G8B8);

		m_pScreenQuad = new ScreenQuad();
		m_pScreenQuad->Init();

		m_pMaterDeferred = new Material("AMBIENT_LIGHT","DeferredLight");
		//Sampler* pDiffuse = Sampler::create(m_pDiffuse->GetTexture());
		//Sampler* pSpeclar = Sampler::create(m_pSpecular->GetTexture());
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

	void RenderQueue::Fulsh()
	{
		for (UINT i = 0; i < m_SolidEntry.size(); ++i)
		{
			GetRenderDevice()->DrawRenderable(m_SolidEntry[i]);
		}

		for (UINT i = 0; i < m_TransEntry.size(); ++i)
		{
			GetRenderDevice()->DrawRenderable(m_TransEntry[i]);
		}
	}

}


