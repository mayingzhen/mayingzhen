#ifndef __IPhysicsRigidBody_H__
#define __IPhysicsRigidBody_H__


namespace ma
{
	class ENGINE_API IRigidBody : public Component
	{
		DECL_OBJECT(IRigidBody)

	public:

		virtual void			SetUseGravity(bool bUseGravity) = 0;

		virtual bool			IsUseGravity() = 0;

		virtual bool			IsKinematic() = 0;

		virtual	void			SetKinematic(bool bKinematic) = 0;

		virtual void			SetMass(float fMass) = 0;

		virtual float			GetMass() = 0;

		virtual void			SetLinearDamping(float fLinearDamping) = 0;

		virtual float			GetLinearDamping() = 0;

		virtual void			SetAngularDamping(float fAngularDamping) = 0;

		virtual float			GetAngularDamping() = 0;

		virtual	void			SetDeactivationTime(float fTime) = 0;

		virtual	void			SetSleepingThresholds(float fLinear,float fAngular) = 0;

		virtual	void			ApplyForce(Vector3 vForce) = 0;
	};

	DeclareRefPtr(IRigidBody);
}

#endif
