#ifndef _Sample_H_
#define _Sample_H_


namespace ma
{
	class CameraController;
	class Camera;

	class Sample : 
		public OIS::KeyListener , 
		public OIS::MouseListener , 
		public OIS::MultiTouchListener
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

		virtual bool		keyPressed(const OIS::KeyEvent &arg) {return true;}
		virtual bool		keyReleased(const OIS::KeyEvent &arg) {return true;}	
		virtual bool		mouseMoved( const OIS::MouseEvent &arg ) {return true;}
		virtual bool		mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {return true;}
		virtual bool		mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {return true;}
		virtual bool		touchMoved( const OIS::MultiTouchEvent &arg ) {return true;}
		virtual bool		touchPressed( const OIS::MultiTouchEvent &arg ) {return true;}
		virtual bool		touchReleased( const OIS::MultiTouchEvent &arg ) {return true;}
		virtual bool		touchCancelled( const OIS::MultiTouchEvent &arg ) {return true;}

	protected:
		RefPtr<Scene>		m_pScene;
		RefPtr<Camera>		m_pCamera;
	
	};
}

#endif

