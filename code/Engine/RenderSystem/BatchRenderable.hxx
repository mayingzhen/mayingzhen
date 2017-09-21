#include "BatchRenderable.h"


namespace ma
{

	void BatchGroup::AddTransforms(const Renderable& batch)
	{
		InstanceData newInstance;
		//newInstance.distance_ = batch.distance_;
		//newInstance.instancingData_ = batch.instancingData_;

		//for (unsigned i = 0; i < batch.numWorldTransforms_; ++i)
		{
			newInstance.worldTransform_ = batch.GetWorldMatrix();
			instances_.push_back(newInstance);
		}
	}

	/// Pre-set the instance data. Buffer must be big enough to hold all data.
	void BatchGroup::SetInstancingData(void* lockedData, unsigned stride, unsigned& freeIndex)
	{
		// Do not use up buffer space if not going to draw as instanced
		//if (geometryType_ != GEOM_INSTANCED)
		//	return;

		startIndex_ = freeIndex;
		unsigned char* buffer = static_cast<unsigned char*>(lockedData) + startIndex_ * stride;

		for (unsigned i = 0; i < instances_.size(); ++i)
		{
			const InstanceData& instance = instances_[i];

			memcpy(buffer, &instance.worldTransform_, sizeof(Matrix3x4));
			if (instance.instancingData_)
				memcpy(buffer + sizeof(Matrix3x4), instance.instancingData_, stride - sizeof(Matrix3x4));

			buffer += stride;
		}

		freeIndex += instances_.size();
	}

	void BatchGroup::Render(Technique* pTech)
	{
		GetRenderContext()->SetCurRenderObj(this);

		pTech->Bind();

		pTech->CommitChanges();

		VertexBuffer* pInstanceBuffer = GetRenderSystem()->GetInstanceBuffer();

// 		GetRenderDevice()->SetVertexBuffer(0, this->m_pVertexBuffer.get());
// 
// 		GetRenderDevice()->SetVertexBuffer(1, pInstanceBuffer);
// 
// 		GetRenderDevice()->SetIndexBuffer(this->m_pIndexBuffer.get());
// 
// 		GetRenderDevice()->DrawRenderable(this, pTech);	
	}

	struct SortDescendingLess
	{
		SortDescendingLess()
		{
		}

		bool operator()(const Renderable* a, const Renderable* b) const
		{
			long i = long(a->GetMaterial()->GetShadingTechnqiue()->GetShaderProgram()) - long(b->GetMaterial()->GetShadingTechnqiue()->GetShaderProgram());
			if (i<0)
				return true;
			else if(i>0)
				return false;

			i = long(a->GetMaterial()->GetShadingTechnqiue()) - long(b->GetMaterial()->GetShadingTechnqiue());
			if (i<0)
				return true;
			else if(i>0)
				return false;

			i = int(a->GetMaterial()) - int(b->GetMaterial());
			if (i<0)
				return true;
			else if(i>0)
				return false;

			return false;
		}
	};

	BatchRenderable::BatchRenderable()
	{
		m_vecIntanceData.resize(100);
		m_pInstanceBuffer = GetRenderSystem()->CreateVertexBuffer(
			(uint8*)&m_vecIntanceData[0],
			m_vecIntanceData.size() * sizeof(InstanceData), 
			sizeof(InstanceData));
	}

	void BatchRenderable::AddRenderObj(Renderable* pRenderObj) 
	{
		m_arrRenderList.push_back(pRenderObj);
	}

	void BatchRenderable::PrepareRender()
	{
		std::sort(m_arrRenderList.begin(), m_arrRenderList.end(), SortDescendingLess());

		m_arrMerge.clear();

		SubMaterial* pPreMaterial = NULL;

		VEC_RENDERABLE s_batch;

		for (auto iter = m_arrRenderList.begin(); iter != m_arrRenderList.end(); ++iter)
		{
			Renderable* info = *iter;
			SubMaterial* pMaterial = info->GetMaterial();

			if (pPreMaterial && (pMaterial != pPreMaterial))
			{
				m_arrMerge.push_back(s_batch);
				s_batch.clear();
			}

			s_batch.push_back(info);

			pPreMaterial = pMaterial;
		}

		m_arrMerge.push_back(s_batch);

		for (UINT i = 0; i < m_arrMerge.size(); ++i)
		{
			VEC_RENDERABLE& vecRenderable = m_arrMerge[i];
			if (vecRenderable.size() == 1)
			{
				continue;
			}
		}
	}

	void BatchRenderable::Render()
	{
		for (UINT i = 0; i < m_arrRenderList.size(); ++i)
		{
			Renderable* pRenderObj = m_arrRenderList[i];
			if (pRenderObj == NULL)
				continue; 

			Technique* pTech = pRenderObj->m_pSubMaterial->GetShadingTechnqiue();

			pRenderObj->Render(pTech);
		}
	}

	void BatchRenderable::Clear()
	{
		m_arrRenderList.clear();
	}

}

