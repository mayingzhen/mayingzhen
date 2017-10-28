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

	void Renderable::Render(Technique* pTechnique, RenderCommand* pRenderCommand)
	{
		const RefPtr<SubMeshData>& pSubMeshData = this->m_pSubMeshData;

		UINT nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : this->m_pIndexBuffer->GetNumber();
		UINT nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;

		pRenderCommand->SetTechnique(pTechnique);

		pRenderCommand->SetVertexBuffer(0, this->m_pVertexBuffer.get(), 0);

		pRenderCommand->SetIndexBuffer(this->m_pIndexBuffer.get());

		pRenderCommand->DrawIndex(nIndexStart, nIndexCount, 1, this->m_ePrimitiveType);
	}

}


