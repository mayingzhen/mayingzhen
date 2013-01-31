#ifndef __SceneContext_h__
#define __SceneContext_h__

namespace ma
{	
	class Application;

	class Application_API SceneView
	{
	public:	
		virtual void Init(Application* pApplication) = 0;

		virtual void Shutdown() = 0;

		virtual void Load() = 0;

		virtual void Unload() = 0;

		virtual void Tick(float timeElapsed) = 0;

		virtual void Render() = 0;

		virtual	void OnResize(int w,int h) = 0;
	};
}


#endif
