#include "Engine.h"
#include <thread>

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
		
		g_pObjectFactoryManager = new ObjectFactoryManager();	
		g_pAttributeManager = new AttributeManager();

		g_pArchiveManager = new ArchiveManager();
		g_pMeshManager = new ResourceSystem<MeshData>();
		g_pMaterialManager = new ResourceSystem<Material>();
		g_pXmlFileManager = new ResourceSystem<XmlFile>();
		g_pTextureManager = new TextureManager();
		g_pShaderManager = new ShaderManager();

		g_pParameterManager = new ParameterManager();
		g_pDeviceCapabilities = new DeviceCapabilitie();
		g_pRenderSystem = new RenderSystem();

		g_pJobScheduler = new JobScheduler();
	}

	Engine::~Engine()
	{
		SAFE_DELETE(g_pJobScheduler);

		SAFE_DELETE(g_pRenderSystem);
		SAFE_DELETE(g_pDeviceCapabilities);
		SAFE_DELETE(g_pParameterManager);

		SAFE_DELETE(g_pMeshManager); 
		SAFE_DELETE(g_pMaterialManager);
		SAFE_DELETE(g_pXmlFileManager);
		SAFE_DELETE(g_pTextureManager);
		SAFE_DELETE(g_pShaderManager); 
		SAFE_DELETE(g_pArchiveManager);

		SAFE_DELETE(g_pAttributeManager);
		SAFE_DELETE(g_pObjectFactoryManager);

		SAFE_DELETE(g_pTimer);
	}

	void Engine::Init(void* hWnd, bool bRenderThread, bool bDataThread, bool bJobScheduler)
	{
		CImageCodec::Startup();

		EngineRTTIInit();

		g_pRenderSystem->Init(hWnd,bRenderThread);

		if (bDataThread)
		{
			g_pDataThread = new DataThread();
			g_pDataThread->Start();
		}

		if (bJobScheduler)
		{
			GetJobScheduler()->CreateThreads(std::thread::hardware_concurrency() - 1,16);
		}
	}

	void Engine::Reset(uint32_t nWidth, uint32_t nHeight)
	{
		g_pRenderSystem->Reset(nWidth,nHeight);
	}
	
	void Engine::Shutdown()
	{
		if (g_pDataThread)
		{
			g_pDataThread->Stop();
			SAFE_DELETE(g_pDataThread);
		}

		g_pMeshManager->Clear();
		g_pMaterialManager->Clear();
		g_pXmlFileManager->Clear();
		g_pTextureManager->Clear();
		g_pShaderManager->Clear();

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
