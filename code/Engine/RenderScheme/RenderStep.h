#pragma once

#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
	class RenderPass;
	class RenderQueue;

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

		typedef std::vector<RefPtr<Texture>> VEC_TEXTURE;
		VEC_TEXTURE			m_vecReadTextue;
	};

	RefPtr<RenderStep> CreateRenderStep();

}





