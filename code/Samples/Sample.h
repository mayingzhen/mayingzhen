#pragma once


namespace ma
{
	class CameraController;
	class Camera;

	class Sample : public Referenced
	{
	public:
		Sample(const char* pszName = NULL);

		~Sample();

		virtual void		Load() = 0;

		virtual void		UnLoad() = 0;

		virtual void		Update() {}

		virtual void		Render()  {}

		CameraController*	GetCameraControll();

		Camera*				GetCamera();

		Scene*				GetScene() {return m_pScene.get();}

		virtual void		keyEvent(Keyboard::KeyEvent evt, Keyboard::Key key){}

	protected:
		void				CreateMeshMaterial(const char* pszTexture,const char* pMatPath,const char* pszAddMacro = NULL);

	protected:
		RefPtr<Scene>		m_pScene;
		RefPtr<Camera>		m_pCamera;
	
	};
}


