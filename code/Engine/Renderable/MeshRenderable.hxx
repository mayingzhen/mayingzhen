#include "Renderable.h"
#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
	MeshRenderable::MeshRenderable()
	{

	}

	void MeshRenderable::Render(Technique* pTech)
	{
		GetRenderContext()->SetCurRenderObj(this);

		pTech->Bind();

		pTech->CommitChanges();

		GetRenderSystem()->DrawRenderable(this,pTech);
	}
}

