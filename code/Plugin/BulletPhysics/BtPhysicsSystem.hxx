#include "BulletPhysics/BtPhysicsSystem.h"
#include "BulletPhysics/BtPhysicsThread.h"

namespace ma
{
	PhysicsSystem* g_pPhysicsSystem = NULL;

	PhysicsSystem* GetPhysicsSystem()
	{
		return g_pPhysicsSystem;
	}

	PhysicsSystem::PhysicsSystem()
	{
		m_pDynamicsWorld = NULL;
		m_pPhysicsThread = NULL;
		m_vGravity = btVector3(0,0,-9.8f);
		m_debugMode = btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawConstraints | btIDebugDraw::DBG_DrawConstraintLimits;
	}

	PhysicsSystem::~PhysicsSystem()
	{

	}

	void PhysicsSystem::Init()
	{
		m_pPhysicsThread = new PhysicsThread();
        //m_pPhysicsThread->Start();
	}

	void PhysicsSystem::Shoutdown()
	{
        m_pPhysicsThread->Stop();
        SAFE_DELETE(m_pPhysicsThread);
	}

	void PhysicsSystem::Start()
	{
		m_pCollisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		m_pDispatcher = new	btCollisionDispatcher(m_pCollisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		m_pOverlappingPairCache = new btDbvtBroadphase();
		m_pbtGhostPairCallback = new btGhostPairCallback();
		m_pOverlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(m_pbtGhostPairCallback);

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		m_pSolver = new btSequentialImpulseConstraintSolver;

		m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher,m_pOverlappingPairCache,m_pSolver,m_pCollisionConfiguration);
		//m_pDynamicsWorld->setDebugDrawer(this);

		m_pDynamicsWorld->setGravity(m_vGravity);

		for (uint32_t i = 0; i < m_arrPhysicsObject.size(); ++i)
		{
			StartPhysicsObject(m_arrPhysicsObject[i]);
		}
	
		for (uint32_t i = 0; i < m_arrPhysicsObject.size(); ++i)
		{
			SceneNode* pGameObj = m_arrPhysicsObject[i];
			std::vector<PhysicsJoint*> arrJoint;
			pGameObj->GetTypeComponent<PhysicsJoint>(arrJoint);
			for (uint32_t j = 0; j < arrJoint.size(); ++j)
			{
				arrJoint[j]->Start();
			}
		}
	}

	void PhysicsSystem::Stop()
	{
		BulletContactReport::ClearCollisionListener();

		SAFE_DELETE(m_pDynamicsWorld);
		SAFE_DELETE(m_pSolver);
		SAFE_DELETE(m_pOverlappingPairCache);
		SAFE_DELETE(m_pDispatcher);
		SAFE_DELETE(m_pCollisionConfiguration);
		SAFE_DELETE(m_pbtGhostPairCallback);
	}

	btCollisionObject* PhysicsSystem::InitCollObject(SceneNode* pGameObj)
	{
		btCollisionObject* pBtCollObject = NULL;

		CharacterController* pCharControl = pGameObj->GetTypeComponent<CharacterController>();
		if (pCharControl)
		{
			pBtCollObject = pCharControl->Start();
		}
		else
		{
			std::vector<CollisionShape*> arrCollisionShape;
			pGameObj->GetTypeComponent<CollisionShape>(arrCollisionShape);

			RigidBody* pRigidBody = pGameObj->GetTypeComponent<RigidBody>();

			if (arrCollisionShape.empty() && pRigidBody == NULL)
			{
				ASSERT(false);
				return NULL;
			}

			btCompoundShape* pCompoundShape = new btCompoundShape();
			for (uint32_t i = 0; i < arrCollisionShape.size(); ++i)
			{
				btCollisionShape* pBtShape = (btCollisionShape*)arrCollisionShape[i]->Create();
				btTransform btTsfLs = ToBulletUnit(arrCollisionShape[i]->GetTransformLS());
				pCompoundShape->addChildShape(btTsfLs,pBtShape);
			}

			if (pRigidBody)
			{
				pBtCollObject = pRigidBody->CreatebtRigidBody(pCompoundShape,m_pDynamicsWorld);
			}
			else
			{
				pBtCollObject = new btCollisionObject();
				pBtCollObject->setCollisionShape(pCompoundShape);
				m_pDynamicsWorld->addCollisionObject(pBtCollObject);	
			}
		}

		return pBtCollObject;
	}

	void PhysicsSystem::StartPhysicsObject(SceneNode* pGameObj)
	{
		btCollisionObject* pBtCollObject = InitCollObject(pGameObj);

		pBtCollObject->setUserPointer(pGameObj);
		pBtCollObject->setWorldTransform( ToBulletUnit( pGameObj->GetTransformWS() ) );
		pGameObj->SetUserData("btCollisionObject",pBtCollObject);

		CollisionMaterial* pBtCollisionMaterial = pGameObj->GetTypeComponent<CollisionMaterial>();
		if (pBtCollisionMaterial)
		{
			pBtCollisionMaterial->Start(pBtCollObject);
		}
	}

	void PhysicsSystem::BeginUpdate()
	{
		if (m_pDynamicsWorld == NULL)
			return;

		uint32_t nActNum = m_pDynamicsWorld->getNumCollisionObjects();
		for (uint32_t i = 0; i < nActNum; ++i)
		{
			btCollisionObject* pObj = m_pDynamicsWorld->getCollisionObjectArray()[i];
			if (pObj == NULL)
				continue;

			SceneNode* pPhysicsObj = (SceneNode*)pObj->getUserPointer();
			if (pPhysicsObj == NULL)
				continue;

			if ( pObj->isKinematicObject()  || IsCharacterController(pObj) )
			{
				// SyncToPhysics
				pObj->setWorldTransform( ToBulletUnit( pPhysicsObj->GetTransformWS() ) );
			}
		}	

		m_pPhysicsThread->BeginUpdate();
	}

	void PhysicsSystem::EndUpdate() 
	{
		if (m_pDynamicsWorld == NULL)
			return;

		m_pPhysicsThread->EndUpdate();

		uint32_t nActNum = m_pDynamicsWorld->getNumCollisionObjects();
		for (uint32_t i = 0; i < nActNum; ++i)
		{
			btCollisionObject* pObj = m_pDynamicsWorld->getCollisionObjectArray()[i];
			if (pObj == NULL)
				continue;

			SceneNode* pPhysicsObj = (SceneNode*)pObj->getUserPointer();
			if (pPhysicsObj == NULL)
				continue;

			if ( !pObj->isStaticOrKinematicObject() || IsCharacterController(pObj) )
			{
				// SyncFromPhysics
				pPhysicsObj->SetTransformWS( ToMaUnit( pObj->getWorldTransform() ) );
			}
		}

		BulletContactReport::Update();
	}

	void PhysicsSystem::DebugRender()
	{
		if (m_pDynamicsWorld)
			m_pDynamicsWorld->debugDrawWorld();
	}

	void PhysicsSystem::SetLayerCollisionMask(uint8_t nLayer,uint8_t nColLayer,bool bCollide)
	{

	}

	Vector3	PhysicsSystem::GetGravity() const
	{
		return ToMaUnit( m_pDynamicsWorld->getGravity() );
	}

	void PhysicsSystem::SetGravity(const Vector3& g)
	{
		if (m_pDynamicsWorld)
			m_pDynamicsWorld->setGravity( ToBulletUnit(g) );
		m_vGravity = ToBulletUnit(g);
	}

	void PhysicsSystem::AddPhysicsObject(SceneNode* pGameObj)
	{
		std::vector<SceneNode*>::iterator it = std::find(m_arrPhysicsObject.begin(),m_arrPhysicsObject.end(),pGameObj);
		if (it != m_arrPhysicsObject.end())
			return;
		
		m_arrPhysicsObject.push_back(pGameObj);
	}

	SceneNode* PhysicsSystem::RayCastCloseGameObj(const Vector3& rayOrig, const Vector3& rayDir, int nCollLayer,Vector3& hitPosWS)
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

		return (SceneNode*)( pBtObject->getUserPointer() );
	}

	bool PhysicsSystem::isVisible(const btVector3& aabbMin, const btVector3& aabbMax)
	{
		return true;
	}

	void PhysicsSystem::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		ColourValue dcolor(color.getX(),color.getY(),color.getZ(),1.0f);
		LineRender::DrawLine(ToMaUnit(from),ToMaUnit(to),dcolor);
	}

	void PhysicsSystem::reportErrorWarning(const char* warningString)
	{

	}

}

