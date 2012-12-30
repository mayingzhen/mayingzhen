#include "BulletPhysics/BtPhysicsScene.h"

#include "BulletPhysics/BulletUtil.h"

namespace ma
{
	BtPhysicsScene::BtPhysicsScene()
	{
		m_pCollisionConfiguration = NULL;
		m_pDispatcher = NULL;
		m_pOverlappingPairCache = NULL;
		m_pSolver = NULL;
		m_pDynamicsWorld = NULL;
	}

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

		//return true;
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
		if (m_pDynamicsWorld)
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
}