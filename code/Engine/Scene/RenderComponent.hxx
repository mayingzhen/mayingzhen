#include "RenderComponent.h"

namespace ma
{
	RenderComponent::RenderComponent()
	{
		m_bParallelUpdate = false;
	
		m_bVisible = true;
		m_fViwMinZ = 0;
		m_fViwMaxZ = 0;
		m_bShadowCaster = false;

		m_nAABBChangeType = ACT_SELF_MATRIX;	

		m_pRenderproxy = new RenderProxy();
	}

	void RenderComponent::Update()
	{
		bool bNeedUpdate = (m_nAABBChangeType != ACT_NONE && m_nAABBChangeType != ACT_SELF_CUSTOM);

		UpdateWorldBoundingBox();

		if (bNeedUpdate && m_pSceneNode && m_pSceneNode->GetScene())
		{
			m_pSceneNode->GetScene()->GetCullTree()->UpdateObject(m_pRenderproxy.get());
		}
	}

	void RenderComponent::SetNeedChange(CHANGE_TYPE eChangeType)
	{
		if (eChangeType == CT_FROMPARENT)
		{
			m_nAABBChangeType |= ACT_SELF_MATRIX;
		}
	}

	void RenderComponent::OnAddToSceneNode(SceneNode* pNode)
	{
		if (m_pSceneNode && m_pSceneNode->GetScene())
		{
			m_pSceneNode->GetScene()->GetCullTree()->UpdateObject(m_pRenderproxy.get());
		}
	}

	void RenderComponent::OnRemoveFromSceneNode(SceneNode* pNode)
	{
		if (m_pSceneNode && m_pSceneNode->GetScene())
		{
			m_pSceneNode->GetScene()->GetCullTree()->RemoveObject(m_pRenderproxy.get());
		}
	}

	void RenderComponent::UpdateWorldBoundingBox()
	{
		// when to update
		if (m_nAABBChangeType == ACT_NONE || m_nAABBChangeType == ACT_SELF_CUSTOM || m_nAABBChangeType == ACT_NOTIFY || m_nAABBChangeType == (ACT_SELF_CUSTOM|ACT_NOTIFY))
		{
			return;
		}

		// reset bounds first
		m_worldAABB.setNull();

		// get local bounds
		// update world bounding with worldMatrix
		if ((m_nAABBChangeType&ACT_SELF_MATRIX) != 0 && (m_nAABBChangeType&ACT_SELF_CUSTOM) == 0)
		{
			m_AABB.setNull();
		}

		if ((m_nAABBChangeType&ACT_SELF_CUSTOM) != 0)
		{
			m_nAABBChangeType = ACT_SELF_CUSTOM | ACT_NOTIFY;
		}
		else
		{
			m_nAABBChangeType = ACT_NOTIFY;
		}

		m_worldAABB = m_AABB;
		ASSERT(m_worldAABB.getMinimum() != m_worldAABB.getMaximum());
		m_worldAABB.transformAffine(m_pSceneNode->GetMatrixWS());
	}

	void RenderComponent::Render(RenderQueue* pRenderQueue, RenderPass* pRenderPass)
	{
		ASSERT(false);
	}

	const	AABB& RenderComponent::GetAABB() const
	{
		return m_AABB;
	}

	void RenderComponent::SetAABB(const AABB& box)
	{
		if (m_AABB == box)
			return;

		m_AABB = box;
		m_nAABBChangeType |= ACT_SELF_CUSTOM | ACT_SELF_MATRIX;

		if (m_pSceneNode && m_pSceneNode->GetScene())
		{
			m_pSceneNode->GetScene()->GetCullTree()->UpdateObject(m_pRenderproxy.get());
		}
	}

	const AABB&	RenderComponent::GetAABBWS() 
	{
		UpdateWorldBoundingBox();
		
		return m_worldAABB;
	}

}


