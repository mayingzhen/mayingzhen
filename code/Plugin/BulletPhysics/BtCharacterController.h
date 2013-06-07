#ifndef __BulletCharacterController_H__
#define __BulletCharacterController_H__


#include "Engine/Physics/ICharacterController.h"


class btKinematicCharacterController;
class btPairCachingGhostObject;
class btCapsuleShape;
class btCollisionObject;

namespace ma
{
	class GameObject;

	class BULLETPHYSICS_API BulletCharacterController : public ICharacterController
	{
	public:
		BulletCharacterController();

		~BulletCharacterController();

		virtual void			SetTransformWS(const NodeTransform& tsfWS);

		virtual NodeTransform	GetTransformWS();

		virtual void			SetCenterLS(const Vector3& vCenter);

		virtual Vector3			GetCenterLS() const;

		virtual void			SetHeight(float fHeight);

		virtual float			GetHeight() const;

		virtual void			SetRadius(float fRadius);

		virtual float			GetRadius() const;

		virtual void			SetStepOffset(float fStepOffset);

		virtual float			GetStepOffset();

		virtual bool			IsGrounded() const;

		virtual CollisionFlags	GetCollisionState() const;		

		virtual bool			Start(GameObject* pGameObj);

		virtual void			Stop(GameObject* pGameObj);


		void					SyncToPhysics();

		void					SyncFromPhysics();

		GameObject*				GetGameObject() {return m_pGameObject;}

		int						GetColllLayer() {return m_nCollLayer;}

	protected:

		CollisionFlags MoveImpl(const Vector3& motion);

	private:
		GameObject* m_pGameObject;
		btKinematicCharacterController* m_character;
		btPairCachingGhostObject* m_ghostObject;
		
		btCapsuleShape*	m_capsule;
		float m_fHeight;
		float m_fRadius;
		float m_fSetpOffset;

		Vector3 m_vCenter;

		int m_nCollLayer;
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
