#pragma once

namespace ma
{

	class SceneContext;

	struct SMFrustumInfo
	{
		Matrix4 m_matFrustum;
		Matrix4	m_matLightViewProj;
		Matrix4	m_matShadow;
		SamplerState* m_pShadowDepth = nullptr;
		Vector4 m_uvClamp = Vector4(0.0, 0.0, 1.0, 1.0);
		float m_fNear = 0.0f;
		float m_fFar = 0.0f;
	};

	class DeferredShadow : public Referenced
	{
	public:
		DeferredShadow();

		void Init();
	
		void Reset(Texture* pDepthStencil);

		void Render(SceneContext* sc);

		void Shoutdown();

		void AddSMFrustumInfo(const SMFrustumInfo& info);

		SamplerState* GetShadowSampler() {return m_pShadowSampler.get();}

	private:
		void CreateSimpleLightFrustumMesh();

	private:

		//RefPtr<RenderStep>	m_pRenderStep;

		RefPtr<Texture>		m_pShadowTex;	
		RefPtr<SamplerState> m_pShadowSampler;

		RefPtr<RenderPass>	m_pShadowPass;

		RefPtr<Technique>	m_pFrustumVolume[4];

		RefPtr<Technique>	m_pDefferedShadow[4];

		RefPtr<Renderable>	m_pRenderable;

		std::vector<SMFrustumInfo>	m_vecFrustum;

	};

	DeferredShadow* GetDeferredShadow();

}

