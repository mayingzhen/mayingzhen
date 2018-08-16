#include "InstanceRenderable.h"
#include "RenderSystem.h"


namespace ma
{
	void InstanceRenderable::PreRender(Technique* pTech)
	{
		pTech->Bind(this);
	}

	void InstanceRenderable::Render(Technique* pTechnique, RenderCommand* pRenderCommand)
	{
		const RefPtr<SubMeshData>& pSubMeshData = this->m_pSubMeshData;

		uint32_t nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : this->m_pIndexBuffer->GetNumber();
		uint32_t nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;

		pRenderCommand->SetTechnique(pTechnique);

		pRenderCommand->SetIndexBuffer(this->m_pIndexBuffer.get());

		pRenderCommand->SetVertexBuffer(0, this->m_pVertexBuffer.get(),0);

		VertexBuffer* pInstanceBuffer = GetRenderSystem()->GetRTInstaneBuffer()->GetVertexBuffer();
		uint32_t nOffset = m_subVB.m_nFirstVertex * sizeof(InstaceData);
		pRenderCommand->SetVertexBuffer(1, pInstanceBuffer, nOffset);

		uint32_t nInstancCount = m_arrRenderList.size();

		pRenderCommand->DrawIndex(nIndexStart, nIndexCount, 0, nInstancCount);
	}

	void InstanceRenderable::AddRenderable(Renderable* pRenderObj)
	{
		m_arrRenderList.push_back(pRenderObj);

		InstaceData data;
		data.m_world = pRenderObj->GetWorldMatrix();

		m_arrInstanceData.push_back(data);
	}

	void InstanceRenderable::Create()
	{
		if (m_arrRenderList.empty())
			return;

		Renderable* pRenderable = m_arrRenderList[0];
		m_ePrimitiveType = pRenderable->m_ePrimitiveType;
		m_pVertexBuffer = pRenderable->m_pVertexBuffer;
		m_pIndexBuffer = pRenderable->m_pIndexBuffer;
		m_pSubMeshData = pRenderable->m_pSubMeshData;
		m_pSubMaterial = pRenderable->m_pSubMaterial;

		m_subVB = GetRenderSystem()->GetInstanceBuffer()->AllocVertexBuffer(m_arrInstanceData.size());
		memcpy(m_subVB.m_pVertices, m_arrInstanceData.data(), m_arrInstanceData.size() * sizeof(InstaceData));

	}

}


