#ifndef _Sample_CharaControl_H_
#define _Sample_CharaControl_H_

namespace ma
{
	class Scene;

	class SampleCharaControl : public Sample
	{
	public:
		SampleCharaControl();

		virtual void Init();

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Update();

		virtual void Render();

	private:
		Scene*	m_pScene;	

		 //m_pRigidBodyComp;
	};
}


#endif

