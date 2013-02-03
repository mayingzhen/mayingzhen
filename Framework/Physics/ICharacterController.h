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


	class FRAMEWORK_API ICharacterController
	{
	public:
		virtual void			SetTransformWS(const maNodeTransform& tsfWS) = 0;

		virtual maNodeTransform GetTransformWS() = 0;

 		virtual void			SetCenterLS(const D3DXVECTOR3& vCenter) = 0;
 
 		virtual D3DXVECTOR3		GetCenterLS() const = 0;

		virtual void			SetHeight(float fHeight) = 0;

		virtual float			GetHeight() const = 0;

		virtual void			SetRadius(float fRadius) = 0;

		virtual float			GetRadius() const = 0;

		virtual void			SetStepOffset(float fStepOffset) = 0;

		virtual float			GetStepOffset() = 0;

		virtual bool			IsGrounded() const = 0;

		virtual CollisionFlags	GetCollisionState() const = 0;

		virtual bool			Start(GameObject* pGameObj) = 0;

		virtual void			Stop(GameObject* pGameObj) = 0;

	};
}


#endif //__xmCharacterController_H__
