#ifndef _SimpleSceneView_H_
#define _SimpleSceneView_H_

namespace ma
{
	class Camera;
	class Application;

	class SimpleSceneView : public SceneView
	{
	public:
		SimpleSceneView();

		virtual void Init(Application* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

		virtual	void OnResize(int w,int h);

	protected:
		void UpdateCamra(float timeElapsed);

	protected:
		Scene*			m_pScene;

		Camera*			m_pCamera;
	
		Application*	m_pApplication;

		float			m_fMoveCameraSpeed;
	};
}

#endif
