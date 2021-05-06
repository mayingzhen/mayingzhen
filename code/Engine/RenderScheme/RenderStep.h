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

	class RenderStep : public Referenced
	{
	public:

		RenderStep();

		virtual ~RenderStep();

		virtual void BeginePrepareRender();

		virtual void PrepareRender(RenderProxy* proxy) {}

		virtual void Render();

	public:

		RefPtr<RenderQueue> m_pRenderQueue;

		RefPtr<RenderPass>	m_pRenderPass;

		std::string			m_strName;
	};

	class GbufferStep : public RenderStep
	{
	public:
		GbufferStep();

	private:

		virtual void BeginePrepareRender();

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

	class DefferedLightStep : public RenderStep
	{
	public:
		DefferedLightStep(Texture* pDepthTexture);

	private:
		virtual void PrepareRender(RenderProxy* proxy) override;

	private:
		void		PrepareLightProxy(LightProxy* proxy);

	private:
		RefPtr<Texture>			m_pHDRColorTex;
	};

	class UIStep : public RenderStep
	{
	public:
		UIStep(SubMaterial* pLastPostProcss);

	private:
		virtual void BeginePrepareRender() override;

		virtual void PrepareRender(RenderProxy* proxy) override;

	private:
		RefPtr<SubMaterial> m_pLastPostProcss;
	
	};

}





