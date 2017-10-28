#include "BatchRenderable.h"
#include "RenderSystem.h"


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

			i = long(a->m_pVertexBuffer.get()) - long(b->m_pVertexBuffer.get());
			if (i < 0)
				return true;
			else if (i > 0)
				return false;

			i = long(a->m_pIndexBuffer.get()) - long(b->m_pIndexBuffer.get());
			if (i < 0)
				return true;
			else if (i > 0)
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

	void InstanceRenderable::Render(RenderCommand* pRenderCommand)
	{
		Technique* pTech = this->GetTechnique();

		const RefPtr<SubMeshData>& pSubMeshData = this->m_pSubMeshData;

		UINT nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : this->m_pIndexBuffer->GetNumber();
		UINT nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;

		pRenderCommand->SetTechnique(pTech);

		pRenderCommand->SetVertexBuffer(0, this->m_pVertexBuffer.get(),0);

		pRenderCommand->SetIndexBuffer(this->m_pIndexBuffer.get());

		VertexBuffer* pInstanceBuffer = GetRenderSystem()->GetRTInstaneBuffer()->GetVertexBuffer();
		UINT nOffset = m_subVB.m_nFirstVertex * sizeof(InstaceData);
		pRenderCommand->SetVertexBuffer(1, pInstanceBuffer, nOffset);

		UINT nInstancCount = m_arrRenderList.size();

		pRenderCommand->DrawIndex(nIndexStart, nIndexCount, nInstancCount, this->m_ePrimitiveType);
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

		m_Technique = pRenderable->GetTechnique()->GetInstTech();
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
		//ASSERT(!batch.empty());
		if (batch.empty())
			return;

		if (batch.size() == 1)
		{
			m_arrNoInsRenderList.push_back(batch[0]);
			return;
		}

		RefPtr<InstanceRenderable> pInstanceRenderable;

		for (UINT i = 0; i < batch.size(); ++i)
		{
			if (!batch[i]->GetTechnique()->GetInstTech() || !batch[i]->m_bSuportInstace)
			{
				m_arrNoInsRenderList.push_back(batch[i]);
				continue;
			}

			if (pInstanceRenderable == NULL)
			{
				pInstanceRenderable = new InstanceRenderable();
			}

			pInstanceRenderable->AddRenderable(batch[i]);
		}

		if (pInstanceRenderable == NULL)
			return;

		pInstanceRenderable->Create();

		m_arrInsRenderList.push_back(pInstanceRenderable);
	}

	void BatchRenderable::PrepareInstance(RenderPassType eRPType)
	{
		if (m_arrRenderList.empty())
			return;

		m_batchTemp.clear();

		//Technique* pPreMaterial = NULL;
		ShaderProgram* pPreShader = NULL;
		VertexBuffer* pPreVB = NULL;

		for (auto iter = m_arrRenderList.begin(); iter != m_arrRenderList.end(); ++iter)
		{
			Renderable* pRenderable = *iter;

			ShaderProgram* pShader = pRenderable->GetTechnique()->GetShaderProgram();
			VertexBuffer* pVB = pRenderable->m_pVertexBuffer.get();

			if (pPreShader && pShader != pPreShader || pPreVB && pPreVB != pVB)
			{
				this->PrepareInstance(m_batchTemp, NULL);

				m_batchTemp.clear();
			}

			m_batchTemp.push_back(pRenderable);

			pPreShader = pShader;
			pPreVB = pVB;
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
			m_arrPrePareRenderList.push_back(m_arrNoInsRenderList[i]);
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

			pRenderable->Render(pRenderable->GetTechnique(),pJobData->pCommand);
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

				pRenderable->Render(pRenderable->GetTechnique(), pCommand);
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

