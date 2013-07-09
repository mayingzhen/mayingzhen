#ifndef _Sample_RigidBody_H_
#define _Sample_RigidBody_H_

namespace ma
{
	class RigidBodyComponent;
	class Scene;

	class SampleRigidBody : public Sample
	{
	public:
		SampleRigidBody();

		virtual void Init(const Platform* pPlatform);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Update();

		virtual void Render();

	private:
		RigidBodyComponent* m_pRigidBodyComp;

		Scene*	m_pScene;
	};
}


#endif

