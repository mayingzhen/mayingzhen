#pragma once

namespace ma
{
	class Engine
	{
	public:
		Engine();

		~Engine();

		void Init(void* hWnd, int width, int height, bool bRenderThread, bool bDataThread, bool bJobScheduler);
		
		void Reset(uint32_t nWidth, uint32_t nHeight);

		void Shutdown();

		void Update();

		void Render();	
	};

	extern Engine* g_pEngine;
	void SetEngine(Engine* pEngine);
	Engine* GetEngine();
}
