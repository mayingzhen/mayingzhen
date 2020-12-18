#include "ParallelCull.h"
//#include "../Thread/JobScheduler.h"
//#include "MTScheduler.h"

namespace ma
{
	ParallelCull::ParallelCull()
	: CullTree()
	{
	}

	ParallelCull::~ParallelCull(void)
	{
	}

	void ParallelCull::Resize(const AABB& box)
	{

	}


	// object operation
	void ParallelCull::AddObject(RenderProxy* pObject)
	{
		// 虚拟对象不放入裁剪树
		if (pObject->GetAABBWS().isNull())
		{
			return;
		}

		ASSERT(pObject->GetCullIndex() == -1);

		m_vecNode.push_back(pObject);

		m_vecNodeBound.push_back( NodeBound(pObject->GetAABBWS()) );

		pObject->SetCullIndex(m_vecNode.size() - 1);
	}

	void ParallelCull::RemoveObject(RenderProxy* pObject)
	{
		uint32_t nCullIndex = pObject->GetCullIndex();
		if (nCullIndex == -1)
			return;

		ASSERT(nCullIndex < m_vecNode.size());
		if (nCullIndex >= m_vecNode.size())
			return;

		ASSERT(m_vecNode[nCullIndex] == pObject);
		if (m_vecNode[nCullIndex] != pObject)
			return;

		// 交换最后一个元素和要删除元素位置
		pObject->SetCullIndex(-1);

		RenderProxy* pLast = m_vecNode.back();

		if (pLast == pObject)
		{
			m_vecNode.pop_back();
			m_vecNodeBound.pop_back();
			return;
		}

		m_vecNode[nCullIndex] = pLast;

		m_vecNodeBound[nCullIndex] = NodeBound( pLast->GetAABBWS() );

		pLast->SetCullIndex(nCullIndex);

		m_vecNode.pop_back();

		m_vecNodeBound.pop_back();
	}

	void ParallelCull::UpdateObject(RenderProxy* pObject)
	{
		// 虚拟对象不放入裁剪树
		if (pObject->GetAABBWS().isNull())
		{
			return;
		}

		uint32_t nCullIndex = pObject->GetCullIndex();
		if (nCullIndex == -1)
		{
			AddObject(pObject);
			nCullIndex = pObject->GetCullIndex();
		}

		ASSERT(nCullIndex != -1);
		if (nCullIndex == -1)
			return;

		ASSERT(nCullIndex < m_vecNode.size());
		if (nCullIndex >= m_vecNode.size())
			return;

		ASSERT(m_vecNode[nCullIndex] == pObject);
		if (m_vecNode[nCullIndex] != pObject)
			return;

		m_vecNodeBound[nCullIndex] = NodeBound( pObject->GetAABBWS() );
	}


	struct CullJobData
	{
		RenderProxy** m_pNodeStart;
		int* m_pVisStart;
		ParallelCull::NodeBound* m_pNodeBoundStart;
		uint32_t m_nNodeCount;
		uint32_t m_nMask;
		Frustum* m_pFrustum;

		CullJobData()
		{
			m_pNodeStart = NULL;
			m_pVisStart = NULL;
			m_pNodeBoundStart = NULL;
			m_nNodeCount = 0;
			m_nMask = 0;
			m_pFrustum = NULL;
		}
	};

	void* ParallelCullFrustum(void* rawData,void* rawData1)
	{
		const Frustum* pFrustum = reinterpret_cast<const Frustum*>(rawData);
		CullJobData* pCullJobData = reinterpret_cast<CullJobData*>(rawData1);
		RenderProxy** ppNodeStart = pCullJobData->m_pNodeStart;
		int* ppViewStart = pCullJobData->m_pVisStart;
		ParallelCull::NodeBound* ppNodeBound = pCullJobData->m_pNodeBoundStart;
		uint32_t nIndexCount = pCullJobData->m_nNodeCount;

		for (uint32_t i = 0; i <= nIndexCount; ++i)
		{
			RenderProxy* pNode = ppNodeStart[i];
			int& pView = ppViewStart[i];
			ParallelCull::NodeBound& bound = ppNodeBound[i];
			if (bound.m_bInfinite || pFrustum->IntersectSIMD(bound.m_vCenter,bound.m_vExtern) != Frustum::Visibility_NONE)
			{
				pView = 1;
			}
			else
			{
				pView = 0;
			}
		}

		return NULL;
	}


	void ParallelCull::FindObjectsIn(const Frustum* pFrustum,uint32_t mask, OUT vector<RenderProxy*>& vecObj) 
	{
		MICROPROFILE_SCOPEI("", "ParallelCull::FindObjectsIn", 0);

		ASSERT(vecObj.empty());

        uint32_t nNumJob = GetJobScheduler()->GetNumThreads() + 1; // WorkThread + MainThread
	
		if (nNumJob > 1 && m_vecNode.size() > nNumJob)
		{
			BEGIN_TIME(g_pJobScheduler);

			static vector<int> vecVis;
			vecVis.resize(m_vecNode.size(),false);

			static vector<CullJobData> vecJobData;
			vecJobData.resize(nNumJob);

			uint32_t nCountPerJob = m_vecNode.size() / nNumJob;

			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(nNumJob);

			for (uint32_t iJob = 0; iJob < nNumJob; ++iJob)
			{
				uint32_t nStartIndex = iJob * nCountPerJob;
				uint32_t nEndIndex = nStartIndex + nCountPerJob - 1;
				if (iJob == nNumJob - 1) 
					nEndIndex  = m_vecNode.size() - 1;

				ASSERT(nEndIndex >= nStartIndex);
				if (nEndIndex < nStartIndex)
					continue;

				uint32_t nCount = nEndIndex - nStartIndex;
				void* data = (Frustum*)pFrustum;
				vecJobData[iJob].m_pNodeStart = &(m_vecNode[nStartIndex]);
				vecJobData[iJob].m_pVisStart = &(vecVis[nStartIndex]);
				vecJobData[iJob].m_pNodeBoundStart = &(m_vecNodeBound[nStartIndex]);
				vecJobData[iJob].m_nNodeCount = nCount;
				void* data1 = &vecJobData[iJob];
				GetJobScheduler()->SubmitJob(jobGroup,
					[data,data1]() { ParallelCullFrustum(data, data1); }
				);
			}
			GetJobScheduler()->WaitForGroup(jobGroup);

			vecObj.clear();
			for (uint32_t i = 0; i < vecVis.size(); ++i)
			{
				if (vecVis[i])
				{
					vecObj.push_back(m_vecNode[i]);
				}
			}

			END_TIME(g_pJobScheduler);
		}
		else
		{
			//BEGIN_TIME(ParallelCull);
	
			vecObj.clear();
			for (uint32_t i = 0; i < m_vecNode.size(); ++i)
			{
				RenderProxy* pObject = m_vecNode[i];

				ParallelCull::NodeBound& bound = m_vecNodeBound[i];
				if (bound.m_bInfinite || pFrustum->IntersectSIMD(bound.m_vCenter,bound.m_vExtern) != Frustum::Visibility_NONE)
				{
					vecObj.push_back(pObject);
				}
			}

			//END_TIME(ParallelCull);
		}
	}
}

