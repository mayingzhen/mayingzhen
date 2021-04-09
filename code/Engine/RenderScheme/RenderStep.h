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

		virtual void Render();

		virtual void Render(RenderPass* prePass, RenderPass* nextPass);

		virtual void Reset() {}

		virtual void PrepareRender(RenderProxy* proxy) {}

		virtual void DrawRenderProxy(RenderProxy* proxy) {}

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

	private:

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
		DefferedLightStep();

	private:
		virtual void PrepareRender(RenderProxy* proxy) override;

		virtual void Render() override;
	};

	class TransluceStep : public RenderStep
	{
	public:
		TransluceStep();

	private:

		virtual void PrepareRender(RenderProxy* proxy) override;

		virtual void Render() override;
	};

// 	class PostProcessStep : public RenderStep
// 	{
// 
// 	};

	class UIRenderStep : public RenderStep
	{

	};
}





