#include "BulletPhysics/BtPhysicsSystem.h"

namespace ma
{
	BtPhysicsSystem::BtPhysicsSystem()
	{
		m_pDynamicsWorld = NULL;
	}

	void BtPhysicsSystem::Init()
	{
	}

	void BtPhysicsSystem::ShoutDown()
	{
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
			m_vGenericJoint[i]->Create();
			m_pDynamicsWorld->addConstraint(m_vGenericJoint[i]->GetBtConstraint(),true);
		}

		for (UINT i = 0; i < m_vHingeJoint.size(); ++i)
		{
			m_vHingeJoint[i]->Create();
			m_pDynamicsWorld->addConstraint(m_vHingeJoint[i]->GetBtConstraint(),true);
		}
	}

	void BtPhysicsSystem::Stop()
	{
		for (UINT i = 0; i < m_arrPhysicsObject.size(); ++i)
		{
			m_arrPhysicsObject[i]->Stop();
		}

		BulletContactReport::ClearCollisionListener();

		SAFE_DELETE(m_pDynamicsWorld);
		SAFE_DELETE(m_pSolver);
		SAFE_DELETE(m_pOverlappingPairCache);
		SAFE_DELETE(m_pDispatcher);
		SAFE_DELETE(m_pCollisionConfiguration);
	}

	void BtPhysicsSystem::Update()
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


		m_pDynamicsWorld->stepSimulation(GetTimer()->GetFrameDeltaTime());


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

	IPhysicsGenericJoint*	BtPhysicsSystem::CreatePhysicsGenericJoint(IPhysicsObject* pPhyObjA,IPhysicsObject* pPhyObjB)
	{
		BulletPhysicsGenericJoint* pGenericJoint = new BulletPhysicsGenericJoint((BulletPhysicsObject*)pPhyObjA,(BulletPhysicsObject*)pPhyObjB);
		m_vGenericJoint.push_back(pGenericJoint);
		return pGenericJoint;
	}


	IPhysicsHingeJoint*		BtPhysicsSystem::CreatePhysicsHingeJoint(IPhysicsObject* pPhyObjA,IPhysicsObject* pPhyObjB)
	{
		BulletPhysicsHingeJoint* pHingeJoint = new BulletPhysicsHingeJoint((BulletPhysicsObject*)pPhyObjA,(BulletPhysicsObject*)pPhyObjB);
		m_vHingeJoint.push_back(pHingeJoint);
		return pHingeJoint;
	}


}

