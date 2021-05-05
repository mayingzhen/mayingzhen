#include "Engine.h"
#include <thread>
//#include "Scene/SoftwareRasterizer/Rasterizer.h"

#include "FrameGraph/FrameGraph.h"

namespace ma
{
	//Rasterizer* g_rasterizer = nullptr;

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

	void Engine::Init(void* hWnd, int width, int height, bool bRenderThread, bool bDataThread, bool bJobScheduler)
	{
		ProfileInit();

		CImageCodec::Startup();

		EngineRTTIInit();

		g_pRenderSystem->Init(hWnd, width, height, bRenderThread);

		if (bDataThread)
		{
			g_pDataThread = new DataThread();
			g_pDataThread->Start();
		}

		if (bJobScheduler)
		{
			GetJobScheduler()->CreateThreads(std::thread::hardware_concurrency() - 1,16);
		}

		//g_rasterizer = new Rasterizer(width, height);

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
		ProfileUpdateFrame();

		//SYSTRACE(Engine_Update);
		ROFILE_SCOPEI("Engine::Update", 0);

		if (GetTimer())
			GetTimer()->UpdateFrame();

		if (g_pDataThread)
			g_pDataThread->Process();

		g_pRenderSystem->Update();
	}


// 	FrameGraphId<FrameGraphTexture> colorPass(FrameGraph& fg, const char* name)  
// 	{
// 
// 		struct ColorPassData {
// 			FrameGraphId<FrameGraphTexture> shadows;
// 			FrameGraphId<FrameGraphTexture> color;
// 			FrameGraphId<FrameGraphTexture> output;
// 			FrameGraphId<FrameGraphTexture> depth;
// 			FrameGraphId<FrameGraphTexture> ssao;
// 			FrameGraphId<FrameGraphTexture> ssr;
// 			FrameGraphId<FrameGraphTexture> structure;
// 			FrameGraphRenderTargetHandle rt{};
// 			ColourValue clearColor{};
// 		};
// 
// 		auto& colorPass = fg.addPass<ColorPassData>(name,
// 			[&](FrameGraph::Builder& builder, ColorPassData& data) {
// 
// 				Blackboard& blackboard = fg.getBlackboard();
// 				TargetBufferFlags clearDepthFlags = TargetBufferFlags::NONE;
// 				TargetBufferFlags clearColorFlags = config.clearFlags & TargetBufferFlags::COLOR;
// 				data.clearColor = config.clearColor;
// 
// 				data.shadows = blackboard.get<FrameGraphTexture>("shadows");
// 				data.ssr = blackboard.get<FrameGraphTexture>("ssr");
// 				data.ssao = blackboard.get<FrameGraphTexture>("ssao");
// 				data.color = blackboard.get<FrameGraphTexture>("color");
// 				data.depth = blackboard.get<FrameGraphTexture>("depth");
// 				data.structure = blackboard.get<FrameGraphTexture>("structure");
// 
// 				if (config.hasContactShadows) {
// 					assert(data.structure.isValid());
// 					data.structure = builder.sample(data.structure);
// 				}
// 
// 				if (data.shadows.isValid()) {
// 					data.shadows = builder.sample(data.shadows);
// 				}
// 
// 				if (data.ssr.isValid()) {
// 					data.ssr = builder.sample(data.ssr);
// 				}
// 
// 				if (data.ssao.isValid()) {
// 					data.ssao = builder.sample(data.ssao);
// 				}
// 
// 				if (!data.color.isValid()) {
// 					// we're allocating a new buffer, so its content is undefined and we might need
// 					// to clear it.
// 
// 					if (view.getBlendMode() == View::BlendMode::TRANSLUCENT) {
// 						// if the View is going to be blended in, then always clear to transparent
// 						clearColorFlags |= TargetBufferFlags::COLOR;
// 						data.clearColor = {};
// 					}
// 
// 					if (view.isSkyboxVisible()) {
// 						// if the skybox is visible, then we don't need to clear at all
// 						clearColorFlags &= ~TargetBufferFlags::COLOR;
// 					}
// 
// 					data.color = builder.createTexture("Color Buffer", colorBufferDesc);
// 				}
// 
// 				if (!data.depth.isValid()) {
// 					// clear newly allocated depth buffers, regardless of given clear flags
// 					clearDepthFlags = TargetBufferFlags::DEPTH;
// 					data.depth = builder.createTexture("Depth Buffer", {
// 							.width = colorBufferDesc.width,
// 							.height = colorBufferDesc.height,
// 							// If the color attachment requested MS, we assume this means the MS buffer
// 							// must be kept, and for that reason we allocate the depth buffer with MS
// 							// as well. On the other hand, if the color attachment was allocated without
// 							// MS, no need to allocate the depth buffer with MS, if the RT is MS,
// 							// the tile depth buffer will be MS, but it'll be resolved to single
// 							// sample automatically -- which is what we want.
// 							.samples = colorBufferDesc.samples,
// 							.format = TextureFormat::DEPTH32F,
// 						});
// 				}
// 
// 				if (colorGradingConfig.asSubpass) {
// 					data.output = builder.createTexture("Tonemapped Buffer", {
// 							.width = colorBufferDesc.width,
// 							.height = colorBufferDesc.height,
// 							.format = colorGradingConfig.ldrFormat
// 						});
// 					data.output = builder.write(data.output);
// 					data.color = builder.read(data.color);
// 				}
// 
// 				data.color = builder.write(builder.read(data.color));
// 				data.depth = builder.write(builder.read(data.depth));
// 
// 				blackboard["depth"] = data.depth;
// 
// 				data.rt = builder.createRenderTarget("Color Pass Target", {
// 						.attachments = {{ data.color, data.output, {}, {}}, data.depth, {}},
// 						.samples = config.msaa,
// 						.clearFlags = clearColorFlags | clearDepthFlags });
// 			},
// 			[=, &view](FrameGraphPassResources const& resources,
// 				ColorPassData const& data, DriverApi& driver) {
// 					auto out = resources.get(data.rt);
// 
// 					// set samplers and uniforms
// 					PostProcessManager& ppm = getEngine().getPostProcessManager();
// 					view.prepareSSAO(data.ssao.isValid() ?
// 						resources.getTexture(data.ssao) : ppm.getOneTexture());
// 
// 					// set shadow sampler
// 					view.prepareShadow(data.shadows.isValid() ?
// 						resources.getTexture(data.shadows) : ppm.getOneTextureArray());
// 
// 					assert(data.structure.isValid());
// 					if (data.structure.isValid()) {
// 						const auto& structure = resources.getTexture(data.structure);
// 						view.prepareStructure(structure ? structure : ppm.getOneTexture());
// 					}
// 
// 					// TODO: check what getTexture() returns
// 					if (data.ssr.isValid()) {
// 						view.prepareSSR(resources.getTexture(data.ssr), config.refractionLodOffset);
// 					}
// 
// 					view.prepareViewport(static_cast<filament::Viewport&>(out.params.viewport));
// 					view.commitUniforms(driver);
// 
// 					out.params.clearColor = data.clearColor;
// 
// 					if (colorGradingConfig.asSubpass) {
// 						out.params.subpassMask = 1;
// 						driver.beginRenderPass(out.target, out.params);
// 						pass.executeCommands(resources.getPassName());
// 						ppm.colorGradingSubpass(driver, colorGradingConfig.translucent);
// 					}
// 					else {
// 						driver.beginRenderPass(out.target, out.params);
// 						pass.executeCommands(resources.getPassName());
// 					}
// 
// 					driver.endRenderPass();
// 
// 					// color pass is typically heavy and we don't have much CPU work left after
// 					// this point, so flushing now allows us to start the GPU earlier and reduce
// 					// latency, without creating bubbles.
// 					driver.flush();
// 			}
// 			);
// 
// 		// when color grading is done as a subpass, the output of the color-pass is the ldr buffer
// 		auto output = colorGradingConfig.asSubpass ?
// 			colorPass.getData().output : colorPass.getData().color;
// 
// 		fg.getBlackboard()["color"] = output;
// 		return output;
// 	}


	void Engine::Render()
	{
		//SYSTRACE(Engine_Render);
		ROFILE_SCOPEI("Engine::Render", 0);




		g_pRenderSystem->Render();
	}

}
