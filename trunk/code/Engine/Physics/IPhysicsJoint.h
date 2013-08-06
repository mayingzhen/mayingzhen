#ifndef  _IPhysicsJoint__H__
#define  _IPhysicsJoint__H__

namespace ma
{
	class ENGINE_API IPhysicsJoint
	{
	public:
		virtual float			GetBreakingImpulse() = 0;

		virtual void			SetBreakingImpulse(float impulse) = 0;

		virtual bool			IsEnabled() const = 0;

		virtual void			SetEnabled(bool enabled) = 0;

		virtual NodeTransform	GetATransformLS() = 0;

		virtual void			SetATransformLS(const NodeTransform& tsfLS) = 0;

		virtual NodeTransform	GetBTransformLS() = 0;

		virtual void			SetBTransformLS(const NodeTransform& tsfLS) = 0;

		virtual void			SetPysicsObjectB(IPhysicsObject* pPhyObjB) = 0;
		
	};

	class ENGINE_API IPhysicsGenericJoint : public IPhysicsJoint
	{
	public:
		virtual void			SetAngularLowerLimit(const Vector3& limits) = 0;
 
		virtual void			SetAngularUpperLimit(const Vector3& limits) = 0;
    
		virtual void			SetLinearLowerLimit(const Vector3& limits) = 0;
    
		virtual void			SetLinearUpperLimit(const Vector3& limits) = 0;
	};


	class ENGINE_API IPhysicsHingeJoint : public IPhysicsJoint
	{
	public:
		virtual void			SetLimits(float minAngle, float maxAngle, float bounciness = 1.0f) = 0;
	};

// 	class ENGINE_API IRagDoll 
// 	{
// 		virtual bool			IsEnabled() const = 0;
// 
// 		virtual void			SetEnabled(bool enabled) = 0;
// 
// 		virtual void			Init(ISkeleton* pSkeleton) = 0;
// 	};
}


#endif
