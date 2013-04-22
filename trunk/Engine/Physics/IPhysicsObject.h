#ifndef  _IPhysicsObject__H__
#define  _IPhysicsObject__H__

namespace ma
{

	class ENGINE_API IPhysicsObject
	{
	public:
		virtual bool			Start(GameObject* pOwner) = 0;

		virtual	bool			Stop(GameObject* pOwner) = 0;

		virtual IRigidBody*		GetRigidBody() = 0;

		virtual void			SetRigidBody(IRigidBody* pRigidBody) = 0;

		virtual void			AddBoxCollisionShape(IBoxCollisionShape* pCollisionShape) = 0;

		virtual	void			AddSphereCollisionShape(ISphereCollisionShape* pSphereCollisionShape)  = 0;

		virtual void			SetTransformWS(const NodeTransform& tsfWS) = 0;

		virtual NodeTransform GetTransformWS() = 0;

		virtual	void			ApplyForce(Vector3 vForce) = 0;
	};
}


#endif
