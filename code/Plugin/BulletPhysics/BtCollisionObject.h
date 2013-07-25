#ifndef __Bullet_CollisionObject_H__
#define __Bullet_CollisionObject_H__



class btRigidBody;

namespace ma
{
	class ENGINE_API CollisionObject
	{	
	public:
		virtual void			SetTransformWS(const NodeTransform& tsfWS);

		virtual NodeTransform	GetTransformWS();
	};

	class ENGINE_API RigidBody : public CollisionObject
	{
	public:
		virtual void			SetUseGravity(bool bUseGravity);

		virtual bool			IsUseGravity();

		virtual bool			IsKinematic();

		virtual	void			SetKinematic(bool bKinematic);

		virtual void			SetMass(float fMass);

		virtual float			GetMass();

		virtual void			SetLinearDamping(float fLinearDamping);

		virtual float			GetLinearDamping();

		virtual void			SetAngularDamping(float fAngularDamping);

		virtual float			GetAngularDamping();

		virtual	void			ApplyForce(Vector3 vForce);

		virtual	IGenericJoint*	CreatePhysicsGenericJoint(IRigidBody* pPhyObjB);

		virtual	IHingeJoint*	CreatePhysicsHingeJoint(IRigidBody* pPhyObjB);
	};


	class ENGINE_API CharaControll : public CollisionObject
	{
	public:
		virtual void			SetStepOffset(float fStepOffset);

		virtual float			GetStepOffset();

		virtual bool			IsGrounded() const;
	};
}

#endif // __Bullet_CollisionObject_H__

