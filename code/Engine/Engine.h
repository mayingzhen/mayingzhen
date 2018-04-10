#pragma once

namespace ma
{
	class Engine
	{
	public:
		Engine();

		~Engine();

		void Init(HWND hWnd, bool bRenderThread, bool bDataThread, bool bJobScheduler);
		
		void Reset(uint32 nWidth, uint32 nHeight);

		void Shutdown();

		void Update();

		void Render();	
	};

	extern Engine* g_pEngine;
	void SetEngine(Engine* pEngine);
	Engine* GetEngine();
}
