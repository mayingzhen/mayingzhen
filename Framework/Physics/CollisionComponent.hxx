#include "Framework/Physics/CollisionComponent.h"

namespace ma
{
	CollisionComponent::CollisionComponent()
	{
		maTransformSetIdentity(&m_tsfLS);
	}

	CollisionComponent::~CollisionComponent()
	{
	}

	BoxCollisionComponent::BoxCollisionComponent()
	{
		m_vSize = D3DXVECTOR3(0,0,0);

	}

	BoxCollisionComponent::~BoxCollisionComponent()
	{

	}

	void BoxCollisionComponent::Start()
	{
		if ( GetGameObject() == NULL)
			return;

		m_pPhysicsObj = GetGameObject()->GetPhyscisObject();
		if (m_pPhysicsObj == NULL)
		{
			IPhysicsDevice* pPhysicsDevice = GetPhysicsDevice();
			if (pPhysicsDevice)
			{
				m_pPhysicsObj = pPhysicsDevice->CreatePhysicsObject();
				GetGameObject()->SetPhyscisObject(m_pPhysicsObj);
			}
		}

		if (m_pPhysicsObj == NULL)
			return;

		m_pPhysicsObj->AddBoxCollision( BoxCollisionInfo(m_tsfLS,m_vSize) );
	}
}

