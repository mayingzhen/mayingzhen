#ifndef  _IPhysicsObject__H__
#define  _IPhysicsObject__H__

namespace ma
{
	class IRigidBody;
	class IBoxCollisionShape;
	class ISphereCollisionShape;

	class ENGINE_API IPhysicsObject
	{
	public:

		virtual	IRigidBody*				CreateRigidBody() = 0;

		virtual IBoxCollisionShape*		CreateBoxCollisionShape() = 0;

		virtual ISphereCollisionShape*	CreateSphereCollisionShape() = 0;

		virtual void					SetTransformWS(const NodeTransform& tsfWS) = 0;

		virtual NodeTransform			GetTransformWS() = 0;

		virtual	void					ApplyForce(Vector3 vForce) = 0;
	};
}


#endif
