#ifndef _Sample_RigidBody_H_
#define _Sample_RigidBody_H_

namespace ma
{
	class RigidBodyComponent;

	class SampleRigidBody : public SimpleSceneView
	{
	public:
		SampleRigidBody();

		virtual void Init(Application* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

	private:
		RigidBodyComponent* m_pRigidBodyComp;
	};
}


#endif

