#include "Engine.h"

namespace ma
{
	static Engine* g_pEngine = NULL;
	
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
		CImageCodec::Startup();

		ClassManager* pClsMan = new ClassManager();
		SetClassManager(pClsMan);

		ObjectFactoryManager* pObjMan = new ObjectFactoryManager();
		SetObjectFactoryManager(pObjMan);

		AttributeManager* pContexMan = new AttributeManager();
		SetAttributeManager(pContexMan);

		ResourceSystem* pRsourceSystem = new ResourceSystem();
		SetResourceSystem(pRsourceSystem);

		CodeTimerManager* pCodeTimeMng = new CodeTimerManager();
		SetCodeTimeMng(pCodeTimeMng);

		ArchiveManager* pArchiveMang = new ArchiveManager();
		SetArchiveManager(pArchiveMang);

		Time* pTime = new Time();
		SetTimer(pTime);

		ParameterManager* pMaterialMang = new ParameterManager();
		SetParameterManager(pMaterialMang);

		DeviceCapabilitie* pDevCap = new DeviceCapabilitie();
		SetDeviceCapabilities(pDevCap);

		RenderSystem* pRenderSystem = new RenderSystem();
		SetRenderSystem(pRenderSystem);

		ParticleSystem* pParticleMang = new ParticleSystem();
		SetParticleSystem(pParticleMang);

		LightSystem* pLightSystem = new LightSystem();
		SetLightSystem(pLightSystem);

		WorkQueue* pWorkQueue = new WorkQueue();
		SetWorkQueue(pWorkQueue);

		EngineRTTIInit();
	}

	Engine::~Engine()
	{
		EngineRTTIShutdown();


		Time* pTime = GetTimer();
		SAFE_DELETE(pTime);
		SetTimer(NULL);

		WorkQueue* pWorkQueue = GetWorkQueue();
		SAFE_DELETE(pWorkQueue);
		SetWorkQueue(NULL);

		ArchiveManager* pArchiveManager = GetArchiveMananger();
		SAFE_DELETE(pArchiveManager);
		SetArchiveManager(NULL);

		CodeTimerManager* pCodeTimeMng = GetCodetTimeMng();
		SAFE_DELETE(pCodeTimeMng);
		SetCodeTimeMng(NULL);

		LightSystem* pLightSystem = GetLightSystem();
		SAFE_DELETE(pLightSystem);
		SetLightSystem(NULL);

		ParticleSystem* pParticleMang = GetParticleSystem();
		SAFE_DELETE(pParticleMang)
		SetParticleSystem(NULL);

		RenderSystem* pRenderSystem = GetRenderSystem();
		SAFE_DELETE(pRenderSystem);
		SetRenderSystem(NULL);

		DeviceCapabilitie* pDevCap = GetDeviceCapabilities();
		SAFE_DELETE(pDevCap);
		SetDeviceCapabilities(NULL);

		ParameterManager* pMaterialMang = GetParameterManager();
		SAFE_DELETE(pMaterialMang);
		SetParameterManager(NULL);

		ResourceSystem* pRsourceSystem = GetResourceSystem();
		SAFE_DELETE(pRsourceSystem);
		SetResourceSystem(NULL);

		AttributeManager* pContexMan = GetAttributeManager();
		SAFE_DELETE(pContexMan);
		SetAttributeManager(NULL);

		ObjectFactoryManager* pObjMan = GetObjectFactoryManager();
		SAFE_DELETE(pObjMan);
		SetObjectFactoryManager(NULL);

		ClassManager* pClsMan = GetClassManager();
		SAFE_DELETE(pClsMan);
		SetClassManager(NULL);

		CImageCodec::Shutdown();
	}

	void Engine::Init(HWND hWnd, bool bRenderThread, bool bDataThread, bool bParticleThread)
	{
		if (GetRenderSystem())
			GetRenderSystem()->Init(hWnd, bRenderThread);

		if (GetResourceSystem())
			GetResourceSystem()->Init(bDataThread);

		if (GetParticleSystem())
			GetParticleSystem()->Init(bParticleThread);

		//if (GetWorkQueue())
		//	GetWorkQueue()->CreateThreads(3);
	}
	

	void Engine::Shutdown()
	{
		if (GetParticleSystem())
			GetParticleSystem()->Shutdown();

		if (GetResourceSystem())
			GetResourceSystem()->Shoutdown();

		if (GetRenderSystem())
			GetRenderSystem()->Shoutdown();
	}

	void Engine::Update()
	{
		if (GetTimer())
			GetTimer()->UpdateFrame();

		if (GetResourceSystem())
			GetResourceSystem()->Update();

		if (GetRenderSystem())
			GetRenderSystem()->Update();
	}

	void Engine::Render()
	{
		if (GetRenderSystem())
			GetRenderSystem()->Render();
	}

}