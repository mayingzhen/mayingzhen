#pragma once

namespace ma
{
	class DeferredShadow;
	class PostProcess;
	class PostProcessPipeline;



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

		RefPtr<PostProcessPipeline>	m_pPostProcessPipeline = nullptr;
	};

}




