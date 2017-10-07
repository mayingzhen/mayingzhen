#pragma once

#include "Renderable.h"

namespace ma
{
	class MeshRenderable : public Renderable
	{
	public:
		MeshRenderable();

		virtual void	PreRender(Technique* pTech);
    };
}



