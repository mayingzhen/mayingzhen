#ifndef _SimpleSceneView_H_
#define _SimpleSceneView_H_

namespace ma
{
	class CameraController;
	class Camera;

	class Sample
	{
	public:
		virtual void		Load() = 0;

		virtual void		UnLoad() = 0;

		virtual void		Update() = 0;

		virtual void		Render() = 0;

		CameraController*	GetCameraControll();

		Camera*				GetCamera();

	};
}

#endif
