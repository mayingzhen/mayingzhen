#include "BtCharacterController.h"
#include "BulletUtil.h"
#include "BtPhysicsScene.h"
#include "BtPhysicsDevive.h"
#include "BtCollisionShape.h"
#include "BtPhysicsObject.h"

namespace ma
{
	BulletCharacterController::BulletCharacterController(BulletPhysicsObject* pPhysicsObject)
	{
		m_ghostObject = new btPairCachingGhostObject();
		m_character = NULL;
		m_fSetpOffset = 0.5f;
		m_pCapsuleShape = NULL;
		m_pPhysicsObject = pPhysicsObject;

		 m_pCapsuleShape = new BulletCapsuleCollisionShape();
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
		return m_character->onGround();
	}

	btCollisionObject* BulletCharacterController::Start()
	{
		BulletScene* pBulletScene = ((BulletDevice*)GetPhysicsDevice())->GetPhysicsScene();
		btDiscreteDynamicsWorld* pbtWorld = pBulletScene ? pBulletScene->GetDynamicsWorld() : NULL;
		ASSERT(pbtWorld);
		if (pbtWorld == NULL)
			return NULL;

		btCompoundShape* pCompoundShape = new btCompoundShape();
		btCapsuleShape* pCapsule = new btCapsuleShape(m_pCapsuleShape->GetRadius(),m_pCapsuleShape->GetHeight());
		btTransform btTsfLs = ToBulletUnit(m_pCapsuleShape->GetTransformLS());
		pCompoundShape->addChildShape(btTsfLs,pCapsule);
		m_ghostObject->setCollisionShape(pCompoundShape);
		m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		//m_ghostObject->setUserPointer(m_pPhysicsObject);
		m_ghostObject->setWorldTransform( ToBulletUnit( m_pPhysicsObject->GetGameObject()->GetSceneNode()->GetTransform() ) );
		
		m_character = new btKinematicCharacterController(m_ghostObject,pCapsule/*pCompoundShape*/,m_fSetpOffset);
		m_character->setGravity(1);
		
		pbtWorld->addCollisionObject(m_ghostObject,btBroadphaseProxy::CharacterFilter,btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
		pbtWorld->addCharacter(m_character);

		
		return m_ghostObject;
	}

	void BulletCharacterController::Stop()
	{
		BulletScene* pBulletScene = ((BulletDevice*)GetPhysicsDevice())->GetPhysicsScene();
		btDiscreteDynamicsWorld* pbtWorld = pBulletScene ? pBulletScene->GetDynamicsWorld() : NULL;
		ASSERT(pbtWorld);
		if (pbtWorld == NULL)
			return;

		if (m_character)
		{
			pbtWorld->removeCollisionObject(m_ghostObject);
		}
	}

	void BulletCharacterController::MoveImpl(const Vector3& motion)
	{
		m_character->setWalkDirection(ToBulletUnit(motion));
	}


}
