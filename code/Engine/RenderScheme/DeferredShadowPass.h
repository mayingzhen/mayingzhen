#pragma once

namespace ma
{
	class DeferredShadow : public Referenced
	{
	public:
		DeferredShadow();

		void Init();
	
		void Reset();

		void Render();

		void Shoutdown();

		SamplerState* GetShadowSampler() {return m_pShadowSampler.get();}

	private:
		void CreateSimpleLightFrustumMesh();

	private:
		RefPtr<Texture>		m_pShadowTex;	
		RefPtr<SamplerState> m_pShadowSampler;

		RefPtr<Technique>	m_pFrustumVolume;
		RefPtr<Technique>	m_pDefferedShadow;

		RefPtr<Technique>	m_pBlendMaterial;

		RefPtr<Technique>	m_pScreen;

		RefPtr<Renderable>	m_pRenderable;

		RefPtr<DirectonalLight> m_ShadowLight;

// 		std::vector<DepthStencilState> m_vecPasss1;
// 		std::vector<DepthStencilState> m_vecPasss2;
// 		std::vector<DepthStencilState> m_vecPasss3;
	};

}

