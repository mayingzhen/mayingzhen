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

	void RenderQueue::AddRenderObj(int stage, Renderable* pRenderObj, Technique* pTech)
	{
		m_vecRenderList[stage].emplace_back(RenderItem(pRenderObj, pTech));
	}

	void RenderQueue::AddComputeObj(Renderable* pRenderObj, Technique* pTech)
	{
		m_vecCompute.emplace_back(RenderItem(pRenderObj, pTech));
	}

	void RenderQueue::ParallelRender(RenderCommand* pCommand, RenderItem* pNodeStart, uint32_t nNodeCount)
	{
		pCommand->Begin();

		for (uint32_t i = 0; i < nNodeCount; ++i)
		{
			RenderItem& pRenderItem = pNodeStart[i];

			pRenderItem.m_renderable->PreRender(pRenderItem.m_tech);

			pRenderItem.m_renderable->Render(pRenderItem.m_tech, pCommand);
		}

		pCommand->End();
	}

	void RenderQueue::Render(RenderPass* pPass)
	{
		VEC_RENDERABLE vecAllRenderItem;
		for (uint32_t i = 0; i < RL_Count; ++i)
		{
			for (RenderItem& it : m_vecRenderList[i])
			{
				vecAllRenderItem.emplace_back(it);
			}
			m_vecRenderList[i].clear();
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
					ParallelRender(pCommand, ppNodeStart, nCount);
					});
			}

			GetJobScheduler()->WaitForGroup(jobGroup);
		}
		else
		{
			RenderCommand* pCommand = pPass->GetThreadCommand(0);
			RenderItem* ppNodeStart = &(vecAllRenderItem[0]);
			uint32_t nCount = vecAllRenderItem.size();

			ParallelRender(pCommand, ppNodeStart, nCount);
		}
	}

	void RenderQueue::Render(RenderPass* pPass, int stageBegin, int stageEnd)
	{
// 		for (int stage = stageBegin; stage <= stageEnd; ++stage)
// 		{
// 			auto it = m_vecRenderList.find(stage);
// 			if (it == m_vecRenderList.end())
// 			{
// 				continue;
// 			}
// 
// 			it->second.PrepareRender();
// 
// 			it->second.Render(pPass, it->first);
// 		}
	}

	void RenderQueue::Render(RenderCommand* pRenderCommand, int stageBegin, int stageEnd)
	{
// 		for (int stage = stageBegin; stage <= stageEnd; ++stage)
// 		{
// 			auto it = m_vecRenderList.find(stage);
// 			if (it == m_vecRenderList.end())
// 			{
// 				continue;
// 			}
// 
// 			it->second.PrepareRender();
// 
// 			it->second.Render(pRenderCommand, it->first);
// 		}
	}

	void RenderQueue::Render(RenderCommand* pRenderCommand)
	{
// 		for (auto& it : m_vecRenderList)
// 		{
// 			Render(pRenderCommand, it.first, it.first);
// 		}
	}

	void RenderQueue::Compute()
	{
		for (auto& it : m_vecCompute)
		{
			it.m_renderable->Compute(it.m_tech, GetRenderSystem()->GetComputeCommand());
		}
	}

	void RenderQueue::Clear()
	{
		for (uint32_t i = 0; i < RL_Count; ++i)
		{
			m_vecRenderList[i].clear();
		}

		m_vecCompute.clear();
	}
}

