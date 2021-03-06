#pragma once

#include "RenderView.h"

namespace ma
{
	class ShadowRenderStep;

	class ShadowRenderView : public RenderView
	{
	public:
		ShadowRenderView();

		void Update() override;

		void Render() override;

		void AddRenderStep(ShadowRenderStep* step);

	private:
		void PrepareDeferredShadow(ShadowRenderStep* step);

	public:
		
		std::vector< RefPtr<ShadowRenderStep> > m_vecRenderStep;

		RefPtr<RenderPass>	m_pRenderPass;

		RefPtr<SamplerState> m_pShadowMapSampler;
	};

}