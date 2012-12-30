#ifndef  _BtPhysicsObject__H__
#define  _BtPhysicsObject__H__

namespace ma
{

	class BULLETPHYSICS_API BtPhysicsObject : public IPhysicsObject
	{
	public:
		virtual void Start();

		virtual void Stop();

		virtual bool IsKinematic();

		//virtual bool Is

		virtual void SetRigidBody(const RigidBodyInfo& info);

		virtual void AddBoxCollision(const BoxCollisionInfo& info);

		virtual void SetTransformWS(const maNodeTransform& tsfWS);

		virtual maNodeTransform GetTransformWS() const;

	private:
		btRigidBody* m_pRigidBody;

		RigidBodyInfo m_rbInfo;
		std::vector<BoxCollisionInfo> m_vBoxCollInfo;
	};
}


#endif
