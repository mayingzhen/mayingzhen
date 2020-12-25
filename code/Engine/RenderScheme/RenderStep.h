#pragma once

#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
	class RenderPass;
	class DeferredShading;
	class DeferredShadow;
	class HDRPostProcess;
	class SMAAPostProcess;
	class FrameBuffer;
	class Scene;
	class PostProcessStep;

	class RenderStep : public Referenced
	{
	public:

		RenderStep();

		virtual ~RenderStep();

		virtual void Render(RenderPass* prePass, RenderPass* nextPass);

		virtual void Reset() {}

	public:

		RefPtr<RenderQueue> m_pRenderQueue;

		RefPtr<RenderPass>	m_pRenderPass;

		std::string			m_strName;

		std::vector<RefPtr<Texture>>		m_pReadTextue;
	};

	RefPtr<RenderStep> CreateRenderStep();
}





