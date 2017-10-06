#include "BatchRenderable.h"


namespace ma
{
	struct SortDescendingLess
	{
		SortDescendingLess()
		{
		}

		bool operator()(const Renderable* a, const Renderable* b) const
		{
			long i = long(a->GetTechnique()->GetShaderProgram()) - long(b->GetTechnique()->GetShaderProgram());
			if (i<0)
				return true;
			else if(i>0)
				return false;

			i = long(a->GetTechnique()) - long(b->GetTechnique());
			if (i<0)
				return true;
			else if(i>0)
				return false;

			return false;
		}
	};

	void InstanceRenderable::PreRender(Technique* pTech)
	{
		GetRenderContext()->SetCurRenderObj(this);

		pTech->Bind();
	}

	void InstanceRenderable::AddRenderable(Renderable* pRenderObj)
	{
		m_arrRenderList.push_back(pRenderObj);

		MeshRenderable* pMeshRenderable = (MeshRenderable*)(pRenderObj);

		InstaceData data;
		data.m_world = pRenderObj->GetWorldMatrix();
		data.m_pos_center = pMeshRenderable->m_pos_center;
		data.m_pos_extent = pMeshRenderable->m_pos_extent;
		data.m_tc_extent_center = pMeshRenderable->m_tc_extent_center;

		m_arrInstanceData.push_back(data);
	}

	void InstanceRenderable::Create(Technique* pTech)
	{
		if (m_arrInstanceData.empty())
			return;

		m_Technique = pTech->GetInstTech();

		uint8* pData = (uint8*)m_arrInstanceData.data();
		UINT nSize = m_arrInstanceData.size() * sizeof(InstaceData);
		m_pInstBuffer = GetRenderSystem()->CreateVertexBuffer(pData, nSize, sizeof(InstaceData));
	}

	BatchRenderable::BatchRenderable()
	{

	}

	void BatchRenderable::AddRenderObj(Renderable* pRenderObj) 
	{
		m_arrRenderList.push_back(pRenderObj);
	}

	void BatchRenderable::PrepareInstance(const std::vector<Renderable*>& batch,Technique* pTech)
	{
		ASSERT(!batch.empty());
		if (batch.empty())
			return;

		if (batch.size() == 1)
		{
			m_arrNoInsRenderList.push_back(batch[0]);
			return;
		}

		InstanceRenderable* pInstanceRenderable = new InstanceRenderable();

		Technique* pBaseTech = batch[0]->GetTechnique();

		Renderable* pRenderable = NULL;
		for (UINT i = 0; i < batch.size(); ++i)
		{
			pRenderable = batch[i];
			if (!pBaseTech->GetInstTech())
			{
				m_arrNoInsRenderList.push_back(pRenderable);
				continue;
			}

			pInstanceRenderable->AddRenderable(pRenderable);
		}

		pInstanceRenderable->m_ePrimitiveType = pRenderable->m_ePrimitiveType;
		pInstanceRenderable->m_pVertexBuffer = pRenderable->m_pVertexBuffer;
		pInstanceRenderable->m_pIndexBuffer = pRenderable->m_pIndexBuffer;
		pInstanceRenderable->m_pSubMeshData = pRenderable->m_pSubMeshData;
		pInstanceRenderable->m_pSubMaterial = pRenderable->m_pSubMaterial;

		pInstanceRenderable->Create(pBaseTech);

		m_arrInsRenderList.push_back(pInstanceRenderable);
	}

	void BatchRenderable::PrepareInstance(RenderPassType eRPType)
	{
		if (m_arrRenderList.empty())
			return;

		m_batchTemp.clear();

		//Technique* pPreMaterial = NULL;
		ShaderProgram* pPreShader = NULL;

		for (auto iter = m_arrRenderList.begin(); iter != m_arrRenderList.end(); ++iter)
		{
			Renderable* info = *iter;

			ShaderProgram* pShader = info->GetTechnique()->GetShaderProgram();

			if (pPreShader && (pShader != pPreShader))
			{
				this->PrepareInstance(m_batchTemp, NULL);

				m_batchTemp.clear();
			}

			m_batchTemp.push_back(info);

			pPreShader = pShader;
		}

		this->PrepareInstance(m_batchTemp, NULL);

		m_batchTemp.clear();
	}

	void BatchRenderable::PrepareRender(RenderPassType eRPType)
	{
		std::sort(m_arrRenderList.begin(), m_arrRenderList.end(), SortDescendingLess());

		PrepareInstance(eRPType);

		for (UINT i = 0; i < m_arrInsRenderList.size(); ++i)
		{
			m_arrPrePareRenderList.push_back(m_arrInsRenderList[i].get());
		}

		for (UINT i = 0; i < m_arrNoInsRenderList.size(); ++i)
		{
			m_arrPrePareRenderList.push_back(m_arrRenderList[i]);
		}

		for (UINT i = 0; i < m_arrPrePareRenderList.size(); ++i)
		{
			Renderable* pRenderObj = m_arrPrePareRenderList[i];
			if (pRenderObj == NULL)
				continue;

			Technique* pTech = pRenderObj->GetTechnique();

			pRenderObj->PreRender(pTech);
		}
	}

	void RenderRenderable(RenderCommand* pRenderCommand, Renderable* pRenderable, RenderPassType eRPType)
	{
		Technique* pTech = pRenderable->GetTechnique();

		const RefPtr<SubMeshData>& pSubMeshData = pRenderable->m_pSubMeshData;

		UINT nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : pRenderable->m_pIndexBuffer->GetNumber();
		UINT nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;

		pRenderCommand->SetTechnique(pTech);

		pRenderCommand->SetVertexBuffer(0, pRenderable->m_pVertexBuffer.get());

		pRenderCommand->SetIndexBuffer(pRenderable->m_pIndexBuffer.get());

		UINT nInstancCount = 1;
		if (pRenderable->m_pInstBuffer)
		{
			pRenderCommand->SetVertexBuffer(1, pRenderable->m_pInstBuffer.get());

			nInstancCount = pRenderable->m_pInstBuffer->GetNumber();
		}

		pRenderCommand->DrawIndex(nIndexStart, nIndexCount, nInstancCount, pRenderable->m_ePrimitiveType);
	}

	struct RenderJobData
	{
		Renderable** m_pNodeStart = NULL;
		uint32 m_nNodeCount = 0;
		UINT m_nIndex = 0;
		RenderPassType eRPType;
		RenderCommand* pCommand = NULL;
	};

	void* ParallelRender(void* rawData, void* rawData1)
	{
		RenderJobData* pJobData = reinterpret_cast<RenderJobData*>(rawData);

		pJobData->pCommand->Begin();

		Renderable** ppNodeStart = pJobData->m_pNodeStart;
		UINT32 nIndexCount = pJobData->m_nNodeCount;

		for (uint32 i = 0; i <= nIndexCount; ++i)
		{
			Renderable* pRenderable = ppNodeStart[i];

			RenderRenderable(pJobData->pCommand, pRenderable, pJobData->eRPType);
		}

		pJobData->pCommand->End();

		return NULL;
	}

	void BatchRenderable::Render(RenderPass* pPass, RenderPassType eRPType, RenderListType eRLType)
	{
		uint32 nNumJob = GetJobScheduler()->GetNumThreads() + 1; // WorkThread + MainThread

		if (nNumJob > 1 && m_arrPrePareRenderList.size() > nNumJob)
		{
			//BEGIN_TIME(g_pTaskScheduler);

			static vector<RenderJobData> vecJobData;
			vecJobData.resize(nNumJob);

			uint32 nCountPerJob = m_arrPrePareRenderList.size() / nNumJob;

			JobScheduler::JobGroupID jobGroup = GetJobScheduler()->BeginGroup(nNumJob);

			for (UINT32 iJob = 0; iJob < nNumJob; ++iJob)
			{
				uint32 nStartIndex = iJob * nCountPerJob;
				uint32 nEndIndex = nStartIndex + nCountPerJob - 1;
				if (iJob == nNumJob - 1)
					nEndIndex = m_arrPrePareRenderList.size() - 1;

				ASSERT(nEndIndex >= nStartIndex);
				if (nEndIndex < nStartIndex)
					continue;

				uint32 nCount = nEndIndex - nStartIndex;

				vecJobData[iJob].m_pNodeStart = &(m_arrPrePareRenderList[nStartIndex]);

				vecJobData[iJob].m_nNodeCount = nCount;

				vecJobData[iJob].m_nIndex = iJob;

				vecJobData[iJob].eRPType = eRPType;

				vecJobData[iJob].pCommand = pPass->GetThreadCommand(iJob, eRLType);

				void* data = &vecJobData[iJob];
				GetJobScheduler()->SubmitJob(jobGroup, ParallelRender, data, NULL, NULL);
			}

			GetJobScheduler()->WaitForGroup(jobGroup);
		}
		else
		{
			RenderCommand* pCommand = pPass->GetThreadCommand(0,eRLType);
		
			pCommand->Begin();

			for (UINT i = 0; i < m_arrPrePareRenderList.size(); ++i)
			{
				Renderable* pRenderable = m_arrPrePareRenderList[i];
				if (pRenderable == NULL)
					continue;

				RenderRenderable(pCommand, pRenderable, eRPType);
			}

			pCommand->End();
		}
	}

	void BatchRenderable::Clear()
	{
		m_arrRenderList.clear();

		m_arrInsRenderList.clear();
		m_arrNoInsRenderList.clear();

		m_arrPrePareRenderList.clear();
	}

}

