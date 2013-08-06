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

	void CharaControlComponent::Render()
	{	
		if (m_pCharaControll == NULL)
			return;

		ICapsuleCollisionShape* pCapusle = m_pCharaControll->GetCollisionShape();
		
		NodeTransform tsfWS = m_pGameObject->GetPhyscisObject()->GetTransformWS();
		NodeTransform tsfLS = pCapusle->GetTransformLS();
		
		NodeTransform tsfBoxWS;
		TransformMul(&tsfBoxWS,&tsfWS,&tsfLS);
		Matrix4x4 matWS;
		MatrixFromTransform(&matWS,&tsfBoxWS);

		float fRadius = pCapusle->GetRadius();
		float fHeight = pCapusle->GetHeight();
		Vector3 boxSize = Vector3(fRadius * 2, fHeight + 2 * fRadius, fRadius * 2);
		LineRender::DrawBox(matWS,boxSize,Color(1,0,0,0));
	}

}
