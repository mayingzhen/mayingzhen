#include "BtPhysicsScene.h"
#include "BulletUtil.h"
#include "BtContactReport.h"


namespace ma
{
	BulletScene::BulletScene()
	{
		m_pCollisionConfiguration = NULL;
		m_pDispatcher = NULL;
		m_pOverlappingPairCache = NULL;
		m_pSolver = NULL;
		m_pDynamicsWorld = NULL;

		m_vGravity = btVector3(0,-9.8f,0);
	}

	bool BulletScene::Start()
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

		return true;
	}

	void BulletScene::Stop()
	{
		for (UINT i = 0; i < m_arrPhysicsObject.size(); ++i)
		{
			m_arrPhysicsObject[i]->Stop();
		}

		SAFE_DELETE(m_pDynamicsWorld);
		SAFE_DELETE(m_pSolver);
		SAFE_DELETE(m_pOverlappingPairCache);
		SAFE_DELETE(m_pDispatcher);
		SAFE_DELETE(m_pCollisionConfiguration);
	}


	void BulletScene::SetLayerCollisionMask(Uint8 nLayer,Uint8 nColLayer,bool bCollide)
	{

	}

	//------------------------------------------------------------------------------
	//Dynamic
	//------------------------------------------------------------------------------
	Vector3	BulletScene::GetGravity() const
	{
		return ToMaUnit( m_pDynamicsWorld->getGravity() );
	}

	void		BulletScene::SetGravity(const Vector3& g)
	{
		if (m_pDynamicsWorld)
			m_pDynamicsWorld->setGravity( ToBulletUnit(g) );
		m_vGravity = ToBulletUnit(g);
	}


	void		BulletScene::BeginSimulation(float fDeltaTime)
	{
		if (m_pDynamicsWorld == NULL)
			return;

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

		m_pDynamicsWorld->stepSimulation(fDeltaTime);
	}

	void		BulletScene::EndSimulation()
	{
		if (m_pDynamicsWorld == NULL)
			return;

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

	GameObject* 	BulletScene::RayCastCloseGameObj(const Vector3& rayOrig, const Vector3& rayDir, int nCollLayer,Vector3& hitPosWS)
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

	IPhysicsObject*	BulletScene::CreatePhysicsObject(GameObject* pGameObj)
	{
		ASSERT(pGameObj);
		if (pGameObj == NULL)
			return NULL;
		
		BulletPhysicsObject* pPhysicObj = new BulletPhysicsObject(pGameObj,this);
		m_arrPhysicsObject.push_back(pPhysicObj);
		return pPhysicObj;
	}

	IPhysicsGenericJoint*	BulletScene::CreatePhysicsGenericJoint(IPhysicsObject* pPhyObjA,IPhysicsObject* pPhyObjB)
	{
		BulletPhysicsGenericJoint* pGenericJoint = new BulletPhysicsGenericJoint((BulletPhysicsObject*)pPhyObjA,(BulletPhysicsObject*)pPhyObjB);
		m_vGenericJoint.push_back(pGenericJoint);
		return pGenericJoint;
	}


	IPhysicsHingeJoint*		BulletScene::CreatePhysicsHingeJoint(IPhysicsObject* pPhyObjA,IPhysicsObject* pPhyObjB)
	{
		BulletPhysicsHingeJoint* pHingeJoint = new BulletPhysicsHingeJoint((BulletPhysicsObject*)pPhyObjA,(BulletPhysicsObject*)pPhyObjB);
		m_vHingeJoint.push_back(pHingeJoint);
		return pHingeJoint;
	}

}
