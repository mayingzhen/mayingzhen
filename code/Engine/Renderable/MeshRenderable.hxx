#include "Renderable.h"
#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
	MeshRenderable::MeshRenderable()
	{

	}

	void MeshRenderable::PreRender(Technique* pTech)
	{
		GetRenderContext()->SetCurRenderObj(this);
 
 		pTech->Bind();
	}

}



