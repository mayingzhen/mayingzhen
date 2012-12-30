#include "BulletPhysics/BtPhysicsObject.h"
#include "BulletPhysics/BulletUtil.h"
#include "BulletPhysics/BtPhysicsDevive.h"
#include "BulletPhysics/BtPhysicsScene.h"

namespace ma
{
	void BtPhysicsObject::Start()
	{
		BtPhysicsDevice* pBtPhysicsDevice = (BtPhysicsDevice*)GetPhysicsDevice();
		if (pBtPhysicsDevice == NULL)
			return;

		BtPhysicsScene* pBtPhysicsScene = pBtPhysicsDevice->GetBtPhysicsScene();
		if (pBtPhysicsScene == NULL)
			return;

		btDiscreteDynamicsWorld* pBtWorld = pBtPhysicsScene->GetDynamicsWorld();
		if (pBtWorld == NULL)
			return;

		btCollisionShape* pCollisonShape = NULL;
		if (m_vBoxCollInfo.size() == 1)
		{
			btVector3 boxHalfExtents = ToBulletUnit(m_vBoxCollInfo[0].m_vSize) * 0.5;
			pCollisonShape = new btBoxShape(boxHalfExtents);
		}
 		else if (m_vBoxCollInfo.size() > 1)
 		{
 			btCompoundShape* pCompoundShape = new btCompoundShape();
 			pCollisonShape = pCompoundShape;
 
			for (UINT i = 0; i < m_vBoxCollInfo.size(); ++i)
 			{
				btVector3 boxHalfExtents = ToBulletUnit(m_vBoxCollInfo[i].m_vSize) * 0.5;
				btTransform btTsfLS = ToBulletUnit(m_vBoxCollInfo[i].m_tsfLS); 
 				btBoxShape* pCollisonShape = new btBoxShape(boxHalfExtents);	
				pCompoundShape->addChildShape(btTsfLS,pCollisonShape);
 			}
		}

		btVector3 localInertia(0,0,0);			
		if (pCollisonShape)
		{
			pCollisonShape->calculateLocalInertia(m_rbInfo.m_fMass,localInertia);
		}

		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbInfo(m_rbInfo.m_fMass,myMotionState,pCollisonShape,localInertia);
		m_pRigidBody = new btRigidBody(rbInfo);
		
		pBtWorld->addRigidBody(m_pRigidBody);	
	}

	void BtPhysicsObject::Stop()
	{

	}

	bool BtPhysicsObject::IsKinematic()
	{
		return m_pRigidBody->isKinematicObject();
	}

	void BtPhysicsObject::SetRigidBody(const RigidBodyInfo& info)
	{
		m_rbInfo = info;
	}

	void BtPhysicsObject::AddBoxCollision(const BoxCollisionInfo& info)
	{
		m_vBoxCollInfo.push_back(info);
	}

	void BtPhysicsObject::SetTransformWS(const maNodeTransform& tsfWS)
	{
		if (m_pRigidBody == NULL)
			return;

		m_pRigidBody->setWorldTransform( ToBulletUnit(tsfWS) );
	}

	maNodeTransform BtPhysicsObject::GetTransformWS() const
	{
		if (m_pRigidBody == NULL)
		{
			maNodeTransform tsfWS;
			maTransformSetIdentity(&tsfWS);
			return tsfWS;
		}

		return ToMaUnit( m_pRigidBody->getWorldTransform() );
	}
}