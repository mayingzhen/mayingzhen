#ifndef  _Engine_Engine__H__
#define  _Engine_Engine__H__

namespace ma
{
	class Engine
	{
	public:
		Engine();

		~Engine();

		void Init(HWND hWnd, bool bRenderThread, bool bDataThread, bool bWorkQueue);

		void Shutdown();

		void Update();

		void Render();

	private:
		
	};

	extern Engine* g_pEngine;
	void SetEngine(Engine* pEngine);
	Engine* GetEngine();
}


#endif
