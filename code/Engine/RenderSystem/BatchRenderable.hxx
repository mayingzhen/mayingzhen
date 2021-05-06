#include "BatchRenderable.h"
#include "RenderSystem.h"
#include "InstanceRenderable.h"

namespace ma
{
	BatchRenderable::BatchRenderable()
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			m_pVertexBuffer[i] = GetRenderSystem()->CreateVertexBuffer(NULL,
				sizeof(InstanceRenderable::InstaceData) * 1024 * 10, sizeof(InstanceRenderable::InstaceData), HBU_DYNAMIC);
		}
	}


	void BatchRenderable::AddRenderObj(Renderable* pRenderObj, Technique* pTech)
	{
		if (!pTech || !pRenderObj->m_bSuportInstace)
		{
			return;
		}

		RefPtr<InstanceRenderable> pIntance = nullptr;
		
		InstanceKey key;
		key.m_pTech = pTech;
		key.m_pIndexBuffer = pRenderObj->m_pIndexBuffer;
		key.m_pVertexBuffer = pRenderObj->m_pVertexBuffer;
		auto it = m_mapInsRenderList.find(key);
		if (it == m_mapInsRenderList.end())
		{
			pIntance = new InstanceRenderable(pTech);
			m_mapInsRenderList[key] = pIntance;
		}
		else
		{
			pIntance = it->second;
		}

		pIntance->AddRenderable(pRenderObj);
	}

	void BatchRenderable::PrepareRender(RenderQueue* pRenderQueue)
	{
		m_nBufferIndex = m_nBufferIndex++;
		m_nBufferIndex = m_nBufferIndex % 3;

		uint32_t nOffset = 0;
		for (auto& it : m_mapInsRenderList)
		{
			it.second->PrepareRender(m_pVertexBuffer[m_nBufferIndex].get(), nOffset);
		}

		for (auto& it : m_mapInsRenderList)
		{
			RefPtr<Technique> pInstanceTech = it.first.m_pTech->CreateInstTech();
			pRenderQueue->AddRenderObj(it.second.get(), pInstanceTech.get());
		}
	}


	void BatchRenderable::Clear()
	{
		m_mapInsRenderList.clear();
	}

}

