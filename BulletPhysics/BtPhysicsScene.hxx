#include "BulletPhysics/BtPhysicsScene.h"
#include "BulletPhysics/BulletUtil.h"
#include "BulletPhysics/BtContactReport.h"

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

		return true;
	}

	void BulletScene::Stop()
	{
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

	//------------------------------------------------------------------------------
	//Simulation
	//------------------------------------------------------------------------------
	void		BulletScene::BeginSimulation(float fDeltaTime)
	{
		if (m_pDynamicsWorld == NULL)
			return;

		// SyncToPhysics
		UINT nActNum = m_pDynamicsWorld->getNumCollisionObjects();
		for (UINT i = 0; i < nActNum; ++i)
		{
			btCollisionObject* pObj = m_pDynamicsWorld->getCollisionObjectArray()[i];
			if (pObj == NULL)
				continue;

			if ( IsCharacterController(pObj) )
			{
				BulletCharacterController* pCharControl = (BulletCharacterController*)pObj->getUserPointer();
				if (pCharControl)
				{
					pCharControl->SyncToPhysics();
				}
				continue;
			}

			if ( !pObj->isStaticOrKinematicObject() )
				continue;

			GameObject* pGameObj = (GameObject*)pObj->getUserPointer();
			if (pGameObj)
			{
				pGameObj->SyncToPhysics();
			}
		}

		m_pDynamicsWorld->stepSimulation(fDeltaTime);
	}

	void		BulletScene::EndSimulation()
	{
		//m_pNxScene->fetchResults(NX_RIGID_BODY_FINISHED,true);
		if (m_pDynamicsWorld == NULL)
			return;

		// // SyncFromPhysics
		UINT nActNum = m_pDynamicsWorld->getNumCollisionObjects();
		for (UINT i = 0; i < nActNum; ++i)
		{
			btCollisionObject* pObj = m_pDynamicsWorld->getCollisionObjectArray()[i];
			if (pObj == NULL)
				continue;

			if ( IsCharacterController(pObj) )
			{
				BulletCharacterController* pCharControl = (BulletCharacterController*)pObj->getUserPointer();
				if (pCharControl)
				{
					pCharControl->SyncFromPhysics();
				}
				continue;
			}

			if ( pObj->isStaticOrKinematicObject() )
				continue;

			GameObject* pGameObj = (GameObject*)pObj->getUserPointer();
			if (pGameObj)
			{
				pGameObj->SyncFromPhysics();
			}

		}

		BulletContactReport(m_pDynamicsWorld);
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

		assert(closestResults.m_collisionObject);
		if (closestResults.m_collisionObject == NULL)
			return NULL;

		return (GameObject*)closestResults.m_collisionObject->getUserPointer();

		return NULL;
	}
}
