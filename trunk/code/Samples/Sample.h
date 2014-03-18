#ifndef _SimpleSceneView_H_
#define _SimpleSceneView_H_

namespace ma
{
	class CameraController;
	class Camera;

	class Sample : public OIS::KeyListener 
	{
	public:
		virtual void		Load() = 0;

		virtual void		UnLoad() = 0;

		virtual void		Update() {}

		virtual void		Render()  {}

		CameraController*	GetCameraControll();

		Camera*				GetCamera();

		virtual bool		keyPressed(const OIS::KeyEvent &arg) {return true;}

		virtual bool		keyReleased(const OIS::KeyEvent &arg) {return true;}	

	};
}

#endif
