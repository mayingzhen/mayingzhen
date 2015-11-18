#include "Engine.h"

namespace ma
{
	Engine* g_pEngine = NULL;
	void SetEngine(Engine* pEngine)
	{
		g_pEngine = pEngine;
	}
	Engine* GetEngine()
	{
		return g_pEngine;
	}

	Engine::Engine()
	{
		g_pTimer = new Time();
		g_pCodeTimerManager = new CodeTimerManager();
		
		g_pObjectFactoryManager = new ObjectFactoryManager();	
		g_pAttributeManager = new AttributeManager();

		g_pArchiveManager = new ArchiveManager();
		g_pMeshManager = new ResourceSystem<MeshData>();
		g_pXmlFileManager = new ResourceSystem<XmlFile>();
		g_pTextureManager = new TextureManager();
		g_pShaderManager = new ShaderManager();

		g_pParameterManager = new ParameterManager();
		g_pDeviceCapabilities = new DeviceCapabilitie();
		g_pRenderSystem = new RenderSystem();

		g_pWorkQueue = new WorkQueue();
	}

	Engine::~Engine()
	{
		SAFE_DELETE(g_pWorkQueue);

		SAFE_DELETE(g_pRenderSystem);
		SAFE_DELETE(g_pDeviceCapabilities);
		SAFE_DELETE(g_pParameterManager);

		SAFE_DELETE(g_pMeshManager); 
		SAFE_DELETE(g_pXmlFileManager);
		SAFE_DELETE(g_pTextureManager);
		SAFE_DELETE(g_pShaderManager); 
		SAFE_DELETE(g_pArchiveManager);

		SAFE_DELETE(g_pAttributeManager);
		SAFE_DELETE(g_pObjectFactoryManager);

		SAFE_DELETE(g_pCodeTimerManager);
		SAFE_DELETE(g_pTimer);
	}

	void Engine::Init(HWND hWnd, bool bRenderThread, bool bDataThread, bool bWorkQueue)
	{
		CImageCodec::Startup();

		EngineRTTIInit();

		g_pRenderSystem->Init(hWnd,bRenderThread);

		if (bDataThread)
		{
			g_pDataThread = new DataThread();
			g_pDataThread->Start();
		}

		if (bWorkQueue)
		{
			GetWorkQueue()->CreateThreads(3);
		}
	}
	

	void Engine::Shutdown()
	{
		if (GetParticleSystem())
			GetParticleSystem()->Shutdown();

		if (g_pDataThread)
		{
			g_pDataThread->Stop();
			SAFE_DELETE(g_pDataThread);
		}

		g_pRenderSystem->Shoutdown();

		EngineRTTIShutdown();

		CImageCodec::Shutdown();
	}

	void Engine::Update()
	{
		if (GetTimer())
			GetTimer()->UpdateFrame();

		if (g_pDataThread)
			g_pDataThread->Process();

		g_pRenderSystem->Update();
	}

	void Engine::Render()
	{
		g_pRenderSystem->Render();
	}

}