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
 		pTech->Bind(this);
	}

	void Renderable::Render(Technique* pTechnique, RenderCommand* pRenderCommand)
	{
		const RefPtr<SubMeshData>& pSubMeshData = this->m_pSubMeshData;

		uint32_t nIndexBufferNumber = m_pIndexBuffer ? m_pIndexBuffer->GetNumber() : 0;

		uint32_t nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : nIndexBufferNumber;
		uint32_t nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;
		uint32_t nVertexStart = pSubMeshData ? pSubMeshData->m_nVertexStart : 0;
		uint32_t nVertexCount = pSubMeshData ? pSubMeshData->m_nVertexCount : m_pVertexBuffer->GetNumber();

		pRenderCommand->SetTechnique(pTechnique);

		pRenderCommand->SetVertexBuffer(0, this->m_pVertexBuffer.get(), 0);

		if (m_pIndexBuffer)
		{
			pRenderCommand->SetIndexBuffer(this->m_pIndexBuffer.get());

			pRenderCommand->DrawIndex(nIndexStart, nIndexCount, nVertexStart, 1);
		}
		else
		{
			pRenderCommand->Draw(nVertexStart, nVertexCount, 1);
		}
	}

	void Renderable::Compute(Technique* pTechnique, ComputeCommad* pComputeCmd)
	{

	}

}


