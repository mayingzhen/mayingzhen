#ifndef _Sample_RigidBody_H_
#define _Sample_RigidBody_H_

namespace ma
{
	class Scene;

	class SampleRigidBody : public Sample, public CollisionListener
	{
	public:
		SampleRigidBody();

		virtual void Load();

		virtual void UnLoad();

		virtual void Update();

		virtual void collisionEvent(const CollisionData& eventData);

	private:
		IRigidBody* m_pRigidBody;
	};
}


#endif

