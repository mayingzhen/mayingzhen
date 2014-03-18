#include "RenderComponent.h"

namespace ma
{
	IMPL_OBJECT(RenderComponent,Component)

	RenderComponent::RenderComponent(GameObject* pGameObj)
		:Component(pGameObj)
	{

	}

// 	void RenderComponent::UpdateTransform()
// 	{
// 		SetWorldMatrix(m_pGameObject->GetSceneNode()->GetWorldMatrix());
// 	}
}
