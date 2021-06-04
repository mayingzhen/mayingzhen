#include "RenderQueue.h"
#include "BatchRenderable.h"
#include "RenderContext.h"


namespace ma
{
	RenderQueue::RenderQueue()	
	{
	}

	RenderQueue::~RenderQueue()
	{
	}

	void RenderQueue::AddRenderObj(Renderable* pRenderObj, Technique* pTech)
	{
		m_vecRenderList[pRenderObj->GetRenderOrder()].emplace_back(RenderItem(pRenderObj, pTech));
	}

	void RenderQueue::AddComputeObj(Renderable* pRenderObj, Technique* pTech)
	{
		m_vecCompute.emplace_back(RenderItem(pRenderObj, pTech));
	}

	void RenderQueue::ParallelRender(RenderCommand* pCommand, SceneContext* sc, RenderItem* pNodeStart, uint32_t nNodeCount)
	{
		pCommand->Begin();

		for (uint32_t i = 0; i < nNodeCount; ++i)
		{
			RenderItem& pRenderItem = pNodeStart[i];

			pRenderItem.m_renderable->Render(pRenderItem.m_tech, pCommand, sc);
		}

		pCommand->End();
	}


	void RenderQueue::Render(RenderCommand* pCommand, SceneContext* sc)
	{
		for (auto& vecRenderList : m_vecRenderList)
		{
			for (auto& it : vecRenderList)
			{
				RenderItem& pRenderItem = it;

				pRenderItem.m_renderable->Render(pRenderItem.m_tech, pCommand, sc);
			}
		}
	}

	void RenderQueue::Render(RenderPass* pPass, SceneContext* sc)
	{
		VEC_RENDERABLE vecAllRenderItem;
		for (auto& vecRenderList : m_vecRenderList)
		{
			for (auto& it : vecRenderList)
			{
				vecAllRenderItem.emplace_back(it);
			}
			vecRenderList.clear();
		}

		if (vecAllRenderItem.empty())
			return;

		uint32_t nNumJob = GetJobScheduler()->GetNumThreads() + 1; // WorkThread + MainThread

		if (nNumJob > 1 && vecAllRenderItem.size() > nNumJob)
		{
			//BEGIN_TIME(g_pTaskScheduler);

			uint32_t nCountPerJob = vecAllRenderItem.size() / nNumJob;

			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(nNumJob);

			for (uint32_t iJob = 0; iJob < nNumJob; ++iJob)
			{
				uint32_t nStartIndex = iJob * nCountPerJob;
				uint32_t nEndIndex = nStartIndex + nCountPerJob - 1;
				if (iJob == nNumJob - 1)
					nEndIndex = vecAllRenderItem.size() - 1;

				ASSERT(nEndIndex >= nStartIndex);
				if (nEndIndex < nStartIndex)
					continue;

				uint32_t nCount = nEndIndex - nStartIndex + 1;

				RenderItem* ppNodeStart = &(vecAllRenderItem[nStartIndex]);

				RenderCommand* pCommand = pPass->GetThreadCommand(iJob);

				GetJobScheduler()->SubmitJob(jobGroup, [=]() {
					ParallelRender(pCommand, sc, ppNodeStart, nCount);
					});
			}

			GetJobScheduler()->WaitForGroup(jobGroup);
		}
		else
		{
			RenderCommand* pCommand = pPass->GetThreadCommand(0);
			RenderItem* ppNodeStart = &(vecAllRenderItem[0]);
			uint32_t nCount = vecAllRenderItem.size();

			ParallelRender(pCommand, sc, ppNodeStart, nCount);
		}
	}

	void RenderQueue::Compute(SceneContext* sc)
	{
		for (auto& it : m_vecCompute)
		{
			it.m_renderable->Compute(it.m_tech, GetRenderSystem()->GetComputeCommand(), sc);
		}
	}

	void RenderQueue::Clear()
	{
		for (auto& vecRenderList : m_vecRenderList)
		{
			vecRenderList.clear();
		}

		m_vecCompute.clear();
	}
}

