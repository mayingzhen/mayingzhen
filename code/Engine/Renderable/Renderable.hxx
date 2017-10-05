#include "Renderable.h"
#include "Engine/RenderSystem/RenderSystem.h"

namespace ma
{
	Renderable::Renderable()
	{
		m_ePrimitiveType = PRIM_TRIANGLESTRIP;
		m_matWorld[0] = Matrix4::IDENTITY;
		m_matWorld[1] = Matrix4::IDENTITY;
	}

	void Renderable::SetWorldMatrix(const Matrix4& matWS)
	{
		int index = GetRenderSystem()->CurThreadFill();
		m_matWorld[index] = matWS;
	}

	const Matrix4& Renderable::GetWorldMatrix() const 
	{
		int index = GetRenderSystem()->CurThreadProcess();
		return m_matWorld[index];
	}

	void Renderable::PreRender(Technique* pTech)
	{
 		GetRenderContext()->SetCurRenderObj(this);
 
 		pTech->Bind();
	}

}


