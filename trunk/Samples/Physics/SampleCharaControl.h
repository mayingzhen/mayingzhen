#ifndef _Sample_CharaControl_H_
#define _Sample_CharaControl_H_

namespace ma
{
	class Scene;

	class SampleCharaControl : public SimpleSceneView
	{
	public:
		SampleCharaControl();

		virtual void Init(Application* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

		virtual	void OnResize(int w,int h);

	private:
		Scene*	m_pScene;	

		Charac* m_pRigidBodyComp;
	};
}


#endif

