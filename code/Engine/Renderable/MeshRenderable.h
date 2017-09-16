#ifndef  _MeshRendererable__H__
#define  _MeshRendererable__H__

#include "Renderable.h"

namespace ma
{
	class MeshRenderable : public Renderable
	{
	public:
		MeshRenderable();

		virtual void					Render(Technique* pTech);

	};
}

#endif

