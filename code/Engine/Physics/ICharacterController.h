#ifndef __ICharacterController_H__
#define __ICharacterController_H__


namespace ma
{
	enum CollisionFlags
	{
		CF_None = 0,
		CF_Side,
		CF_Above,
		CF_Below,
	};

	class GameObject;


	class ENGINE_API ICharaControll
	{
	public:
		virtual void			SetTransformWS(const NodeTransform& tsfWS) = 0;

		virtual NodeTransform	GetTransformWS() = 0;

 		virtual void			SetCenterLS(const Vector3& vCenter) = 0;
 
 		virtual Vector3			GetCenterLS() const = 0;

		virtual void			SetHeight(float fHeight) = 0;

		virtual float			GetHeight() const = 0;

		virtual void			SetRadius(float fRadius) = 0;

		virtual float			GetRadius() const = 0;

		virtual void			SetStepOffset(float fStepOffset) = 0;

		virtual float			GetStepOffset() = 0;

		virtual bool			IsGrounded() const = 0;

		virtual CollisionFlags	GetCollisionState() const = 0;

	};
}


#endif //__CharacterController_H__
