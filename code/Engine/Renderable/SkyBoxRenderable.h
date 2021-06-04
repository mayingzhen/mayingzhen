#pragma once

#include "Renderable.h"

namespace ma
{
	class SkyBoxRenderable : public Renderable
	{
	public:
		SkyBoxRenderable();

		virtual void	Render(Technique* pTechnique, RenderCommand* pRenderCommand, SceneContext* sc) override;

    };
}



