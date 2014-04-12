#include "BtPhysicsObject.h"
#include "BtCollisionShape.h"
#include "BtRigidBody.h"
#include "BulletUtil.h"
#include "BtPhysicsJoint.h"
#include "BtCharacterController.h"


namespace ma
{

	BulletPhysicsObject::BulletPhysicsObject(GameObject* pGameObject)
	{
		m_pBtCollObject = NULL;
		m_pRigidBody = NULL;
		m_pCharaControll = NULL;

		m_pGameObject = pGameObject;

		TransformSetIdentity(&m_tsfWS);
	}

	BulletPhysicsObject::~BulletPhysicsObject()
	{
		SAFE_DELETE(m_pCharaControll);
		SAFE_DELETE(m_pRigidBody);

		for (UINT i = 0; i < m_vBoxCollisionShape.size(); ++i)
		{
			SAFE_DELETE(m_vBoxCollisionShape[i]);
		}
		m_vBoxCollisionShape.clear();

		for (UINT i = 0; i < m_vSphereCollisionShape.size(); ++i)
		{
			SAFE_DELETE(m_vSphereCollisionShape[i]);
		}
		m_vSphereCollisionShape.clear();

		for (UINT i = 0; i < m_vCapsuleCollisionShape.size(); ++i)
		{
			SAFE_DELETE(m_vCapsuleCollisionShape[i]);
		}
		m_vCapsuleCollisionShape.clear();
	}

	CollisionMaterial* BulletPhysicsObject::GetCollisionMaterial()
	{
		return &m_material;	
	}

	void BulletPhysicsObject::InitCollObject()
	{
		if (m_vCapsuleCollisionShape.empty() && 
			m_vBoxCollisionShape.empty() && 
			m_vSphereCollisionShape.empty() && 
			m_pRigidBody == NULL)
		{
			return ;
		}
		
		ASSERT(m_pBtCollObject == NULL);

		btDiscreteDynamicsWorld* pBtDynamicsWorld = GetDynamicsWorld();
		if (pBtDynamicsWorld == NULL)
			return ;

		btCompoundShape* pCompoundShape = new btCompoundShape();
		for (UINT i = 0; i < m_vBoxCollisionShape.size(); ++i)
		{
			BulletBoxCollisionShape* pBtBoxCollisionShape = m_vBoxCollisionShape[i];
			ASSERT(pBtBoxCollisionShape);
			if (pBtBoxCollisionShape == NULL)
				continue;

			btBoxShape* pBtShape = new btBoxShape( ToBulletUnit( pBtBoxCollisionShape->GetSize() ) * 0.5f );
			btTransform btTsfLs = ToBulletUnit(pBtBoxCollisionShape->GetTransformLS());
			pCompoundShape->addChildShape(btTsfLs,pBtShape);
		}
		for (UINT i = 0; i < m_vSphereCollisionShape.size(); ++i)
		{
			BulletSphereCollisionShape* pBtSphereCollisionShape = m_vSphereCollisionShape[i];
			ASSERT(pBtSphereCollisionShape);
			if (pBtSphereCollisionShape == NULL)
				continue;

			btSphereShape* pBtShape = new btSphereShape(pBtSphereCollisionShape->GetRadius());
			btTransform btTsfLs = ToBulletUnit(pBtSphereCollisionShape->GetTransformLS());
			pCompoundShape->addChildShape(btTsfLs,pBtShape);
		}
		for (UINT i = 0; i < m_vCapsuleCollisionShape.size(); ++i)
		{
			BulletCapsuleCollisionShape* pBtCapusleCollisionShape = m_vCapsuleCollisionShape[i];
			ASSERT(pBtCapusleCollisionShape);
			if (pBtCapusleCollisionShape == NULL)
				continue;

			float fRadius = pBtCapusleCollisionShape->GetRadius();
			float fHeight = pBtCapusleCollisionShape->GetHeight();
			btCapsuleShape* pBtShape = new btCapsuleShape(fRadius,fHeight);
			btTransform btTsfLs = ToBulletUnit(pBtCapusleCollisionShape->GetTransformLS());
			pCompoundShape->addChildShape(btTsfLs,pBtShape);
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

	}

	void BulletPhysicsObject::InitCharControll()
	{
		if (m_pCharaControll == NULL)
			return;

		m_pBtCollObject = m_pCharaControll->Start();
	}

	void BulletPhysicsObject::InitMaterial()
	{
		if (m_pBtCollObject == NULL)
			return;

		m_pBtCollObject->setRestitution(m_material.m_restitution);
		m_pBtCollObject->setFriction(m_material.m_friction);
		m_pBtCollObject->setRollingFriction(m_material.m_rollingFriction);
	}

	bool BulletPhysicsObject::Start()
	{
		InitCharControll();

		InitCollObject();

		InitMaterial();

		if (m_pBtCollObject)
			m_pBtCollObject->setUserPointer(this);

		SyncToPhysics();

		return true;
	}

	bool BulletPhysicsObject::Stop()
	{
		if (m_vBoxCollisionShape.empty() && m_vSphereCollisionShape.empty() && m_pRigidBody == NULL)
			return false;

		btDiscreteDynamicsWorld* pBtDynamicsWorld = GetDynamicsWorld();
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
		
		//if (m_pCharaControll)
		//{
			//Vector3 vMovePos = m_pGameObject->GetSceneNode()->GetTransform().m_vPos;
			//Vector3 vCharPosPre = ToMaUnit( m_pBtCollObject->getWorldTransform().getOrigin() );
			//Vector3 motion = vMovePos - vCharPosPre;
			//m_pCharaControll->MoveImpl(motion);
		//}
		//else
		//{
			SetTransformWS( pSceneNode->GetTransform() );
		//}
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

		//pSceneNode->SetTransform(GetTransformWS());
		NodeTransform tsfPhysics = GetTransformWS();
		NodeTransform tsfScene = pSceneNode->GetTransform();
		tsfScene.m_qRot = tsfPhysics.m_qRot;
		tsfScene.m_vPos = tsfPhysics.m_vPos;
		pSceneNode->SetTransform(tsfScene);
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
		BulletContactReport::AddCollisionListener(this,(BulletPhysicsObject*)objectB,pListener);
	}

	void BulletPhysicsObject::AddCollisionListener(ICharaControll* objectB)
	{
		//BulletContactReport::RemoveCollisionListener();
	}

	NodeTransform BulletPhysicsObject::GetTransformWS()
	{
		if (m_pBtCollObject)
		{
			return ToMaUnit( m_pBtCollObject->getWorldTransform() );
		}

		return m_tsfWS;
	}

	IRigidBody*	BulletPhysicsObject::CreateRigidBody()
	{
		ASSERT(m_pRigidBody == NULL);

		m_pRigidBody = new BulletRigidBody(m_pGameObject);
		return m_pRigidBody;
	}

	IBoxCollisionShape*		BulletPhysicsObject::CreateBoxCollisionShape()
	{
		BulletBoxCollisionShape* pBoxColl = new BulletBoxCollisionShape(m_pGameObject);
		m_vBoxCollisionShape.push_back(pBoxColl);
		return pBoxColl;
	}

	ISphereCollisionShape*	BulletPhysicsObject::CreateSphereCollisionShape()
	{
		BulletSphereCollisionShape* pSpherColl = new BulletSphereCollisionShape(m_pGameObject);
		m_vSphereCollisionShape.push_back(pSpherColl);
		return pSpherColl;
	}

	ICapsuleCollisionShape* BulletPhysicsObject::CreateCapsuleCollisionShape()
	{
		BulletCapsuleCollisionShape* pCapsuleShape = new BulletCapsuleCollisionShape(m_pGameObject);
		m_vCapsuleCollisionShape.push_back(pCapsuleShape);
		return pCapsuleShape;
	}

	ICharaControll*	BulletPhysicsObject::CreateCharaControll()
	{
		ASSERT(m_pCharaControll == NULL);
		BulletCharacterController* pBtCharater = new BulletCharacterController(m_pGameObject);
		m_pCharaControll = pBtCharater;
		return pBtCharater;
	}

}
