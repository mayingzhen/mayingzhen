#ifndef __ICharacterController_H__
#define __ICharacterController_H__


namespace ma
{
	class ICapsuleCollisionShape;

	enum CollisionFlags
	{
		CF_None = 0,
		CF_Side,
		CF_Above,
		CF_Below,
	};

	class ENGINE_API ICharaControll : public Component
	{
		DECL_OBJECT(ICharaControll)

	public:
		ICharaControll(GameObject* pGameObj);

		virtual ICapsuleCollisionShape*		GetCollisionShape() = 0;

		virtual void						SetStepOffset(float fStepOffset) = 0;

		virtual float						GetStepOffset() = 0;

		virtual bool						IsGrounded() const = 0;

	};
}


#endif //__CharacterController_H__
