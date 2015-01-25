#ifndef  _Engine_Engine__H__
#define  _Engine_Engine__H__

namespace ma
{
	class Engine
	{
	public:
		Engine();

		~Engine();

		void Init(HWND hWnd, bool bRenderThread, bool bDataThread, bool bParticleThread);

		void Shutdown();

		void Update();

		void Render();

	private:
		
	};

	void SetEngine(Engine* pEngine);
	Engine* GetEngine();
}


#endif
