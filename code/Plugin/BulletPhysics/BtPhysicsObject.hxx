#include "BtPhysicsObject.h"
#include "BulletPhysics/BtCollisionShape.h"
#include "BulletPhysics/BtRigidBody.h"
#include "BulletPhysics/BulletUtil.h"
#include "BtPhysicsScene.h"
#include "BtPhysicsJoint.h"


namespace ma
{

	BulletPhysicsObject::BulletPhysicsObject(GameObject* pGameObject,BulletScene* pPhyScene)
	{
		m_pBtCollObject = NULL;
		m_pRigidBody = NULL;
		m_nCollLayer = 0;

		m_pGameObject = pGameObject;
		m_pPhyScene = pPhyScene;

		TransformSetIdentity(&m_tsfWS);
	}

	void BulletPhysicsObject::InitCollObject()
	{
		if (m_vBoxCollisionShape.empty() && m_vSphereCollisionShape.empty() && m_pRigidBody == NULL)
			return ;

		btDiscreteDynamicsWorld* pBtDynamicsWorld = m_pPhyScene->GetDynamicsWorld();
		if (pBtDynamicsWorld == NULL)
			return ;

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

		BulletRigidBody* pBulletRigidBody = (BulletRigidBody*)m_pRigidBody;
		if (pBulletRigidBody)
		{
			m_pBtCollObject = pBulletRigidBody->CreatebtRigidBody(pCompoundShape,pBtDynamicsWorld);
		}
		else
		{
			//btDefaultMotionState* myMotionState = new btDefaultMotionState();
			//btRigidBody::btRigidBodyConstructionInfo rbInfo(0,myMotionState,pCompoundShape);
			m_pBtCollObject = new btCollisionObject();
			m_pBtCollObject->setCollisionShape(pCompoundShape);
			pBtDynamicsWorld->addCollisionObject(m_pBtCollObject);	
		}

		m_pBtCollObject->setUserPointer(this);

		SyncToPhysics();
	}

	void BulletPhysicsObject::InitJoint()
	{
		for (UINT i = 0; i < m_vGenericJoint.size(); ++i)
		{
			BulletPhysicsGenericJoint* pGenerGenericJoint = (BulletPhysicsGenericJoint*)m_vGenericJoint[i];
			ASSERT(pGenerGenericJoint);
			if (pGenerGenericJoint == NULL)
				continue;

			pGenerGenericJoint->Create();
		}

		for (UINT i = 0; i < m_vHingeJoint.size(); ++i)
		{
			BulletPhysicsHingeJoint* pHingeJoint = (BulletPhysicsHingeJoint*)m_vHingeJoint[i];
			ASSERT(pHingeJoint);
			if (pHingeJoint == NULL)
				continue;

			pHingeJoint->Create();
		}
		
	}

	bool BulletPhysicsObject::Start()
	{
		InitCollObject();

		InitJoint();

		return true;
	}

	bool BulletPhysicsObject::Stop()
	{
		if (m_vBoxCollisionShape.empty() && m_vSphereCollisionShape.empty() && m_pRigidBody == NULL)
			return false;

		btDiscreteDynamicsWorld* pBtDynamicsWorld = m_pPhyScene->GetDynamicsWorld();
		if (pBtDynamicsWorld == NULL)
			return false;

		pBtDynamicsWorld->removeCollisionObject(m_pBtCollObject);

		BulletRigidBody* pBulletRigidBody = (BulletRigidBody*)m_pRigidBody;
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
		if (m_pBtCollObject)
		{
			m_pBtCollObject->setWorldTransform( ToBulletUnit(tsfWS) );
		}

		m_tsfWS = tsfWS;
	}

	void BulletPhysicsObject::AddCollisionListener(IPhysicsObject* objectB,CollisionListener* pListener)
	{

	}

	void BulletPhysicsObject::AddCollisionListener(ICharaControll* objectB)
	{

	}

	NodeTransform BulletPhysicsObject::GetTransformWS()
	{
		if (m_pBtCollObject)
		{
			return ToMaUnit( m_pBtCollObject->getWorldTransform() );
		}

		return m_tsfWS;
	}

	IRigidBody*				BulletPhysicsObject::CreateRigidBody()
	{
		ASSERT(m_pRigidBody == NULL);

		m_pRigidBody = new BulletRigidBody();
		return m_pRigidBody;
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


	IPhysicsGenericJoint*	BulletPhysicsObject::CreatePhysicsGenericJoint(IPhysicsObject* pPhyObjB)
	{
		BulletPhysicsGenericJoint* pGenericJoint = new BulletPhysicsGenericJoint(this,pPhyObjB);
		m_vGenericJoint.push_back(pGenericJoint);
		return pGenericJoint;
	}


	IPhysicsHingeJoint*		BulletPhysicsObject::CreatePhysicsHingeJoint(IPhysicsObject* pPhyObjB)
	{
		BulletPhysicsHingeJoint* pHingeJoint = new BulletPhysicsHingeJoint(this,pPhyObjB);
		m_vHingeJoint.push_back(pHingeJoint);
		return pHingeJoint;
	}
}
