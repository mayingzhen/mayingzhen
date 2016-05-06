#include "RenderComponent.h"

namespace ma
{
	RenderComponent::RenderComponent()
	{
		m_bParallelUpdate = false;
		m_pCullNode = NULL;
		m_bVisible = true;
		m_fViwMinZ = 0;
		m_fViwMaxZ = 0;
		m_bShadowCaster = false;

		m_nAABBChangeType = ACT_SELF_MATRIX;	
	}

	void RenderComponent::Update()
	{
		UpdateWorldBoundingBox();
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
			m_pSceneNode->GetScene()->GetCullTree()->UpdateObject(this);
	}

	void RenderComponent::OnRemoveFromSceneNode(SceneNode* pNode)
	{
		if (m_pSceneNode && m_pSceneNode->GetScene())
			m_pSceneNode->GetScene()->GetCullTree()->RemoveObject(this);	
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
			m_nAABBChangeType = ACT_NOTIFY;

		m_worldAABB = m_AABB;
		ASSERT(m_worldAABB.getMinimum() != m_worldAABB.getMaximum());
		m_worldAABB.transformAffine(m_pSceneNode->GetMatrixWS());
	}

	void RenderComponent::Show(Camera* pCamera) 
	{
		m_pSceneNode->SetLastVisibleFrame(GetTimer()->GetFrameCount());
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
			m_pSceneNode->GetScene()->GetCullTree()->UpdateObject(this);
	}

	const AABB&	RenderComponent::GetAABBWS() 
	{
		UpdateWorldBoundingBox();
		
		return m_worldAABB;
	}

	UINT RenderComponent::GetRenderableCount(uint32 nLod) const
	{
		return 0;
	}

	Renderable* RenderComponent::GetRenderableByIndex(uint32 nLod,UINT index) const
	{
		return NULL;
	}

}

