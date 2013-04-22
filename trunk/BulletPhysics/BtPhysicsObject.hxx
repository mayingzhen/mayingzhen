#include "BulletPhysics/BtPhysicsObject.h"
#include "BulletPhysics/BtCollisionShape.h"
#include "BulletPhysics/BtRigidBody.h"
#include "BulletPhysics/BulletUtil.h"

namespace ma
{

	BulletPhysicsObject::BulletPhysicsObject()
	{
		m_pbtRigidBody = NULL;
		m_pXmRigidBody = NULL;
		m_nCollLayer = 0;
	}

	bool BulletPhysicsObject::Start(GameObject* pOwner)
	{
		if (pOwner == NULL)
			return false;

		btDiscreteDynamicsWorld* pBtDynamicsWorld = GetbtDiscreteDynamicsWorld(pOwner);
		if (pBtDynamicsWorld == NULL)
			return false;

		btCompoundShape* pCompoundShape = new btCompoundShape();
		for (UINT i = 0; i < m_vBoxCollisionShape.size(); ++i)
		{
			BulletBoxCollisionShape* pBtBoxCollisionShape = (BulletBoxCollisionShape*)m_vBoxCollisionShape[i];
			assert(pBtBoxCollisionShape);
			if (pBtBoxCollisionShape == NULL)
				continue;

			btBoxShape* pBtShape = new btBoxShape( ToBulletUnit( pBtBoxCollisionShape->GetSize() ) * 0.5f );
			btTransform btTsfLs = ToBulletUnit(pBtBoxCollisionShape->GetTransformLS());
			pCompoundShape->addChildShape(btTsfLs,pBtShape);

			int nTempCollLayer = pBtBoxCollisionShape->GetCollisionLayer();
			assert(m_nCollLayer == 0 || m_nCollLayer == nTempCollLayer);
			m_nCollLayer = nTempCollLayer;
		}
		for (UINT i = 0; i < m_vSphereCollisionShape.size(); ++i)
		{
			BulletSphereCollisionShape* pBtSphereCollisionShape = (BulletSphereCollisionShape*)m_vSphereCollisionShape[i];
			assert(pBtSphereCollisionShape);
			if (pBtSphereCollisionShape == NULL)
				continue;

			btSphereShape* pBtShape = new btSphereShape(pBtSphereCollisionShape->GetRadius());
			btTransform btTsfLs = ToBulletUnit(pBtSphereCollisionShape->GetTransformLS());
			pCompoundShape->addChildShape(btTsfLs,pBtShape);

			int nTempCollLayer = pBtSphereCollisionShape->GetCollisionLayer();
			assert(m_nCollLayer == 0 || m_nCollLayer == nTempCollLayer);
			m_nCollLayer = nTempCollLayer;
		}

		BulletRigidBody* pBulletRigidBody = (BulletRigidBody*)m_pXmRigidBody;
		if (pBulletRigidBody)
		{
			m_pbtRigidBody = pBulletRigidBody->CreatebtRigidBody(pCompoundShape,pBtDynamicsWorld);
		}
		else
		{
			btDefaultMotionState* myMotionState = new btDefaultMotionState();
			btRigidBody::btRigidBodyConstructionInfo rbInfo(0,myMotionState,pCompoundShape);
			m_pbtRigidBody = new btRigidBody(rbInfo);
			pBtDynamicsWorld->addRigidBody(m_pbtRigidBody);	
		}

		m_pbtRigidBody->setUserPointer(pOwner);
		pOwner->SyncToPhysics();

		return true;
	}

	bool BulletPhysicsObject::Stop(GameObject* pOwner)
	{
		if (pOwner == NULL)
			return false;

		btDiscreteDynamicsWorld* pBtDynamicsWorld = GetbtDiscreteDynamicsWorld(pOwner);
		if (pBtDynamicsWorld == NULL)
			return false;

		pBtDynamicsWorld->removeRigidBody(m_pbtRigidBody);

		BulletRigidBody* pBulletRigidBody = (BulletRigidBody*)m_pXmRigidBody;
		if (pBulletRigidBody)
		{
			pBulletRigidBody->SetbtRigidBody(NULL);
		}

		return true;
	}

	void BulletPhysicsObject::SetTransformWS(const NodeTransform& tsfWS)
	{
		if (m_pbtRigidBody)
		{
			m_pbtRigidBody->setWorldTransform( ToBulletUnit(tsfWS) );
		}

		m_tsfWS = tsfWS;
	}

	NodeTransform BulletPhysicsObject::GetTransformWS()
	{
		if (m_pbtRigidBody)
		{
			return ToMaUnit( m_pbtRigidBody->getWorldTransform() );
		}

		return m_tsfWS;
	}

	void BulletPhysicsObject::ApplyForce(Vector3 vForce)
	{
		if (m_pbtRigidBody)
		{
			m_pbtRigidBody->applyCentralForce(ToBulletUnit(vForce));
		}
	}

	IRigidBody* BulletPhysicsObject::GetRigidBody()
	{
		return m_pXmRigidBody;
	}

	void BulletPhysicsObject::SetRigidBody(IRigidBody* pRigidBody)
	{
		m_pXmRigidBody = pRigidBody;
	}

	void BulletPhysicsObject::AddBoxCollisionShape(IBoxCollisionShape* pCollisionShape)
	{
		m_vBoxCollisionShape.push_back(pCollisionShape);
	}

	void BulletPhysicsObject::AddSphereCollisionShape(ISphereCollisionShape* pSphereCollisionShape)
	{
		m_vSphereCollisionShape.push_back(pSphereCollisionShape);
	}
}
