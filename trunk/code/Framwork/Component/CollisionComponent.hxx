#include "CollisionComponent.h"

namespace ma
{
	IMPL_OBJECT(BoxCollisionComponent,Component)

	BoxCollisionComponent::BoxCollisionComponent(GameObject* pGameObj):
	Component(pGameObj)
	{
		m_pBoxCollisionShape = NULL;

		ASSERT(m_pGameObject && m_pGameObject->GetPhyscisObject());
		if (m_pGameObject && m_pGameObject->GetPhyscisObject())
		{
			m_pBoxCollisionShape = m_pGameObject->GetPhyscisObject()->CreateBoxCollisionShape();
		}
	}


	void BoxCollisionComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		NodeTransform tsfLS = m_pBoxCollisionShape->GetTransformLS();
		Vector3 vSize = m_pBoxCollisionShape->GetSize();
		
		sl.Serialize(tsfLS);
		sl.Serialize(vSize);

		if (sl.IsReading())
		{
			m_pBoxCollisionShape->SetTransformLS(tsfLS);
			m_pBoxCollisionShape->SetSize(vSize);
		}

		sl.EndSection();
	}

	IMPL_OBJECT(SphereCollisionComponent,Component)

	SphereCollisionComponent::SphereCollisionComponent(GameObject* pGameObj)
	:Component(pGameObj)
	{
		m_pSphereCollisionShape = NULL;

		ASSERT(m_pGameObject && m_pGameObject->GetPhyscisObject());
		if (m_pGameObject && m_pGameObject->GetPhyscisObject())
		{
			m_pSphereCollisionShape = m_pGameObject->GetPhyscisObject()->CreateSphereCollisionShape();
		}
	}

	void SphereCollisionComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		NodeTransform tsfLS = m_pSphereCollisionShape->GetTransformLS();
		float fRadius = m_pSphereCollisionShape->GetRadius();

		sl.Serialize(tsfLS);
		sl.Serialize(fRadius);

		if (sl.IsReading())
		{
			m_pSphereCollisionShape->SetTransformLS(tsfLS);
			m_pSphereCollisionShape->SetRadius(fRadius);
		}

		sl.EndSection();
	}


	IMPL_OBJECT(CapsuleCollisionComponent,Component)

	CapsuleCollisionComponent::CapsuleCollisionComponent(GameObject* pGameObj)
		:Component(pGameObj)
	{
		m_pCapsuleCollisionShape = NULL;

		ASSERT(m_pGameObject && m_pGameObject->GetPhyscisObject());
		if (m_pGameObject && m_pGameObject->GetPhyscisObject())
		{
			m_pCapsuleCollisionShape = m_pGameObject->GetPhyscisObject()->CreateCapsuleCollisionShape();
		}
	}

	void CapsuleCollisionComponent::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		NodeTransform tsfLS = m_pCapsuleCollisionShape->GetTransformLS();
		float fRadius = m_pCapsuleCollisionShape->GetRadius();
		float fHeight = m_pCapsuleCollisionShape->GetHeight();

		sl.Serialize(tsfLS);
		sl.Serialize(fRadius);
		sl.Serialize(fHeight);

		if (sl.IsReading())
		{
			m_pCapsuleCollisionShape->SetTransformLS(tsfLS);
			m_pCapsuleCollisionShape->SetRadius(fRadius);
			m_pCapsuleCollisionShape->SetHeight(fHeight);
		}

		sl.EndSection();
	}
}

