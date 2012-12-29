#include "Framework/Physics/BulletRigidbodyComponent.h"


namespace ma
{


	CollisionComponent::CollisionComponent()
	{
		maTransformSetIdentity(&m_tsfLS);
		m_pBtCollShape = NULL;
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
		m_pPhysicsObj = GetGameObject()->GetPhyscisObject();
		m_pPhysicsObj->AddBoxCollision(m_tsfLS,m_vSize);
	}

	RigidBodyComponent::RigidBodyComponent():
	{
	}

	RigidBodyComponent::~RigidBodyComponent()
	{

	}

	void RigidBodyComponent::SetMass(float fMass)
	{
		//if (m_pPhysicsObj)
		//{
		//	m_pPhysicsObj->setMassProps(fMass, m_rbInfo.m_localInertia);
		//}
		//else
		//{
			m_rbInfo.m_fMass = fMass;
		//}
	}

	float RigidBodyComponent::GetMass()
	{
		//if (m_pPhysicsObj)
		//{
		//	1.0f / m_pRigudBody->getInvMass();
		//}
		//else
		//{
			return m_rbInfo.m_mass;
		//}
	}

	// 	void BulletActor::SetUseGravity(bool bUseGravity)
	// 
	// 	bool BulletActor::IsUseGravity();

	void RigidBodyComponent::SetKinematic(bool bKinematic)
	{
		//if (m_pRigudBody)
		//{
		//	UINT flags = m_pRigudBody->getCollisionFlags();
		//	BitField::StaticSetBit(flags,btCollisionObject::CF_KINEMATIC_OBJECT,bKinematic);
		//	m_pRigudBody->setCollisionFlags(flags);
		//}
		//else
		//{
		m_rbInfo.m_bKinematic = bKinematic;
			//BitField::StaticSetBit(m_rbInfo.m_collisionFlags,btCollisionObject::CF_KINEMATIC_OBJECT,bKinematic);
		//}
	}

	bool RigidBodyComponent::IsKinematic()
	{
		//if (m_pRigudBody)
		//{
		//	return m_pRigudBody->isKinematicObject();
		//}
		//else
		//{
		return m_rbInfo.m_bKinematic;
			//return BitField::StaticGetBit(m_rbInfo.m_collisionFlags,btCollisionObject::CF_KINEMATIC_OBJECT);	
		//}
	}

	void RigidBodyComponent::SetLinearDamping(float fLinearDamping)
	{
		//if (m_pRigudBody)
		//{
		//	m_pRigudBody->setDamping(fLinearDamping,m_pRigudBody->getAngularDamping());
		//}
		//else
		//{
		m_rbInfo.m_fLinearDamping = fLinearDamping;
		//	m_rbInfo.m_linearDamping = fLinearDamping;
		//}
	}

	float RigidBodyComponent::GetLinearDamping()
	{
		//if (m_pRigudBody)
		//{
		//	return m_pRigudBody->getLinearDamping();
		//}
		//else
		//{
		return m_rbInfo.m_fLinearDamping;
		//	return m_rbInfo.m_linearDamping;
		//}
	}

	void RigidBodyComponent::SetAngularDamping(float fAngularDamping)
	{
		//if (m_pRigudBody)
		//{
		//	m_pRigudBody->setDamping(m_pRigudBody->getLinearDamping(),fAngularDamping);
		//}
		//else
		//{
		m_rbInfo.m_fAngularDamping = fAngularDamping;
		//	m_rbInfo.m_angularDamping = fAngularDamping;
		//}
	}

	float RigidBodyComponent::GetAngularDamping()
	{
		//if (m_pRigudBody)
		//{
		//	return m_pRigudBody->getAngularDamping();
		//}
		//else
		//{
		return m_rbInfo.m_fAngularDamping;
		//	return m_rbInfo.m_angularDamping;
		//}
	}

// 	void RigidBodyComponent::AddCollisionShape(CollisionShape* pCollisionShape)
// 	{
// 		if (pCollisionShape == NULL)
// 			return;
// 
// 		m_vCollisionShape.push_back(pCollisionShape);
// 	}

// 	void	RigidBodyComponent::SyncFromScene()
// 	{
// 		GameObject* pGameObject = this->GetGameObject();
// 		if (m_pPhysicsObj && IsKinematic() && pGameObject)
// 		{
// 			m_pPhysicsObj->SetTransformWS( pGameObject->GetTransformWS() );
// 			//m_pRigudBody->getMotionState()->setWorldTransform( ToBulletUnit(tsfWS) );
// 		}
// 	}
// 
// 	void	RigidBodyComponent::SyncToScene()
// 	{
// 		GameObject* pGameObject = this->GetGameObject();
// 		if (m_pPhysicsObj && !IsKinematic() && pGameObject)
// 		{
// 			btTransform TSFWS = m_pPhysicsObj->GetWorldTransform();
// 			//m_pRigudBody->getMotionState()->getWorldTransform(btTSFWS);
// 			pGameObject->SetTransformWS( m_pPhysicsObj->GetWorldTransform() );
// 		}
// 	}

	virtual void RigidBodyComponent::Update()
	{
		
	}

	void RigidBodyComponent::Start()
	{
		m_pPhysicsObj = GetGameObject()->GetPhyscisObject();
		if (m_pPhysicsObj == NULL)
		{
			m_rigidBodyInfo.m_UserData = GetGameObject();
			m_pPhysicsObj->SetRigidBodyInfo(m_rigidBodyInfo);
		}
// 		btDiscreteDynamicsWorld* pBulletWorld = GetBulletWorld(this);
// 		assert(pBulletWorld);
// 		if (pBulletWorld == NULL)
// 			return;
// 
// 		for (UINT i = 0; i < m_vCollisionShape.size(); ++i)
// 		{
// 			if (m_vCollisionShape[i])
// 				m_vCollisionShape[i]->Start();
// 		}
// 
// 		if (m_vCollisionShape.size() == 1)
// 		{
// 			m_rbInfo.m_collisionShape = m_vCollisionShape[0]->GetBtCollisionShape();
// 		}
// 		else
// 		{
// 			btCompoundShape* pCompoundShape = new btCompoundShape();
// 			m_rbInfo.m_collisionShape = pCompoundShape;
// 			for (UINT i = 0; i < m_vCollisionShape.size(); ++i)
// 			{
// 				CollisionShape* pCollisonShape = m_vCollisionShape[i];
// 				if (pCollisonShape == NULL)
// 					continue;
// 
// 				btCollisionShape* pBtColShape = pCollisonShape->GetBtCollisionShape();
// 				btTransform btTsfLS = ToBulletUnit( pCollisonShape->GetTransformLS() ); 
// 				pCompoundShape->addChildShape(btTsfLS,pBtColShape);
// 			}
// 		}
// 
// 		m_rbInfo.m_motionState = new btDefaultMotionState();
// 		btRigidBody* body = new btRigidBody(m_rbInfo);
// 		pBulletWorld->addRigidBody(body);
	}

	void RigidBodyComponent::Stop()
	{
		btDiscreteDynamicsWorld* pBulletWorld = GetBulletWorld(this);
		assert(pBulletWorld);
		if (pBulletWorld == NULL)
			return;

		pBulletWorld->removeRigidBody(m_pRigudBody);
		SAFE_DELETE(m_pRigudBody);
	}

// 	btDiscreteDynamicsWorld* RigidBodyComponent::GetBulletWorld(Component* pComp)
// 	{
// 		GameObject* pGameObj = pComp->GetGameObject();
// 		assert(pGameObj);
// 		if (pGameObj == NULL)
// 			return NULL;
// 
// 		Scene* pScene = pGameObj->GetScene();
// 		assert(pScene);
// 		if (pScene == NULL)
// 			return NULL;
// 
// 		PhysicsScene* pPhySicsScene = pScene->GetPhysicsScene();
// 		if (pPhySicsScene == NULL)
// 			return NULL;
// 
// 		return  pPhySicsScene->GetBulletWorld();
// 	}
}

