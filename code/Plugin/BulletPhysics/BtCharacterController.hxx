#include "BtCharacterController.h"
#include "BulletUtil.h"
#include "BtPhysicsScene.h"

namespace ma
{
	BulletCharacterController::BulletCharacterController(GameObject* pGameObject,BulletScene*	pPhyScene)
	{
		m_ghostObject = new btPairCachingGhostObject();
		m_character = NULL;
		m_capsule = NULL;
		m_fHeight = 0;
		m_fRadius = 0;
		m_fSetpOffset = 0.5f;
		m_nCollLayer = 3;

		m_pGameObject = pGameObject;
		m_pPhyScene = pPhyScene;		
	}	

	BulletCharacterController::~BulletCharacterController()
	{

	}

	void			BulletCharacterController::SetTransformWS(const NodeTransform& tsfWS)
	{
		NodeTransform tsfTemp = tsfWS;
		tsfTemp.m_vPos = tsfTemp.m_vPos + m_vCenter;
		m_ghostObject->setWorldTransform( ToBulletUnit(tsfTemp) );
	}

	NodeTransform BulletCharacterController::GetTransformWS()
	{
		NodeTransform tsfWS = ToMaUnit( m_ghostObject->getWorldTransform() );
		tsfWS.m_vPos = tsfWS.m_vPos - m_vCenter;
		return tsfWS;
	}

	void			BulletCharacterController::SetCenterLS(const Vector3& vCenter)
	{
		m_vCenter = vCenter;
	}

	Vector3		BulletCharacterController::GetCenterLS() const
	{
		return m_vCenter;
	}

	void			BulletCharacterController::SetHeight(float fHeight)
	{
		m_fHeight = fHeight;
	}

	float			BulletCharacterController::GetHeight() const
	{
		return m_fHeight;
	}

	void			BulletCharacterController::SetRadius(float fRadius)
	{
		m_fRadius = fRadius;
	}

	float			BulletCharacterController::GetRadius() const
	{
		return m_fRadius;
	}

	void			BulletCharacterController::SetStepOffset(float fStepOffset)
	{
		m_fSetpOffset = fStepOffset;
	}

	float			BulletCharacterController::GetStepOffset()
	{
		return m_fSetpOffset;
	}

	bool			BulletCharacterController::IsGrounded() const
	{
		return m_character->onGround();
	}

	CollisionFlags	BulletCharacterController::GetCollisionState() const
	{
		return CF_None;
	}

	bool BulletCharacterController::Start()
	{
		btDiscreteDynamicsWorld* pbtWorld = m_pPhyScene->GetDynamicsWorld();
		if (pbtWorld == NULL)
			return false;

		m_capsule = new btCapsuleShape(m_fRadius,m_fHeight);
		SetTransformWS(m_pGameObject->GetSceneNode()->GetTransform(TS_WORLD));
		//m_ghostObject->setWorldTransform(pGameObj->GetSceneNode()->GetTransformWS());
		m_ghostObject->setCollisionShape(m_capsule);
		m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		m_ghostObject->setUserPointer(this);
		
		m_character = new btKinematicCharacterController(m_ghostObject,m_capsule,m_fSetpOffset);
		m_character->setGravity(0);
		
		pbtWorld->addCollisionObject(m_ghostObject,btBroadphaseProxy::CharacterFilter,btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
		pbtWorld->addCharacter(m_character);

		SyncToPhysics();
		
		return true;
	}

	void BulletCharacterController::Stop()
	{
		btDiscreteDynamicsWorld* pbtWorld = m_pPhyScene->GetDynamicsWorld();
		if (pbtWorld == NULL)
			return;

		if (m_character)
		{
			pbtWorld->removeCollisionObject(m_ghostObject);
		}
	}

	void BulletCharacterController::SyncToPhysics()
	{
		if (m_pGameObject == NULL)
			return;

		Vector3 vMovePos = m_pGameObject->GetSceneNode()->GetPosition(TS_WORLD) + m_vCenter;

		Vector3 vCharPosPre = ToMaUnit(m_ghostObject->getWorldTransform().getOrigin());
		Vector3 motion =vMovePos - vCharPosPre;

		MoveImpl(motion);
	}

	void BulletCharacterController::SyncFromPhysics()
	{
		if (m_pGameObject == NULL)
			return;

		Vector3 charPos = ToMaUnit(m_ghostObject->getWorldTransform().getOrigin());
		m_pGameObject->GetSceneNode()->SetPosition(charPos,TS_WORLD);
		//Vector3 vPosWSNew = charPos - m_vCenter;
		//Vector3 vPosWSOld = m_pGameObject->GetPositionWS();
		//Vector3 vDirWs = vPosWSNew - vPosWSOld;
		//m_pGameObject->TranslateWS(vDirWs);
	}

	CollisionFlags BulletCharacterController::MoveImpl(const Vector3& motion)
	{
		m_character->setWalkDirection(ToBulletUnit(motion));
		return CF_None;
	}
}
