#pragma once

#include "RenderStep.h"

namespace ma
{

	class SceneContext;

	struct SMFrustumInfo
	{
		Matrix4	m_matViewToShadow;
		Matrix4	m_matLightViewProj;
		SamplerState* m_pShadowDepth = nullptr;
		float m_fDepthNear = 0.0f;
		float m_fDepthFar = 0.0f;
	};

	class DeferredShadow : public MainRenderStep
	{
	public:
		DeferredShadow(Texture* pDepthStencil);
	
		void Reset(Texture* pDepthStencil);

		virtual void Render(SceneContext* sc) override;

		void AddSMFrustumInfo(const SMFrustumInfo& info);


	private:

		RefPtr<Texture>		m_pShadowTex;	
		RefPtr<SamplerState> m_pShadowSampler;

		RefPtr<Technique>	m_pDefferedShadow[4];

		std::vector<SMFrustumInfo>	m_vecFrustum;
	};
}

