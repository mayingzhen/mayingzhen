#include "CharaControlComponent.h"

namespace ma
{
	IMPL_OBJECT(CharaControlComponent,Component)

	CharaControlComponent::CharaControlComponent(GameObject* pGameObj):
	Component(pGameObj)
	{
		m_pCharaControll = NULL;

		ASSERT(m_pGameObject && m_pGameObject->GetPhyscisObject());
		if (m_pGameObject && m_pGameObject->GetPhyscisObject())
		{
			m_pCharaControll = m_pGameObject->GetPhyscisObject()->CreateCharaControll();
		}
	}


	void CharaControlComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

// 		NodeTransform tsfLS = m_pBoxCollisionShape->GetTransformLS();
// 		Vector3 vSize = m_pBoxCollisionShape->GetSize();
// 		
// 		sl.Serialize(tsfLS);
// 		sl.Serialize(vSize);
// 
// 		if (sl.IsReading())
// 		{
// 			m_pBoxCollisionShape->SetTransformLS(tsfLS);
// 			m_pBoxCollisionShape->SetSize(vSize);
// 		}

		sl.EndSection();
	}

}
