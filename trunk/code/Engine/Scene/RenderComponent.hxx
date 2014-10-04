#include "RenderComponent.h"

namespace ma
{
	IMPL_OBJECT(RenderComponent,Component)

	RenderComponent::RenderComponent(SceneNode* pGameObj)
		:Component(pGameObj)
	{
		m_pCullNode = NULL;
		m_bVisible = true;
	}
	
	void RenderComponent::Show(Camera* pCamera) 
	{
		m_pSceneNode->SetLastVisibleFrame(GetTimer()->GetFrameCount());
	}

	void RenderComponent::OnTransformChange()
	{
		m_pSceneNode->GetScene()->GetCullTree()->UpdateObject(this);
	}

	const AABB&	RenderComponent::GetAABBWS()
	{
		if ( m_pSceneNode->IsMatrixWSDirty() )
		{
			m_worldAABB = m_AABB;
			m_worldAABB.transform( m_pSceneNode->GetWorldMatrix() );
		}

		return m_worldAABB;
	}
}

