#pragma once

#include "RenderStep.h"

namespace ma
{

	class SceneContext;

	struct SMFrustumInfo
	{
		Matrix4	m_matViewToShadow;
		SamplerState* m_pShadowDepth = nullptr;
		float m_fNear = 0.0f;
		float m_fFar = 0.0f;
	};

	class DeferredShadow : public MainRenderStep
	{
	public:
		DeferredShadow(Texture* pDepthStencil);
	
		void Reset(Texture* pDepthStencil);

		virtual void BeginePrepareRender() override;

		void AddSMFrustumInfo(const SMFrustumInfo& info);

		SamplerState* GetShadowSampler() {return m_pShadowSampler.get();}

	private:
		void CreateSimpleLightFrustumMesh();

	private:

		RefPtr<Texture>		m_pShadowTex;	
		RefPtr<SamplerState> m_pShadowSampler;

		RefPtr<Technique>	m_pDefferedShadow[4];

		std::vector<SMFrustumInfo>	m_vecFrustum;
	};
}

