#include "InstanceRenderable.h"
#include "RenderSystem.h"


namespace ma
{
	InstanceRenderable::InstanceRenderable(Technique* pTech)
	{
	}

	void InstanceRenderable::Render(Technique* pTechnique, RenderCommand* pRenderCommand, SceneContext* sc)
	{
		const RefPtr<SubMeshData>& pSubMeshData = this->m_pSubMeshData;

		uint32_t nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : this->m_pIndexBuffer->GetNumber();
		uint32_t nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;

		pRenderCommand->SetTechnique(pTechnique);

		pRenderCommand->SetIndexBuffer(this->m_pIndexBuffer.get());

		pRenderCommand->SetVertexBuffer(0, this->m_pVertexBuffer.get(),0);

		pRenderCommand->SetVertexBuffer(1, m_pInstanceBuffer.get(), m_nInstaceOffset);

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

	void InstanceRenderable::PrepareRender(VertexBuffer* pInstanceBuffer, uint32_t& nOffset)
	{
		if (m_arrRenderList.empty())
			return;

		Renderable* pRenderable = m_arrRenderList[0];
		m_ePrimitiveType = pRenderable->m_ePrimitiveType;
		m_pVertexBuffer = pRenderable->m_pVertexBuffer;
		m_pIndexBuffer = pRenderable->m_pIndexBuffer;
		m_pSubMeshData = pRenderable->m_pSubMeshData;
		m_pSubMaterial = pRenderable->m_pSubMaterial;

		m_pInstanceBuffer = pInstanceBuffer;
		m_nInstaceOffset = nOffset;
		uint32_t nSize = m_arrInstanceData.size() * sizeof(InstaceData);
		pInstanceBuffer->UpdateData(nOffset, (uint8_t*)m_arrInstanceData.data(), nSize);
		nOffset += nSize;
	}

}


