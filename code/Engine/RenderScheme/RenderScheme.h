#pragma once

#include "RenderStep.h"


namespace ma
{
	class RenderPass;
	//class RenderStep;
	class DeferredShadow;
	class HDRPostProcess;
	class SMAAPostProcess;
	class FrameBuffer;
	class Scene;
	class PostProcessStep;
	class PostProcessPipeline;
	class PostProcess;


	class DeferredShading : public Referenced
	{
	public:
		DeferredShading();

		~DeferredShading();

		void	Init();

	private:

		void	SetupGlow(PostProcess* pGlow);

		void	SetupSSAO();

		void	SetupHDR();

	private:

		RefPtr<DeferredShadow>	m_pDeferredShadow;

		PostProcessPipeline*	m_pPostProcessPipeline = nullptr;
	};

}




