#include "RenderObject.h"

namespace ma
{
	IMPL_OBJECT(RenderObject,Component)

	RenderObject::RenderObject(GameObject* pGameObj)
		:Component(pGameObj)
	{

	}

	void RenderObject::UpdateTransform()
	{
		SetWorldMatrix(m_pGameObject->GetSceneNode()->GetWorldMatrix());
	}
}
