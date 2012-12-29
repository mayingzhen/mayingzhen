#ifndef  _BtPhysicsObject__H__
#define  _BtPhysicsObject__H__

namespace ma
{

	class BULLETPHYSICS_API BtPhysicsObject : public IPhysicsObject
	{
	public:
		virtual void Start();

		virtual void SetRigidBody(const RigidBodyInfo& info);

		virtual void AddBoxCollision(const BoxCollisionInfo& info);

		virtual void SetTransformWS(const maNodeTransform& tsfWS);

		virtual maNodeTransform GetTransformWS() const;

	private:
		btRigidBody* m_pRigidBody;
	};
}


#endif
