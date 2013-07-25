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

	void BoxCollisionComponent::Render()
	{	
		NodeTransform tsfWS = m_pGameObject->GetPhyscisObject()->GetTransformWS();
		NodeTransform tsfLS = m_pBoxCollisionShape->GetTransformLS();
		
		NodeTransform tsfBoxWS;
		TransformMul(&tsfBoxWS,&tsfWS,&tsfLS);
		Matrix4x4 matWS;
		MatrixFromTransform(&matWS,&tsfBoxWS);

		LineRender::DrawBox(matWS,m_pBoxCollisionShape->GetSize(),Color(1,0,0,0));
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

	void SphereCollisionComponent::Render()
	{
		NodeTransform tsfWS = m_pGameObject->GetPhyscisObject()->GetTransformWS();
		NodeTransform tsfLS = m_pSphereCollisionShape->GetTransformLS();

		NodeTransform tsfBoxWS;
		TransformMul(&tsfBoxWS,&tsfWS,&tsfLS);
		Matrix4x4 matWS;
		MatrixFromTransform(&matWS,&tsfBoxWS);

		LineRender::DrawWireSphere(matWS,m_pSphereCollisionShape->GetRadius(),Color(1,0,0,0));
	}
}

