#pragma once

#include "Common/Referenced.h"
#include "Engine/RenderSystem/RenderSystem.h"
#include "Engine/RenderSystem/BatchRenderable.h"

namespace ma
{
	class RenderPass;
	class RenderQueue;
	class RenderProxy;
	class SubMaterial;


	class MainRenderStep : public Referenced
	{
	public:
		MainRenderStep();

		virtual ~MainRenderStep();

		virtual void BeginePrepareRender(SceneContext* sc);

		virtual void PrepareRender(RenderProxy* proxy) {}

		virtual void Render(SceneContext* sc);

	public:
		std::string			m_strName;

		RefPtr<RenderQueue> m_pRenderQueue;

		RefPtr<RenderPass>	m_pRenderPass;
	};

	class GbufferStep : public MainRenderStep
	{
	public:
		GbufferStep();

	private:

		virtual void BeginePrepareRender(SceneContext* sc);

		virtual void PrepareRender(RenderProxy* proxy) override;

	public:

		RefPtr<Texture>			m_pDepthTex;
		RefPtr<SamplerState>	m_pDepthSampler;

		RefPtr<Texture>			m_pBaseColor;
		RefPtr<SamplerState>	m_pBaseSampler;

		RefPtr<Texture>			m_pNormalTex;
		RefPtr<SamplerState>	m_pNormalSampler;

		BatchRenderable			m_batchRender;
	};

	class LinearDepthStep : public MainRenderStep
	{
	public:
		LinearDepthStep(Texture* pDepthBuffer);

		virtual void BeginePrepareRender(SceneContext* sc) override;

	public:
		RefPtr<Texture>			m_pLinearDepthTex;
		RefPtr<SamplerState>	m_pLinearDepthSampler;

		RefPtr<Technique>		m_pTechnique;
	};


	class DefferedLightStep : public MainRenderStep
	{
	public:
		DefferedLightStep(Texture* pDepthBuffer);

	private:
		virtual void BeginePrepareRender(SceneContext* sc) override;

		virtual void PrepareRender(RenderProxy* proxy) override;

	private:
		void		PrepareLightProxy(LightProxy* proxy);

	private:
		RefPtr<Texture>			m_pHDRColorTex;

		RefPtr<Technique>		m_pAmbientLight;
		RefPtr<Renderable>		m_pAmbientItem;
	};

	class UIStep : public MainRenderStep
	{
	public:
		UIStep(SubMaterial* pLastPostProcss);

	private:
		virtual void BeginePrepareRender(SceneContext* sc) override;

		virtual void PrepareRender(RenderProxy* proxy) override;

	private:
		RefPtr<SubMaterial> m_pLastPostProcss;
	
	};

}





