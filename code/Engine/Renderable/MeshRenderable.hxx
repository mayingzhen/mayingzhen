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

	void MeshRenderable::Render(Technique* pTech)
	{
		//pTech->CommitChanges();

		//GetRenderSystem()->DrawRenderable(this,pTech);
	}
}

