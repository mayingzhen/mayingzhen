#pragma once

#include "Common/Referenced.h"
#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
	class RenderPass;
	class RenderQueue;
	class RenderProxy;

	class RenderStep : public Referenced
	{
	public:

		RenderStep();

		virtual ~RenderStep();

		virtual void PrepareRender(RenderProxy* proxy) {}

		virtual void Render() {}

	public:

		RefPtr<RenderQueue> m_pRenderQueue;

		RefPtr<RenderPass>	m_pRenderPass;

		std::string			m_strName;

		typedef std::vector<RefPtr<Texture>> VEC_TEXTURE;
		VEC_TEXTURE			m_vecReadTextue;
	};

	RefPtr<RenderStep> CreateRenderStep();

	class GbufferStep : public RenderStep
	{
	public:
		GbufferStep();

	private:

		virtual void PrepareRender(RenderProxy* proxy) override;

		virtual void Render() override;

	public:

		RefPtr<Texture>			m_pDepthTex;
		RefPtr<SamplerState>	m_pDepthSampler;

		RefPtr<Texture>			m_pBaseColor;
		RefPtr<SamplerState>	m_pBaseSampler;

		RefPtr<Texture>			m_pNormalTex;
		RefPtr<SamplerState>	m_pNormalSampler;
	};

	class DefferedLightStep : public RenderStep
	{
	public:
		DefferedLightStep(Texture* pDepthTexture);

	private:
		virtual void PrepareRender(RenderProxy* proxy) override;

		virtual void Render() override;

	private:
		void	PrepareLightProxy(LightProxy* proxy);

	private:
		RefPtr<Texture>			m_pHDRColorTex;
	};

	class UIStep : public RenderStep
	{
	public:
		UIStep();

	private:
		virtual void PrepareRender(RenderProxy* proxy) override;

		virtual void Render() override;

	private:
		RefPtr<Texture>			m_pHDRColorTex;
	};

}





