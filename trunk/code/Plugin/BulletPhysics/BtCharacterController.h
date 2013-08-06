#ifndef __BulletCharacterController_H__
#define __BulletCharacterController_H__


#include "Engine/Physics/ICharacterController.h"


namespace ma
{
	class BulletPhysicsObject;

	class BulletCharacterController : public ICharaControll
	{
	public:
		BulletCharacterController(BulletPhysicsObject* pPhysicsObject);

		~BulletCharacterController();

		//virtual void						SetCollisionShape(ICapsuleCollisionShape* pCapsule) = 0;

		virtual ICapsuleCollisionShape*		GetCollisionShape();

		virtual void						SetStepOffset(float fStepOffset);

		virtual float						GetStepOffset();

		virtual bool						IsGrounded() const;	

		virtual btCollisionObject*			Start();

		virtual void						Stop();

		void								MoveImpl(const Vector3& motion);

	private:
		
		btKinematicCharacterController*		m_character;
		btPairCachingGhostObject*			m_ghostObject;
		
		float								m_fSetpOffset;

		BulletCapsuleCollisionShape*		m_pCapsuleShape;
		
		BulletPhysicsObject*				m_pPhysicsObject;
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
