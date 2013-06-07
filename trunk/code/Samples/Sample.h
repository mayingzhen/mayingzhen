#ifndef _SimpleSceneView_H_
#define _SimpleSceneView_H_

namespace ma
{
	class Camera;
	class Platform;

	class Sample
	{
	public:
		Sample();

		virtual void Init(const Platform* pPlatform);

		virtual void ShoutDown();

		virtual void Update();

		virtual void Render();

		///// Input
		virtual void keyEvent(Keyboard::KeyEvent evt, int key);

		virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

		virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);


	protected:
		//Scene*				m_pScene;

		Camera*				m_pCamera;

		float				m_fMoveCameraSpeed;
		float				m_fZoomSpeed;

		ShaderProgram*				m_pDefaultEffect;

	};
}

#endif
