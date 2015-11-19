#include "RenderComponent.h"

namespace ma
{
	RenderComponent::RenderComponent()
	{
		m_pCullNode = NULL;
		m_bVisible = true;
		m_fViwMinZ = 0;
		m_fViwMaxZ = 0;
		m_bShadowCaster = false;

		m_bMatrixDirty = true;
		m_bCullDirty = true;
	}

	void RenderComponent::Update()
	{
		UpdateAABBWS();

		UpdateCullTree();
	}

	void RenderComponent::UpdateCullTree()
	{
		if (!m_bCullDirty)
			return;
		
		if (m_pSceneNode && m_pSceneNode->GetScene())
			m_pSceneNode->GetScene()->GetCullTree()->UpdateObject(this);

		m_bCullDirty = false;
	}

	void RenderComponent::UpdateAABBWS() const
	{
		if (!m_bMatrixDirty)
			return;

		if (m_pSceneNode)
		{
			m_worldAABB = m_AABB;
			m_worldAABB.transform( m_pSceneNode->GetMatrixWS() );
		}

		m_bMatrixDirty = false;
	}

	void RenderComponent::Show(Camera* pCamera) 
	{
		m_pSceneNode->SetLastVisibleFrame(GetTimer()->GetFrameCount());
	}

	void RenderComponent::MarkDirty()
	{
		m_bMatrixDirty = true;
		m_bCullDirty = true;
	}

	const AABB&	RenderComponent::GetAABBWS() const 
	{
		UpdateAABBWS();
		
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

