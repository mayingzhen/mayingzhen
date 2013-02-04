#include "Framework/Physics/CollisionComponent.h"
#include "Framework/Physics/ICollisionShape.h"

namespace ma
{
	CollisionComponent::CollisionComponent()
	{
		m_pCollisionShape = NULL;
	}

	maNodeTransform CollisionComponent::GetTransformLS() const 
	{
		assert(m_pCollisionShape);
		if (m_pCollisionShape)
		{
			return m_pCollisionShape->GetTransformLS();
		}
		else
		{
			maNodeTransform tsf;
			maTransformSetIdentity(&tsf);
			return tsf;
		}
	}

	void CollisionComponent::SetTransformLS(const maNodeTransform& tsfLS) 
	{
		assert(m_pCollisionShape);
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
		assert(m_pPhysicsObject);
	}


	D3DXMATRIX CollisionComponent::GetWorldMatrix()
	{
		D3DXMATRIX localMatr;
		maNodeTransform tsfLS = GetTransformLS();
		maMatrixFromTransform(&localMatr,&tsfLS);

		D3DXMATRIX worldMatr;
		if (m_pPhysicsObject)
		{	
			maNodeTransform tsfWS = m_pPhysicsObject->GetTransformWS();
			maMatrixFromTransform(&worldMatr,&tsfWS);
		}
		else
		{
			worldMatr =  this->GetGameObject()->GetWorldMatrix();
		}

		worldMatr = localMatr * worldMatr;

		return worldMatr;
	}

	BoxCollisionComponent::BoxCollisionComponent()
	{
		if (GetPhysicsDevice() == NULL)
			return;

		m_pBoxCollisionShape = GetPhysicsDevice()->CreateBoxCollisionShape();
		m_pCollisionShape = m_pBoxCollisionShape;
	}

	void		BoxCollisionComponent::SetSize(const D3DXVECTOR3& vSize)
	{
		if (m_pBoxCollisionShape)
		{
			m_pBoxCollisionShape->SetSize(vSize);	
		}
	}

	D3DXVECTOR3	BoxCollisionComponent::GetSize() const 
	{
		if (m_pBoxCollisionShape)
		{
			return m_pBoxCollisionShape->GetSize();	
		}
		else
		{
			return D3DXVECTOR3(0,0,0);
		}
	}

	void BoxCollisionComponent::Render()
	{
		D3DXMATRIX worldMatr = GetWorldMatrix();

		D3DXVECTOR3 boxSize = GetSize();
		GetRender()->DrawBox(worldMatr, boxSize, D3DCOLOR_RGBA(0,255,0,255));
	}

	void BoxCollisionComponent::DbgRender(BitField flag)
	{	
		if ( !flag.GetBit(eDbgRenderCollShape) )
			return;

		D3DXMATRIX worldMatr = GetWorldMatrix();

		D3DXVECTOR3 boxSize = GetSize();
		GetRender()->DrawBox(worldMatr, boxSize, D3DCOLOR_RGBA(0,255,0,255));
	}


	void BoxCollisionComponent::SetGameObject(GameObject* pGameObj)
	{
		__super::SetGameObject(pGameObj);

		if (m_pPhysicsObject)
		{
			m_pPhysicsObject->AddBoxCollisionShape(m_pBoxCollisionShape);
		}
	}


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
		if ( !flag.GetBit(eDbgRenderCollShape) )
			return;

		D3DXMATRIX worldMatr = GetWorldMatrix();

		float fRadius = this->GetRadius();
		GetRender()->DrawWireSphere(worldMatr, fRadius, D3DCOLOR_RGBA(0,255,0,255));
	}
}

