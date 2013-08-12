#include "BtCharacterController.h"
#include "BulletUtil.h"
#include "BtPhysicsSystem.h"
#include "BtCollisionShape.h"
#include "BtPhysicsObject.h"

namespace ma
{
	BulletCharacterController::BulletCharacterController(BulletPhysicsObject* pPhysicsObject)
	{
		m_ghostObject = new btPairCachingGhostObject();
		m_collShape = new btCompoundShape();
		//m_character = NULL;
		m_fSetpOffset = 0.5f;
		m_pCapsuleShape = NULL;
		m_pPhysicsObject = pPhysicsObject;

		 m_pCapsuleShape = new BulletCapsuleCollisionShape();

		 m_touchSkin = 0.01f;
		 m_bTouched = false;
		 memset(&m_touchNorm,0,sizeof(m_touchNorm));
	
		m_colState = CF_None;	
	}	


	BulletCharacterController::~BulletCharacterController()
	{
		SAFE_DELETE(m_pCapsuleShape);
	}

	ICapsuleCollisionShape* BulletCharacterController::GetCollisionShape()
	{
		ASSERT(m_pCapsuleShape);
		if (m_pCapsuleShape == NULL)
			m_pCapsuleShape = new BulletCapsuleCollisionShape();

		return m_pCapsuleShape;
	}

	void BulletCharacterController::SetStepOffset(float fStepOffset)
	{
		m_fSetpOffset = fStepOffset;
	}

	float BulletCharacterController::GetStepOffset()
	{
		return m_fSetpOffset;
	}

	bool BulletCharacterController::IsGrounded() const
	{
		return m_colState == CF_Above;
	}

	btCollisionObject* BulletCharacterController::Start()
	{
		btDiscreteDynamicsWorld* pbtWorld = GetDynamicsWorld();
		ASSERT(pbtWorld);
		if (pbtWorld == NULL)
			return NULL;

		//btCompoundShape* pCompoundShape = new btCompoundShape();
		btCapsuleShape* pCapsule = new btCapsuleShape(m_pCapsuleShape->GetRadius(),m_pCapsuleShape->GetHeight());
		btTransform btTsfLs = ToBulletUnit(m_pCapsuleShape->GetTransformLS());
		m_collShape->addChildShape(btTsfLs,pCapsule);
		m_ghostObject->setCollisionShape(m_collShape);
		m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		//m_ghostObject->setUserPointer(m_pPhysicsObject);
		m_ghostObject->setWorldTransform( ToBulletUnit( m_pPhysicsObject->GetGameObject()->GetSceneNode()->GetTransform() ) );
		
		//m_character = new btKinematicCharacterController(m_ghostObject,pCapsule/*pCompoundShape*/,m_fSetpOffset);
		//m_character->setGravity(true);
		
		pbtWorld->addCollisionObject(m_ghostObject,btBroadphaseProxy::CharacterFilter,btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
		//pbtWorld->addCharacter(m_character);
		pbtWorld->addAction(this);

		
		return m_ghostObject;
	}

	void BulletCharacterController::Stop()
	{
		btDiscreteDynamicsWorld* pbtWorld = GetDynamicsWorld();
		ASSERT(pbtWorld);
		if (pbtWorld == NULL)
			return;

		//if (m_character)
		//{
			pbtWorld->removeCollisionObject(m_ghostObject);
		//}
	}

	void BulletCharacterController::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
	{
		RecoverFromPenetration(3);
	}

// 	void BulletCharacterController::MoveImpl(const Vector3& motion)
// 	{
// // 		Vector3 vPosLS = m_pCapsuleShape->GetTransformLS().m_vPos;
// // 		Vector3 vMotion = motion + Vector3(0,vPosLS.y,0);
// 		m_character->setWalkDirection(ToBulletUnit(motion));
// 	}

	void BulletCharacterController::DebugRender()
	{
		NodeTransform tsfws = m_pPhysicsObject->GetTransformWS();

		m_pCapsuleShape->DebugRender(tsfws);
	}


	bool BulletCharacterController::RecoverFromPenetration(int nMaxSubStep)
	{
		// Here we must refresh the overlapping paircache as the penetrating movement itself or the
		// previous recovery iteration might have used setWorldTransform and pushed us into an object
		// that is not in the previous cache contents from the last timestep, as will happen if we
		// are pushed into a new AABB overlap. Unhandled this means the next convex sweep gets stuck.
		//
		// Do this by calling the broadphase's setAabb with the moved AABB, this will update the broadphase
		// paircache and the ghostobject's internal paircache at the same time.    /BW

		bool bPenetrate = true;

		for (int nStepCnt = 0; nStepCnt < nMaxSubStep && bPenetrate; ++nStepCnt)
		{
			bool bCurPenetrate = false;

			m_bTouched = false;
			memset(&m_touchNorm,0,sizeof(m_touchNorm));

			btCollisionWorld* collisionWorld = GetDynamicsWorld();


			btVector3 minAabb, maxAabb;
			m_collShape->getAabb(m_ghostObject->getWorldTransform(), minAabb,maxAabb);
			collisionWorld->getBroadphase()->setAabb(m_ghostObject->getBroadphaseHandle(), 
				minAabb, 
				maxAabb, 
				collisionWorld->getDispatcher());


			collisionWorld->getDispatcher()->dispatchAllCollisionPairs(m_ghostObject->getOverlappingPairCache(), collisionWorld->getDispatchInfo(), collisionWorld->getDispatcher());

			btVector3 curPos = m_ghostObject->getWorldTransform().getOrigin();

			btScalar maxPen = btScalar(0.0);
			for (int i = 0; i < m_ghostObject->getOverlappingPairCache()->getNumOverlappingPairs(); i++)
			{
				m_manifoldArray.resize(0);

				btBroadphasePair* collisionPair = &m_ghostObject->getOverlappingPairCache()->getOverlappingPairArray()[i];

				if (collisionPair->m_algorithm)
					collisionPair->m_algorithm->getAllContactManifolds(m_manifoldArray);


				for (int j=0;j<m_manifoldArray.size();j++)
				{
					btPersistentManifold* manifold = m_manifoldArray[j];

					const btCollisionObject* otherObj = manifold->getBody0() == m_ghostObject ? manifold->getBody1() : manifold->getBody0();
					if (otherObj->hasContactResponse()) //test if it is not a trigger
					{
						btScalar directionSign = manifold->getBody0() == m_ghostObject ? btScalar(-1.0) : btScalar(1.0);
						for (int p=0;p<manifold->getNumContacts();p++)
						{
							const btManifoldPoint&pt = manifold->getContactPoint(p);

							btScalar dist = pt.getDistance();

							if (dist < 0.0)
							{
								btVector3 peneDepth = pt.m_normalWorldOnB * directionSign * dist;
								if (dist < -m_touchSkin)
								{
									curPos += peneDepth * btScalar(0.2);
									bCurPenetrate = true;
								}

								m_touchNorm.x += peneDepth.x();
								m_touchNorm.y += peneDepth.y();
								m_touchNorm.z += peneDepth.z();
								m_bTouched = true;
							}
						}
					}
				}
			}

			//Update touch state			
			m_colState = CF_None;
			if (m_bTouched)
			{
				float xz = m_touchNorm.x * m_touchNorm.x + m_touchNorm.z * m_touchNorm.z;
				float yy = m_touchNorm.y * m_touchNorm.y;
				if (xz < yy)
				{
					if (btFabs(m_touchNorm.y) > FEPS)
					{
						m_colState = m_touchNorm.y > 0.0f ? CF_Above : CF_Below;
					}
				}else{
					if (sqrtf(xz) > FEPS)
					{
						m_colState = CF_Side;
					}
				}
			}


			if (bCurPenetrate)
			{
				btTransform newTrans = m_ghostObject->getWorldTransform();
				newTrans.setOrigin(curPos);
				m_ghostObject->setWorldTransform(newTrans);
			}

			bPenetrate = bCurPenetrate;

		}

		return bPenetrate;
	}

}
