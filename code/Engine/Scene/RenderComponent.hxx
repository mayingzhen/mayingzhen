#include "RenderComponent.h"

namespace ma
{
	IMPL_OBJECT(RenderComponent,Component)

	RenderComponent::RenderComponent()
	{
		m_pCullNode = NULL;
		m_bVisible = true;
		m_fViwMinZ = 0;
		m_fViwMaxZ = 0;
		m_bShadowCaster = false;
	}

	void RenderComponent::Show(Camera* pCamera) 
	{
		m_pSceneNode->SetLastVisibleFrame(GetTimer()->GetFrameCount());
	}

	void RenderComponent::OnAddToSceneNode(SceneNode* pNode)
	{
		Component::OnAddToSceneNode(pNode);
		
		//ASSERT(pNode && pNode->GetScene());
		if (pNode && pNode->GetScene())
			pNode->GetScene()->GetCullTree()->UpdateObject(this);
	}

	UINT RenderComponent::GetRenderableCount() const
	{
		return 0;
	}

	Renderable* RenderComponent::GetRenderableByIndex(UINT index) const
	{
		return NULL;
	}
		
	void RenderComponent::OnTransformChange()
	{
		if (m_pSceneNode)
		{	
			m_worldAABB = m_AABB;
 			m_worldAABB.transform( m_pSceneNode->GetMatrixWS() );

			if (m_pSceneNode->GetScene())
				m_pSceneNode->GetScene()->GetCullTree()->UpdateObject(this);
		}
	}

}

