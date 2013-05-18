#include "CollisionComponent.h"
#include "ICollisionShape.h"

namespace ma
{
	IMPL_OBJECT(CollisionComponent,Component)

	CollisionComponent::CollisionComponent()
	{
		m_pCollisionShape = NULL;
		m_pPhysicsObject = NULL;
	}

	NodeTransform CollisionComponent::GetTransformLS() const 
	{
		ASSERT(m_pCollisionShape);
		if (m_pCollisionShape)
		{
			return m_pCollisionShape->GetTransformLS();
		}
		else
		{
			NodeTransform tsf;
			TransformSetIdentity(&tsf);
			return tsf;
		}
	}

	void CollisionComponent::SetTransformLS(const NodeTransform& tsfLS) 
	{
		ASSERT(m_pCollisionShape);
		if (m_pCollisionShape)
		{
			m_pCollisionShape->SetTransformLS(tsfLS);
		}
	}

	void CollisionComponent::SetCollisionLayer(int eCollLayer)
	{
		if (m_pCollisionShape)
		{
			m_pCollisionShape->SetCollisionLayer(eCollLayer);
		}
	}

	int	CollisionComponent::GetCollisionLayer()
	{
		if (m_pCollisionShape)
		{
			return m_pCollisionShape->GetCollisionLayer();
		}
		return ColLayer_Default;
	}

	void CollisionComponent::SetGameObject(GameObject* pGameObj)
	{
		__super::SetGameObject(pGameObj);

		if (pGameObj == NULL)
			return;

		m_pPhysicsObject = pGameObj->GetPhyscisObject();
		if (m_pPhysicsObject == NULL)
		{
			m_pPhysicsObject = GetPhysicsDevice()->CreatePhysicsObject();
			pGameObj->SetPhyscisObject(m_pPhysicsObject);
		}
		ASSERT(m_pPhysicsObject);
	}


	Matrix4x4 CollisionComponent::GetWorldMatrix()
	{
		Matrix4x4 localMatr;
		NodeTransform tsfLS = GetTransformLS();
		MatrixFromTransform(&localMatr,&tsfLS);

		Matrix4x4 worldMatr;
		if (m_pPhysicsObject)
		{	
			NodeTransform tsfWS = m_pPhysicsObject->GetTransformWS();
			MatrixFromTransform(&worldMatr,&tsfWS);
		}
		else if (m_pGameObject)
		{
			worldMatr =  m_pGameObject->GetWorldMatrix();
		}

		worldMatr = localMatr * worldMatr;

		return worldMatr;
	}

	IMPL_OBJECT(BoxCollisionComponent,CollisionComponent)

	BoxCollisionComponent::BoxCollisionComponent()
	{
		if (GetPhysicsDevice() == NULL)
			return;

		m_pBoxCollisionShape = GetPhysicsDevice()->CreateBoxCollisionShape();
		m_pCollisionShape = m_pBoxCollisionShape;
	}

	void		BoxCollisionComponent::SetSize(const Vector3& vSize)
	{
		if (m_pBoxCollisionShape)
		{
			m_pBoxCollisionShape->SetSize(vSize);	
		}
	}

	Vector3	BoxCollisionComponent::GetSize() const 
	{
		if (m_pBoxCollisionShape)
		{
			return m_pBoxCollisionShape->GetSize();	
		}
		else
		{
			return Vector3(0,0,0);
		}
	}

	void BoxCollisionComponent::Render()
	{
		if (GetRenderDevice() == NULL)
			return;

		Matrix4x4 worldMatr = GetWorldMatrix();

		Vector3 boxSize = GetSize();
		GetRenderDevice()->DrawBox(worldMatr, boxSize, COLOR_RGBA(0,255,0,255));
	}

	void BoxCollisionComponent::DbgRender(BitField flag)
	{	
		if (GetRenderDevice() == NULL)
			return;

		if ( !flag.GetBit(eDbgRenderCollShape) )
			return;

		Matrix4x4 worldMatr = GetWorldMatrix();

		Vector3 boxSize = GetSize();
		GetRenderDevice()->DrawBox(worldMatr, boxSize, COLOR_RGBA(0,255,0,255));
	}


	void BoxCollisionComponent::SetGameObject(GameObject* pGameObj)
	{
		__super::SetGameObject(pGameObj);

		if (m_pPhysicsObject)
		{
			m_pPhysicsObject->AddBoxCollisionShape(m_pBoxCollisionShape);
		}
	}

	IMPL_OBJECT(SphereCollisionComponent,CollisionComponent)

	SphereCollisionComponent::SphereCollisionComponent()
	{
		if (GetPhysicsDevice() == NULL)
			return;

		m_pSphereCollisionShape = GetPhysicsDevice()->CreateSphereCollisionShape();
		m_pCollisionShape = m_pSphereCollisionShape;
	}

	void SphereCollisionComponent::SetRadius(float fRadius)
	{
		if (m_pSphereCollisionShape)
		{
			m_pSphereCollisionShape->SetRadius(fRadius);
		}
	}

	float SphereCollisionComponent::GetRadius() const
	{
		if (m_pSphereCollisionShape)
		{
			return m_pSphereCollisionShape->GetRadius();
		}
		return 0;
	}

	void SphereCollisionComponent::SetGameObject(GameObject* pGameObj)
	{
		__super::SetGameObject(pGameObj);

		if (m_pPhysicsObject)
		{
			m_pPhysicsObject->AddSphereCollisionShape(m_pSphereCollisionShape);
		}
	}

	void SphereCollisionComponent::DbgRender(BitField flag)
	{	
		if (GetRenderDevice() == NULL)
			return;

		if ( !flag.GetBit(eDbgRenderCollShape) )
			return;

		Matrix4x4 worldMatr = GetWorldMatrix();

		float fRadius = this->GetRadius();
		GetRenderDevice()->DrawWireSphere(worldMatr, fRadius, COLOR_RGBA(0,255,0,255));
	}
}

