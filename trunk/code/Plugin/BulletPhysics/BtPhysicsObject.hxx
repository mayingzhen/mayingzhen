#include "BtPhysicsObject.h"
#include "BulletPhysics/BtCollisionShape.h"
#include "BulletPhysics/BtRigidBody.h"
#include "BulletPhysics/BulletUtil.h"
#include "BtPhysicsScene.h"

namespace ma
{

	BulletPhysicsObject::BulletPhysicsObject(GameObject* pGameObject,BulletScene* pPhyScene)
	{
		m_pbtRigidBody = NULL;
		m_pXmRigidBody = NULL;
		m_nCollLayer = 0;

		m_pGameObject = pGameObject;
		m_pPhyScene = pPhyScene;
	}

	bool BulletPhysicsObject::Start()
	{
		if (m_vBoxCollisionShape.empty() && m_vSphereCollisionShape.empty() && m_pXmRigidBody == NULL)
			return false;

		btDiscreteDynamicsWorld* pBtDynamicsWorld = m_pPhyScene->GetDynamicsWorld();
		if (pBtDynamicsWorld == NULL)
			return false;

		btCompoundShape* pCompoundShape = new btCompoundShape();
		for (UINT i = 0; i < m_vBoxCollisionShape.size(); ++i)
		{
			BulletBoxCollisionShape* pBtBoxCollisionShape = (BulletBoxCollisionShape*)m_vBoxCollisionShape[i];
			ASSERT(pBtBoxCollisionShape);
			if (pBtBoxCollisionShape == NULL)
				continue;

			btBoxShape* pBtShape = new btBoxShape( ToBulletUnit( pBtBoxCollisionShape->GetSize() ) * 0.5f );
			btTransform btTsfLs = ToBulletUnit(pBtBoxCollisionShape->GetTransformLS());
			pCompoundShape->addChildShape(btTsfLs,pBtShape);

			int nTempCollLayer = pBtBoxCollisionShape->GetCollisionLayer();
			ASSERT(m_nCollLayer == 0 || m_nCollLayer == nTempCollLayer);
			m_nCollLayer = nTempCollLayer;
		}
		for (UINT i = 0; i < m_vSphereCollisionShape.size(); ++i)
		{
			BulletSphereCollisionShape* pBtSphereCollisionShape = (BulletSphereCollisionShape*)m_vSphereCollisionShape[i];
			ASSERT(pBtSphereCollisionShape);
			if (pBtSphereCollisionShape == NULL)
				continue;

			btSphereShape* pBtShape = new btSphereShape(pBtSphereCollisionShape->GetRadius());
			btTransform btTsfLs = ToBulletUnit(pBtSphereCollisionShape->GetTransformLS());
			pCompoundShape->addChildShape(btTsfLs,pBtShape);

			int nTempCollLayer = pBtSphereCollisionShape->GetCollisionLayer();
			ASSERT(m_nCollLayer == 0 || m_nCollLayer == nTempCollLayer);
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

		m_pbtRigidBody->setUserPointer(this);
	
		SyncToPhysics();

		return true;
	}

	bool BulletPhysicsObject::Stop()
	{
		if (m_vBoxCollisionShape.empty() && m_vSphereCollisionShape.empty() && m_pXmRigidBody == NULL)
			return false;

		btDiscreteDynamicsWorld* pBtDynamicsWorld = m_pPhyScene->GetDynamicsWorld();
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

	void BulletPhysicsObject::SyncToPhysics()
	{
		ASSERT(m_pGameObject);
		if (m_pGameObject == NULL)
			return;

		SceneNode* pSceneNode = m_pGameObject->GetSceneNode();
		ASSERT(pSceneNode);
		if (pSceneNode == NULL)
			return;

		SetTransformWS( pSceneNode->GetTransform(TS_WORLD) );
	}

	void BulletPhysicsObject::SyncFromPhysics()
	{
		ASSERT(m_pGameObject);
		if (m_pGameObject == NULL)
			return;

		SceneNode* pSceneNode = m_pGameObject->GetSceneNode();
		ASSERT(pSceneNode);
		if (pSceneNode == NULL)
			return;

		pSceneNode->SetTransform(GetTransformWS(),TS_WORLD);
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

	IRigidBody*				BulletPhysicsObject::CreateRigidBody()
	{
		ASSERT(m_pXmRigidBody == NULL);

		m_pXmRigidBody = new BulletRigidBody();
		return m_pXmRigidBody;
	}

	IBoxCollisionShape*		BulletPhysicsObject::CreateBoxCollisionShape()
	{
		BulletBoxCollisionShape* pBoxColl = new BulletBoxCollisionShape();
		m_vBoxCollisionShape.push_back(pBoxColl);
		return pBoxColl;
	}

	ISphereCollisionShape*	BulletPhysicsObject::CreateSphereCollisionShape()
	{
		BulletSphereCollisionShape* pSpherColl = new BulletSphereCollisionShape();
		m_vSphereCollisionShape.push_back(pSpherColl);
		return pSpherColl;
	}
}
