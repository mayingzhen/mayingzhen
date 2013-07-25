#ifndef _Sample_RigidBody_H_
#define _Sample_RigidBody_H_

namespace ma
{
	class RigidBodyComponent;
	class Scene;

	class SampleRigidBody : public Sample, public CollisionListener
	{
	public:
		SampleRigidBody();

		virtual void Init();

		virtual void Load();

		virtual void Update();

		virtual void Render();

		virtual void collisionEvent(CollisionListener::EventType type,
			const CollisionPair& collisionPair,
			const Vector3& contactPointA,
			const Vector3& contactPointB);

	private:
		IRigidBody* m_pRigidBody;

		Scene*	m_pScene;
	};
}


#endif

