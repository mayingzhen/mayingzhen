#ifndef __BulletCharacterController_H__
#define __BulletCharacterController_H__


#include "Engine/Physics/ICharacterController.h"


namespace ma
{
	class BulletPhysicsObject;

	class BulletCharacterController : public ICharaControll , public btActionInterface
	{
	public:
		BulletCharacterController(GameObject* pGameObj/*BulletPhysicsObject* pPhysicsObject*/);

		~BulletCharacterController();

		//virtual void						SetCollisionShape(ICapsuleCollisionShape* pCapsule) = 0;

		virtual ICapsuleCollisionShape*		GetCollisionShape();

		virtual void						SetStepOffset(float fStepOffset);

		virtual float						GetStepOffset();

		virtual bool						IsGrounded() const;	

		virtual btCollisionObject*			Start();

		virtual void						Stop();

		//void								MoveImpl(const Vector3& motion);

		virtual void						updateAction( btCollisionWorld* collisionWorld, btScalar deltaTimeStep);
		
		virtual void						debugDraw(btIDebugDraw* debugDrawer) {}

		void								DebugRender();

	private:
		bool								RecoverFromPenetration(int nMaxSubStep);


	private:
		
		//btKinematicCharacterController*		m_character;
		btPairCachingGhostObject*			m_ghostObject;
		btCompoundShape*					m_collShape;
		
		float								m_fSetpOffset;

		BulletCapsuleCollisionShape*		m_pCapsuleShape;
		
		BulletPhysicsObject*				m_pPhysicsObject;

		float								m_touchSkin;	//Margin used for stay on ground
		bool								m_bTouched;
		Vector3								m_touchNorm;
		btManifoldArray						m_manifoldArray;

		CollisionFlags						m_colState;
	};


	inline bool IsCharacterController(const btCollisionObject* pObj)
	{
		if (pObj == NULL)
			return false;
		
		int collFlags = pObj->getCollisionFlags();
		return BitFieldBase<int>::StaticGetBit(collFlags,btCollisionObject::CF_CHARACTER_OBJECT);
	}
}


#endif