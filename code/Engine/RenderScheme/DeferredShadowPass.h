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

		RefPtr<RenderPass>	m_pShadowPass;

		RefPtr<Technique>	m_pFrustumVolume[4];

		RefPtr<Technique>	m_pFrustumVolumeScale[4];

		RefPtr<Technique>	m_pDefferedShadow[4];

		RefPtr<Technique>	m_pBlendMaterial[4];

		RefPtr<Renderable>	m_pRenderable;

		RefPtr<DirectonalLight> m_ShadowLight;
	};

}

