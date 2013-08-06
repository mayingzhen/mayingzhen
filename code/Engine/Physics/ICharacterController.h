#ifndef __ICharacterController_H__
#define __ICharacterController_H__


namespace ma
{
	class ICapsuleCollisionShape;

	class ENGINE_API ICharaControll
	{
	public:
		//virtual void						SetCollisionShape(ICapsuleCollisionShape* pCapsule) = 0;

		virtual ICapsuleCollisionShape*		GetCollisionShape() = 0;

		virtual void						SetStepOffset(float fStepOffset) = 0;

		virtual float						GetStepOffset() = 0;

		virtual bool						IsGrounded() const = 0;

	};
}


#endif //__CharacterController_H__
