#include "BulletPhysics/BtPhysicsScene.h"

#include "BulletPhysics/BulletUtil.h"

namespace ma
{
	void BtPhysicsScene::Start()
	{
		m_pCollisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		m_pDispatcher = new	btCollisionDispatcher(m_pCollisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		m_pOverlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		m_pSolver = new btSequentialImpulseConstraintSolver();

		m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher,m_pOverlappingPairCache,m_pSolver,m_pCollisionConfiguration);

		return true;
	}

	void BtPhysicsScene::Stop()
	{
		SAFE_DELETE(m_pDynamicsWorld);
		SAFE_DELETE(m_pSolver);
		SAFE_DELETE(m_pOverlappingPairCache);
		SAFE_DELETE(m_pDispatcher);
		SAFE_DELETE(m_pCollisionConfiguration);
	}

	void BtPhysicsScene::BeginSimulation()
	{
		m_pDynamicsWorld->stepSimulation( GetTimer()->GetFrameDeltaTime() );
	}

	void BtPhysicsScene::EndSimulation()
	{

	}

	D3DXVECTOR3	BtPhysicsScene::GetGravity() const
	{
		return ToMaUnit( m_pDynamicsWorld->getGravity() );
	}

	void		BtPhysicsScene::SetGravity(const D3DXVECTOR3& g)
	{
		m_pDynamicsWorld->setGravity( ToBulletUnit(g) );
	}

	//------------------------------------------------------------------------------
	//Simulation
	//------------------------------------------------------------------------------
	void		PhysicsScene::BeginSimulation()
	{
// 		if (m_pDynamicsWorld == NULL)
// 			return;
// 
// 		xmUint nActNum = m_pDynamicsWorld->getNumCollisionObjects();
// 		for (xmUint i = 0; i < nActNum; ++i)
// 		{
// 			btCollisionObject* pObj = m_pDynamicsWorld->getCollisionObjectArray()[i];
// 			if (pObj == NULL)
// 				continue;
// 
// 			if (!pObj->isStaticOrKinematicObject())
// 			{
// 				// SyncFromScene
// 				BulletRigidBodyComponent* pRigidComp = (BulletRigidBodyComponent*)pObj->getUserPointer();
// 				//GameObject* pGameObj = pRigidComp->GetGameObject();
// 				//maNodeTransform tsfWS = pGameObj->GetTransformWS();
// 				//pGameObj->GetPhysicsActor()->SetTransformWS(tsfWS);
// 				//BulletRigidBodyComponent* pRigidComp = pGameObj ? pGameObj->FindComponent<PhysXRigidBodyComponent>() : NULL;
// 				if (NULL != pRigidComp)
// 				{
// 					pRigidComp->SyncFromScene();
// 				}
// 			}
// 		}

		m_pDynamicsWorld->stepSimulation( GetTimer()->GetFrameDeltaTime() );
	}

	void		PhysicsScene::EndSimulation()
	{
		//m_pNxScene->fetchResults(NX_RIGID_BODY_FINISHED,true);
// 		if (m_pDynamicsWorld == NULL)
// 			return;
// 
// 		xmUint nActNum = m_pDynamicsWorld->getNumCollisionObjects();
// 		for (xmUint i = 0; i < nActNum; ++i)
// 		{
// 			btCollisionObject* pObj = m_pDynamicsWorld->getCollisionObjectArray()[i];
// 			if (pObj == NULL)
// 				continue;
// 
// 			if (!pObj->isStaticOrKinematicObject())
// 			{
// 				// SyncToScene
// 				BulletRigidBodyComponent* pRigidComp = (BulletRigidBodyComponent*)pObj->getUserPointer();
// 				//pRigidComp->SyncToScene();
// 				//S3XGameObject* pGameObj = (S3XGameObject*)pObj->getUserPointer();
// 				//S3XSceneNode* pSceneNode = pGameObj->GetSceneNode();
// 				//maNodeTransform tsfWS = pGameObj->GetPhysicsActor()->GetTransformWS();
// 				//pSceneNode->SetTransformWS(&tsfWS);
// 
// 				//PhysXRigidBodyComponent* pRigidComp = pGameObj ? pGameObj->FindComponent<PhysXRigidBodyComponent>() : NULL;
// 				if (NULL != pRigidComp)
// 				{
// 					pRigidComp->SyncToScene();
// 				}
// 			}
// 		}
	}
}