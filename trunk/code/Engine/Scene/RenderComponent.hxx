#include "RenderComponent.h"

namespace ma
{
	IMPL_OBJECT(RenderComponent,Component)

	RenderComponent::RenderComponent(SceneNode* pGameObj)
		:Component(pGameObj)
	{
		m_pCullNode = NULL;
		m_bVisible = true;
		m_fViewMaxZ = 0;
		m_fViewMaxZ = 0;
	}

	void RenderComponent::Show(Camera* pCamera) 
	{
		m_pSceneNode->SetLastVisibleFrame(GetTimer()->GetFrameCount());
	}

	void RenderComponent::OnTransformChange()
	{
		m_worldAABB = m_AABB;
 		m_worldAABB.transform( m_pSceneNode->GetMatrixWS() );

		if (m_pSceneNode->GetScene())
			m_pSceneNode->GetScene()->GetCullTree()->UpdateObject(this);
	}

	const AABB&	RenderComponent::GetAABBWS()
	{
		return m_worldAABB;
	}

	float RenderComponent::GetViewMinZ()
	{
		return m_fViewMaxZ;
	}

	float RenderComponent::GetViewMaxZ()
	{
		return m_fViewMaxZ;
	}
}

