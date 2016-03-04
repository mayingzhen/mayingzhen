#include "BatchParticleRenderable.h"


namespace ma
{
	BatchParticleRenderable::BatchParticleRenderable()
	{
		Clear();
	}

	void BatchParticleRenderable::MerageRenderable(VEC_RENDERABLE& vecRenderable)
	{
		if (vecRenderable.size() <= 1)
			return;

		if (m_arrMerageRenderList.size() <= m_nMerageCount)
		{
			ParticleSystemRenderable* pRenderable = new ParticleSystemRenderable(NULL);
			m_arrMerageRenderList.push_back(pRenderable);
		}

		ParticleSystemRenderable* pRenderable = (ParticleSystemRenderable*)m_arrMerageRenderList[m_nMerageCount].get();
		pRenderable->m_pSubMaterial = vecRenderable[0]->m_pSubMaterial;
		m_nMerageCount += 1;
	
		uint32 nVertxCount = 0;
		uint32 nIndexCount = 0;

		for (UINT i = 0; i != vecRenderable.size(); ++i)
		{
			ParticleSystemRenderable* p = (ParticleSystemRenderable*)vecRenderable[i].get();
			ASSERT(p->m_nNumIndices > 0);
			if (p->m_nNumIndices <= 0)
				continue;

			UINT nAddVertSize = p->vertices.size();
			if (pRenderable->vertices.size() < nVertxCount + nAddVertSize)
				pRenderable->vertices.resize(nVertxCount + nAddVertSize);
			memcpy(&pRenderable->vertices[0] + nVertxCount, &p->vertices[0], nAddVertSize * sizeof(ParticleSystemRenderable::VERTEX));

			UINT nAddIndexSize = p->m_nNumIndices;
			if (pRenderable->indices.size() < nIndexCount + nAddIndexSize)
				pRenderable->indices.resize(nIndexCount + nAddIndexSize);
			memcpy(&pRenderable->indices[0] + nIndexCount, &p->indices[0], nAddIndexSize * sizeof(uint16));
			for(UINT index = 0; index < p->m_nNumIndices; ++index)
			{
				pRenderable->indices[nIndexCount + index] += nVertxCount;
			}

			nVertxCount += nAddVertSize;	
			nIndexCount += nAddIndexSize;
		}

		pRenderable->m_nNumIndices = nIndexCount;
		pRenderable->m_nNumVertices = nVertxCount;
	}

	void BatchParticleRenderable::PrepareRender()
	{
		BatchRenderable::PrepareRender();

		SubMaterial* pPreMaterial = NULL;
		static VEC_RENDERABLE s_batch;

		for (VEC_RENDERABLE::iterator iter = m_arrRenderList.begin();iter != m_arrRenderList.end();++iter)
		{
			Renderable* info = (*iter).get();
			SubMaterial* pMaterial = info->GetMaterial();

			if (pPreMaterial && pMaterial != pPreMaterial )
			{
				this->MerageRenderable(s_batch);
				s_batch.clear();
			}

			s_batch.push_back(info);

			pPreMaterial = pMaterial;
		}

		this->MerageRenderable(s_batch);
		s_batch.clear();
	}

	void BatchParticleRenderable::Render()
	{
		for (UINT i = 0; i < m_nMerageCount; ++i)
		{
			Renderable* pRenderObj = m_arrMerageRenderList[i].get();
			if (pRenderObj == NULL)
				continue; 

			Technique* pTech = pRenderObj->m_pSubMaterial->GetShadingTechnqiue();

			pRenderObj->Render(pTech);
		}
	}

	void BatchParticleRenderable::Clear()
	{
		BatchRenderable::Clear();

		for (UINT32 i = 0; i < m_arrMerageRenderList.size(); ++i)
		{
			m_arrMerageRenderList[i]->m_pSubMaterial = 0;
		}

		m_nMerageCount = 0;
	}

}

