#include "Renderable.h"
#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
	MeshRenderable::MeshRenderable()
	{

	}

	void MeshRenderable::PreRender(Technique* pTech)
	{
 		pTech->Bind(this);
	}

}



