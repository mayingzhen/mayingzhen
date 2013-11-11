#include "BulletPhysics/BtPhysicsSystem.h"
#include "BulletPhysics/BtPhysicsThread.h"

namespace ma
{
	BtPhysicsSystem::BtPhysicsSystem()
	{
		m_pDynamicsWorld = NULL;
		m_pPhysicsThread = NULL;
		m_vGravity = btVector3(0,-9.8f,0);
	}

	void BtPhysicsSystem::Init()
	{
		m_pPhysicsThread = new PhysicsThread();
        m_pPhysicsThread->Start();
	}

	void BtPhysicsSystem::ShoutDown()
	{
        m_pPhysicsThread->Stop();
        SAFE_DELETE(m_pPhysicsThread);
	}

	void BtPhysicsSystem::Start()
	{
		m_pCollisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		m_pDispatcher = new	btCollisionDispatcher(m_pCollisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		m_pOverlappingPairCache = new btDbvtBroadphase();
		m_pOverlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		m_pSolver = new btSequentialImpulseConstraintSolver;

		m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher,m_pOverlappingPairCache,m_pSolver,m_pCollisionConfiguration);

		m_pDynamicsWorld->setGravity(m_vGravity);

		for (UINT i = 0; i < m_arrPhysicsObject.size(); ++i)
		{
			m_arrPhysicsObject[i]->Start();
		}

		// Joint
		for (UINT i = 0; i < m_vGenericJoint.size(); ++i)
		{
			m_vGenericJoint[i]->Start();
			m_pDynamicsWorld->addConstraint(m_vGenericJoint[i]->GetBtConstraint(),true);
		}

		for (UINT i = 0; i < m_vHingeJoint.size(); ++i)
		{
			m_vHingeJoint[i]->Start();
			m_pDynamicsWorld->addConstraint(m_vHingeJoint[i]->GetBtConstraint(),true);
		}
	}

	void BtPhysicsSystem::Stop()
	{
		for (UINT i = 0; i < m_arrPhysicsObject.size(); ++i)
		{
			m_arrPhysicsObject[i]->Stop();
		}

		for (UINT i = 0; i < m_vGenericJoint.size(); ++i)
		{
			m_vGenericJoint[i]->Stop();
		}

		for (UINT i = 0; i < m_vHingeJoint.size(); ++i)
		{
			m_vHingeJoint[i]->Stop();
		}

		BulletContactReport::ClearCollisionListener();

		SAFE_DELETE(m_pDynamicsWorld);
		SAFE_DELETE(m_pSolver);
		SAFE_DELETE(m_pOverlappingPairCache);
		SAFE_DELETE(m_pDispatcher);
		SAFE_DELETE(m_pCollisionConfiguration);
	}

	void BtPhysicsSystem::BeginUpdate()
	{
		UINT nActNum = m_pDynamicsWorld->getNumCollisionObjects();
		for (UINT i = 0; i < nActNum; ++i)
		{
			btCollisionObject* pObj = m_pDynamicsWorld->getCollisionObjectArray()[i];
			if (pObj == NULL)
				continue;

			BulletPhysicsObject* pPhysicsObj = (BulletPhysicsObject*)pObj->getUserPointer();
			if (pPhysicsObj == NULL)
				continue;

			if ( pObj->isKinematicObject()  || IsCharacterController(pObj) )
			{
				pPhysicsObj->SyncToPhysics();
			}
		}	

		m_pPhysicsThread->BeginUpdate();


	}

	void BtPhysicsSystem::EndUpdate() 
	{
		m_pPhysicsThread->EndUpdate();

		UINT nActNum = m_pDynamicsWorld->getNumCollisionObjects();
		for (UINT i = 0; i < nActNum; ++i)
		{
			btCollisionObject* pObj = m_pDynamicsWorld->getCollisionObjectArray()[i];
			if (pObj == NULL)
				continue;

			BulletPhysicsObject* pPhysicsObj = (BulletPhysicsObject*)pObj->getUserPointer();
			if (pPhysicsObj == NULL)
				continue;

			if ( !pObj->isStaticOrKinematicObject() || IsCharacterController(pObj) )
			{
				pPhysicsObj->SyncFromPhysics();
			}
		}

		BulletContactReport::Update();
	}

	void BtPhysicsSystem::DebugRender()
	{
		for (UINT i = 0; i < m_arrPhysicsObject.size(); ++i)
		{
			m_arrPhysicsObject[i]->DebugRender();
		}
	}

	void BtPhysicsSystem::SetLayerCollisionMask(Uint8 nLayer,Uint8 nColLayer,bool bCollide)
	{

	}

	Vector3	BtPhysicsSystem::GetGravity() const
	{
		return ToMaUnit( m_pDynamicsWorld->getGravity() );
	}

	void BtPhysicsSystem::SetGravity(const Vector3& g)
	{
		if (m_pDynamicsWorld)
			m_pDynamicsWorld->setGravity( ToBulletUnit(g) );
		m_vGravity = ToBulletUnit(g);
	}

	IPhysicsObject*	BtPhysicsSystem::CreatePhysicsObject(GameObject* pGameObj)
	{
		ASSERT(pGameObj);
		if (pGameObj == NULL)
			return NULL;

		BulletPhysicsObject* pPhysicObj = new BulletPhysicsObject(pGameObj);
		m_arrPhysicsObject.push_back(pPhysicObj);
		return pPhysicObj;
	}

	void BtPhysicsSystem::DeletePhysicsObject(IPhysicsObject* pPhysicsObject)
	{
		std::vector<BulletPhysicsObject*>::iterator it;
		it = std::find(m_arrPhysicsObject.begin(),m_arrPhysicsObject.end(),pPhysicsObject);
		ASSERT(it != m_arrPhysicsObject.end());
		if (it == m_arrPhysicsObject.end())
		{
			SAFE_DELETE(pPhysicsObject);
			return;
		}

		SAFE_DELETE(pPhysicsObject);
		m_arrPhysicsObject.erase(it);
	}

	IRigidBody*	BtPhysicsSystem::CreateRigidBody(GameObject* pGameObj)
	{
		return pGameObj->GetPhyscisObject()->CreateRigidBody();
	}

	IBoxCollisionShape*	BtPhysicsSystem::CreateBoxCollisionShape(GameObject* pGameObj)
	{
		return pGameObj->GetPhyscisObject()->CreateBoxCollisionShape();
	}

	ISphereCollisionShape* BtPhysicsSystem::CreateSphereCollisionShape(GameObject* pGameObj)
	{
		return pGameObj->GetPhyscisObject()->CreateSphereCollisionShape();
	}

	ICapsuleCollisionShape* BtPhysicsSystem::CreateCapsuleCollisionShape(GameObject* pGameObj)
	{
		return pGameObj->GetPhyscisObject()->CreateCapsuleCollisionShape();
	}

	ICharaControll*	BtPhysicsSystem::CreateCharaControll(GameObject* pGameObj)
	{
		return pGameObj->GetPhyscisObject()->CreateCharaControll();
	}

	void BtPhysicsSystem::DeleteRigidBody(IRigidBody* pRigidBody)
	{
		//pRigidBody->GetGameObject()->GetPhyscisObject()->
	}

	void BtPhysicsSystem::DeleteBoxCollisionShape(IBoxCollisionShape*	pBox)
	{

	}

	void BtPhysicsSystem::DeleteSphereCollisionShape(ISphereCollisionShape* pSphere)
	{

	}

	void BtPhysicsSystem::DeleteCapsuleCollisionShape(ICapsuleCollisionShape* pCapsule)
	{

	}

	void BtPhysicsSystem::DeleteCharaControll(ICharaControll* pCharcontrol)
	{

	}

	void BtPhysicsSystem::DeletePhysicsGenericJoint(IPhysicsGenericJoint* pJoint)
	{
		std::vector<BulletPhysicsGenericJoint*>::iterator it;
		it = std::find(m_vGenericJoint.begin(),m_vGenericJoint.end(),pJoint);
		ASSERT(it != m_vGenericJoint.end());
		if (it == m_vGenericJoint.end())
		{
			SAFE_DELETE(pJoint);
			return;
		}

		SAFE_DELETE(pJoint);
		m_vGenericJoint.erase(it);
	}

	void BtPhysicsSystem::DeletePhysicsHingeJoint(IPhysicsHingeJoint* pJoint)
	{
		std::vector<BulletPhysicsHingeJoint*>::iterator it;
		it = std::find(m_vHingeJoint.begin(),m_vHingeJoint.end(),pJoint);
		ASSERT(it != m_vHingeJoint.end());
		if (it == m_vHingeJoint.end())
		{
			SAFE_DELETE(pJoint);
			return;
		}

		SAFE_DELETE(pJoint);
		m_vHingeJoint.erase(it);
	}

	GameObject* BtPhysicsSystem::RayCastCloseGameObj(const Vector3& rayOrig, const Vector3& rayDir, int nCollLayer,Vector3& hitPosWS)
	{
		btVector3 from = ToBulletUnit(rayOrig);
		btVector3 to = ToBulletUnit(rayOrig + 10000.0f * rayDir);

		MyClosestRayResultCallbackCallback	closestResults(from,to);
		closestResults.m_nTestLayer = nCollLayer;
		m_pDynamicsWorld->rayTest(from,to,closestResults);
		if ( !closestResults.hasHit() )
			return NULL;

		hitPosWS = ToMaUnit(closestResults.m_hitPointWorld);

		const btCollisionObject* pBtObject = closestResults.m_collisionObject;
		ASSERT(pBtObject);
		if (pBtObject == NULL)
			return NULL;

		BulletPhysicsObject* pPhysicsObject = (BulletPhysicsObject*)(pBtObject->getUserPointer());
		return pPhysicsObject ? pPhysicsObject->GetGameObject() : NULL;
	}

	IPhysicsGenericJoint*	BtPhysicsSystem::CreatePhysicsGenericJoint(GameObject* pGameObj)
	{
		BulletPhysicsGenericJoint* pGenericJoint = new BulletPhysicsGenericJoint(pGameObj);
		m_vGenericJoint.push_back(pGenericJoint);
		return pGenericJoint;
	}


	IPhysicsHingeJoint*		BtPhysicsSystem::CreatePhysicsHingeJoint(GameObject* pGameObj)
	{
		BulletPhysicsHingeJoint* pHingeJoint = new BulletPhysicsHingeJoint(pGameObj);
		m_vHingeJoint.push_back(pHingeJoint);
		return pHingeJoint;
	}


}

