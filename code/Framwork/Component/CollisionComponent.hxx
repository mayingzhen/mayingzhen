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

	void CapsuleCollisionComponent::Render()
	{
		ICapsuleCollisionShape* pCapusle = m_pCapsuleCollisionShape;

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

