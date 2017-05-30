#include "VulkanRenderDevice.h"
#include "VulkanTexture.h"
#include "VulkanConstantBuffer.h"
#include "VulkanRenderState.h"
#include "VulkanSamplerState.h"

namespace ma
{

	IVulkanDevice* GetVulkanDxDevive()
	{
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();
		ASSERT(pRender);
		return pRender->GetDXDevive();
	}

	IVulkanDeviceContext* GetVulkanDxDeviveContext()
	{
		VulkanRenderDevice* pRender = (VulkanRenderDevice*)GetRenderDevice();
		ASSERT(pRender);
		return pRender->GetDXDeviveContext();
	}

	VulkanRenderDevice::VulkanRenderDevice()
	{
		m_pD3DDevice = NULL;
		m_pDeviceContext = NULL;
		m_pSwapChain = NULL;

		ClearAllStates();

		m_bRenderTargetsDirty = true;

		m_nFirstDirtyVB = 0;
		m_nLastDirtyVB = 0;
		memset(m_arrVertexBuffers,0,sizeof(m_arrVertexBuffers));
		memset(m_arrVertexSize,0,sizeof(m_arrVertexSize));
		memset(m_arrVertexOffset,0,sizeof(m_arrVertexOffset));

	 	memset(m_arrTexture,0,sizeof(m_arrTexture));
		memset(m_arrShaderResourceView,0,sizeof(m_arrShaderResourceView));
		m_nFirstDirtyTexture = M_MAX_UNSIGNED;
		m_nLastDirtyTexture = M_MAX_UNSIGNED;
		m_bTexturesDirty = true;

		memset(m_arrVulkanSampler,0,sizeof(m_arrVulkanSampler));
		m_nFirstDirtySamplerState = M_MAX_UNSIGNED;
		m_nLastDirtySamplerState = M_MAX_UNSIGNED;
		m_bSamplerStatesDirty = true;

		m_pDepthStencil = NULL;
		memset(m_pRenderTarget,0,sizeof(m_pRenderTarget));

		m_pCurInput = NULL;
		m_pCurDSState = NULL;
		m_nStencilRef = 0;
		m_pCurBlendState = NULL;
		m_pCurRSState = NULL;
	}

	VulkanRenderDevice::~VulkanRenderDevice()
	{
	}

	void VulkanRenderDevice::ClearAllStates()
	{
	}

	Texture* VulkanRenderDevice::CreateTexture()
	{
		return new VulkanTexture();
	}

	Texture* VulkanRenderDevice::CreateRenderTarget(int nWidth,int nHeight,UINT32 nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType)
	{
		return new VulkanTexture(nWidth,nHeight,nMipMap,format,false,bSRGB,USAGE_RENDERTARGET,eType);
	}

	Texture* VulkanRenderDevice::CreateDepthStencil(int nWidth,int nHeight,PixelFormat format,bool bTypeLess)
	{
		return new VulkanTexture(nWidth,nHeight,1,format,bTypeLess,false,USAGE_DEPTHSTENCIL,TEXTYPE_2D);
	}

	VertexDeclaration* VulkanRenderDevice::CreateVertexDeclaration()
	{
		return new VulkanVertexDeclaration();
	}

	VertexBuffer*	VulkanRenderDevice::CreateVertexBuffer()
	{
		return new VulkanVertexBuffer();
	}

	IndexBuffer*	VulkanRenderDevice::CreateIndexBuffer()
	{
		return new VulkanIndexBuffer();
	}

	ShaderProgram*	VulkanRenderDevice::CreateShaderProgram()
	{
		return new VulkanShaderProgram();
	}

	BlendState*	VulkanRenderDevice::CreateBlendState()
	{
		return new VulkanBlendStateObject();
	}

	DepthStencilState*	VulkanRenderDevice::CreateDepthStencilState()
	{
		return new VulkanDepthStencilStateObject();
	}

	RasterizerState* VulkanRenderDevice::CreateRasterizerState()
	{
		return new VulkanRasterizerStateObject();
	}

	SamplerState* VulkanRenderDevice::CreateSamplerState()
	{
		return new VulkanSamplerStateObject();
	}

	void VulkanRenderDevice::Shoutdown()
	{
		m_hWnd = NULL;

		VulkanRasterizerStateObject::Clear();
		VulkanDepthStencilStateObject::Clear();
		VulkanBlendStateObject::Clear();
		VulkanVertexDeclaration::Clear();
		VulkanSamplerStateObject::Clear();
		ConstantBuffer::Clear();

		SAFE_RELEASE(m_pDeviceContext);
		SAFE_RELEASE(m_pSwapChain);

#if defined(DEBUG) || defined(_DEBUG)
		IVulkanDebug *d3dDebug;
		HRESULT hr = m_pD3DDevice->QueryInterface(__uuidof(IVulkanDebug), reinterpret_cast<void**>(&d3dDebug));
		if (SUCCEEDED(hr))
		{
			hr = d3dDebug->ReportLiveDeviceObjects(Vulkan_RLDO_DETAIL);
		}
		if (d3dDebug != NULL)			
			d3dDebug->Release();
#endif

		SAFE_RELEASE(m_pD3DDevice);
	}

	void VulkanRenderDevice::Init(HWND wndhandle)
	{
		m_hWnd = wndhandle;

		//this->settings.validation = enableValidation;

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		//appInfo.pApplicationName = Game::GetInstance()->GetGameName();
		//appInfo.pEngineName = Game::GetInstance()->GetGameName();
		appInfo.apiVersion = VK_API_VERSION_1_0;

		std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

		// Enable surface extensions depending on os
#if defined(_WIN32)
		instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(__ANDROID__)
		instanceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(_DIRECT2DISPLAY)
		instanceExtensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
		instanceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined(__linux__)
		instanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = NULL;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		if (instanceExtensions.size() > 0)
		{
// 			if (settings.validation)
// 			{
// 				instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
// 			}
			instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
			instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
		}
// 		if (settings.validation)
// 		{
// 			instanceCreateInfo.enabledLayerCount = vks::debug::validationLayerCount;
// 			instanceCreateInfo.ppEnabledLayerNames = vks::debug::validationLayerNames;
// 		}
		vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

		VkResult err;

		// Physical device
		uint32_t gpuCount = 0;
		// Get number of available physical devices
		VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr));
		assert(gpuCount > 0);
		// Enumerate devices
		std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
		err = vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data());
		if (err)
		{
			//vks::tools::exitFatal("Could not enumerate physical devices : \n" + vks::tools::errorString(err), "Fatal error");
		}

		uint32_t selectedDevice = 0;

		physicalDevice = physicalDevices[selectedDevice];

		// Store properties (including limits), features and memory properties of the phyiscal device (so that examples can check against them)
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);

		// Store Properties features, limits and properties of the physical device for later use
		// Device properties also contain limits and sparse properties
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);
		// Features should be checked by the examples before using them
		vkGetPhysicalDeviceFeatures(physicalDevice, &features);
		// Memory properties are used regularly for creating all kinds of buffers
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
		// Queue family properties, used for setting up requested queues upon device creation
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		assert(queueFamilyCount > 0);
		queueFamilyProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

		// Get list of supported extensions
		uint32_t extCount = 0;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, nullptr);
		if (extCount > 0)
		{
			std::vector<VkExtensionProperties> extensions(extCount);
			if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
			{
				for (auto ext : extensions)
				{
					supportedExtensions.push_back(ext.extensionName);
				}
			}
		}

		VkResult res = vulkanDevice->createLogicalDevice(enabledFeatures, enabledExtensions);
		if (res != VK_SUCCESS) {
			vks::tools::exitFatal("Could not create Vulkan device: \n" + vks::tools::errorString(res), "Fatal error");
		}
		device = vulkanDevice->logicalDevice;

		// Get a graphics queue from the device
		vkGetDeviceQueue(device, vulkanDevice->queueFamilyIndices.graphics, 0, &queue);

		// Find a suitable depth format
		VkBool32 validDepthFormat = vks::tools::getSupportedDepthFormat(physicalDevice, &depthFormat);
		assert(validDepthFormat);

		swapChain.connect(instance, physicalDevice, device);

		// Create synchronization objects
		VkSemaphoreCreateInfo semaphoreCreateInfo = vks::initializers::semaphoreCreateInfo();
		// Create a semaphore used to synchronize image presentation
		// Ensures that the image is displayed before we start submitting new commands to the queu
		VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphores.presentComplete));
		// Create a semaphore used to synchronize command submission
		// Ensures that the image is not presented until all commands have been sumbitted and executed
		VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphores.renderComplete));
		// Create a semaphore used to synchronize command submission
		// Ensures that the image is not presented until all commands for the text overlay have been sumbitted and executed
		// Will be inserted after the render complete semaphore if the text overlay is enabled
		VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphores.textOverlayComplete));

		// Set up submit info structure
		// Semaphores will stay the same during application lifetime
		// Command buffer submission info is set by each example
		submitInfo = vks::initializers::submitInfo();
		submitInfo.pWaitDstStageMask = &submitPipelineStages;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &semaphores.presentComplete;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &semaphores.renderComplete;
	}

	bool VulkanRenderDevice::UpdateSwapChain(int width, int height)
	{
		IVulkanRenderTargetView* defaultRenderTargetView;

		IVulkanTexture2D* defaultDepthTexture;
		IVulkanDepthStencilView* defaultDepthStencilView;

		m_pSwapChain->ResizeBuffers(1, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

		// Create default rendertarget view representing the backbuffer
		IVulkanTexture2D* backbufferTexture;
		m_pSwapChain->GetBuffer(0, IID_IVulkanTexture2D, (void**)&backbufferTexture);
		if (backbufferTexture)
		{
			m_pD3DDevice->CreateRenderTargetView(backbufferTexture, 0, &defaultRenderTargetView);
			backbufferTexture->Release();
		}
		else
		{
			LogError("Failed to get backbuffer texture");
			return false;
		}

		// Create default depth-stencil texture and view
		Vulkan_TEXTURE2D_DESC depthDesc;
		memset(&depthDesc, 0, sizeof depthDesc);
		depthDesc.Width = (UINT)width;
		depthDesc.Height = (UINT)height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.SampleDesc.Count = (UINT)1/*multiSample_*/;
		depthDesc.SampleDesc.Quality = 1/*multiSample_*/ > 1 ? 0xffffffff : 0;
		depthDesc.Usage = Vulkan_USAGE_DEFAULT;
		depthDesc.BindFlags = Vulkan_BIND_DEPTH_STENCIL;
		depthDesc.CPUAccessFlags = 0;
		depthDesc.MiscFlags = 0;
		m_pD3DDevice->CreateTexture2D(&depthDesc, 0, &defaultDepthTexture);
		if (defaultDepthTexture)
		{
			m_pD3DDevice->CreateDepthStencilView(defaultDepthTexture, 0, &defaultDepthStencilView);
		}
		else
		{
			LogError("Failed to create backbuffer depth-stencil texture");
			return false;
		}

		m_pDepthStencil = defaultDepthStencilView;
		m_pRenderTarget[0] = defaultRenderTargetView;

		m_pDefaultRenderTargetTexture = new VulkanTexture();
		m_pDefaultRenderTargetTexture->SetRenderTargetView(defaultRenderTargetView);
	
		m_pDefaultDepthStencilTexture = new VulkanTexture();
		m_pDefaultDepthStencilTexture->SetDepthStencilView(defaultDepthStencilView);
		m_pDefaultDepthStencilTexture->SetTexture2D(defaultDepthTexture);	

		SetViewport(Rectangle(0, 0, (float)width, (float)height));

		return true;
	}
	
	bool VulkanRenderDevice::TestDeviceLost()
	{
		return false;
	}

	bool VulkanRenderDevice::Rest()
	{
		return true;
	}

	void VulkanRenderDevice::BeginRender()
	{
		IVulkanShaderResourceView* pTextures[MAX_TEXTURE_UNITS];
		for (int i = 0;i< MAX_TEXTURE_UNITS;++i)
		{
			pTextures[i] = NULL;
		}
		GetVulkanDxDeviveContext()->VSSetShaderResources(0, MAX_TEXTURE_UNITS, &pTextures[0]);
		GetVulkanDxDeviveContext()->PSSetShaderResources(0, MAX_TEXTURE_UNITS, &pTextures[0]);
	}

	void VulkanRenderDevice::EndRender()
	{
		HRESULT hr = S_OK;
		hr = m_pSwapChain->Present(0,0);
		ASSERT( hr == S_OK);
	}

	void VulkanRenderDevice::SetFrameBuffer(FrameBuffer* pFB)
	{
		for (uint32 i = 0; i < MAX_RENDERTARGETS; ++i)
		{
			VulkanTexture* pVulkanTexture = (VulkanTexture*)(pFB->m_arrColor[i].get());
			if (pVulkanTexture)
			{
				m_pRenderTarget[i] = pVulkanTexture->GetRenderTargetView();

				DetachSRV(pVulkanTexture->GetShaderResourceView());
			}
			else
			{
				m_pRenderTarget[i] = NULL;
			}
		}

		VulkanTexture* pVulkanTexture = (VulkanTexture*)(pFB->m_pDepthStencil.get());
		if (pVulkanTexture)
		{
			m_pDepthStencil = pVulkanTexture->GetDepthStencilView();

			DetachSRV(pVulkanTexture->GetShaderResourceView());
		}
		else
		{
			m_pDepthStencil = NULL;
		}

		m_pDeviceContext->OMSetRenderTargets(MAX_RENDERTARGETS, &m_pRenderTarget[0], m_pDepthStencil);
	}

	void VulkanRenderDevice::SetRenderTarget(int index,Texture* pTexture,int level, int array_index, int face)
	{
		VulkanTexture* pVulkanTexture = (VulkanTexture*)(pTexture);
		if (pVulkanTexture)
		{
			m_pRenderTarget[index] = pVulkanTexture->GetRenderTargetView(level,array_index,face);
		
			DetachSRV(pVulkanTexture->GetShaderResourceView());
		}
		else
		{
			m_pRenderTarget[index] = NULL;
		}

		m_pDeviceContext->OMSetRenderTargets(MAX_RENDERTARGETS, &m_pRenderTarget[0], m_pDepthStencil);
	}


	void VulkanRenderDevice::SetDepthStencil(Texture* pTexture)
	{
		VulkanTexture* pVulkanTexture = (VulkanTexture*)(pTexture);
		m_pDepthStencil = pVulkanTexture->GetDepthStencilView();

		m_pDeviceContext->OMSetRenderTargets(MAX_RENDERTARGETS, &m_pRenderTarget[0], m_pDepthStencil);
	}

	Texture* VulkanRenderDevice::GetDefaultRenderTarget(int index)
	{
		return m_pDefaultRenderTargetTexture.get();
	}

	Texture* VulkanRenderDevice::GetDefaultDepthStencil()
	{
		return m_pDefaultDepthStencilTexture.get();
	}

	void VulkanRenderDevice::SetViewport(const Rectangle& rect)
	{
		Vulkan_VIEWPORT vp;
		vp.TopLeftX = rect.left;
		vp.TopLeftY = rect.top;
		vp.Width = rect.width();
		vp.Height = rect.height();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		m_pDeviceContext->RSSetViewports(1,&vp);
	}

	Rectangle VulkanRenderDevice::GetViewport()
	{
		Rectangle rect;
		UINT num = 1;

		Vulkan_VIEWPORT vp[Vulkan_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		m_pDeviceContext->RSGetViewports(&num,vp);
		rect.left = (float)vp[0].TopLeftX;
		rect.top = (float)vp[0].TopLeftY;
		rect.bottom = rect.top + (float)vp[0].Height;
		rect.right = rect.left + (float)vp[0].Width;

		return rect;
	}

	void VulkanRenderDevice::SetBlendState(const BlendState* pBlendState/*,const ColourValue& blend_factor, UINT32 sample_mask*/)
	{
		VulkanBlendStateObject* pD3DllObject = (VulkanBlendStateObject*)pBlendState;
		
		if (m_pCurBlendState != pD3DllObject->m_pVulkanBlendState)
		{
			m_pDeviceContext->OMSetBlendState(pD3DllObject->m_pVulkanBlendState, 0, M_MAX_UNSIGNED);

			m_pCurBlendState = pD3DllObject->m_pVulkanBlendState;
		}
	}

	void VulkanRenderDevice::SetDepthStencilState(const DepthStencilState* pDSState, UINT nStencilRef)
	{
		VulkanDepthStencilStateObject* pD3DllObject = (VulkanDepthStencilStateObject*)pDSState;
		
		if (m_pCurDSState != pD3DllObject->m_pVulkanDSState || m_nStencilRef != nStencilRef)
		{
			m_pDeviceContext->OMSetDepthStencilState(pD3DllObject->m_pVulkanDSState, nStencilRef);

			m_pCurDSState = pD3DllObject->m_pVulkanDSState;
			m_nStencilRef = nStencilRef;
		}
	}
	
	void VulkanRenderDevice::SetRasterizerState(const RasterizerState* pRSState)
	{
		VulkanRasterizerStateObject* pD3DllObject = (VulkanRasterizerStateObject*)pRSState;

		if (m_pCurRSState != pD3DllObject->m_pVulkanRSState)
		{
			m_pDeviceContext->RSSetState(pD3DllObject->m_pVulkanRSState);

			m_pCurRSState = pD3DllObject->m_pVulkanRSState;
		}
	}

	void VulkanRenderDevice::SetTexture(uint32 index,Texture* pTexture,bool bSRGBNotEqual)
	{
		if (pTexture != m_arrTexture[index])
		{
			if (m_nFirstDirtyTexture == M_MAX_UNSIGNED)
				m_nFirstDirtyTexture = m_nLastDirtyTexture = index;
			else
			{
				if (index < m_nFirstDirtyTexture)
					m_nFirstDirtyTexture = index;
				if (index > m_nLastDirtyTexture)
					m_nLastDirtyTexture = index;
			}

			m_arrTexture[index] = pTexture;
			if (bSRGBNotEqual)
			{
				m_arrShaderResourceView[index] = pTexture ? ((VulkanTexture*)pTexture)->GetShaderResourceView() : 0;
			}
			else
			{
				m_arrShaderResourceView[index] = pTexture ? ((VulkanTexture*)pTexture)->GetShaderResourceViewSRGBNotEqual() : 0;
			}
			m_bTexturesDirty = true;
		}
	}

	void VulkanRenderDevice::DetachSRV(IVulkanShaderResourceView* rtv_src)
	{
		bool cleared = false;
		UINT i = 0;
		for (i = 0; i < MAX_TEXTURE_UNITS; ++ i)
		{
			if (m_arrShaderResourceView[i] && m_arrShaderResourceView[i] == rtv_src)
			{
				m_arrShaderResourceView[i] = NULL;
				cleared = true;
				break;
			}
		}

		if (cleared)
		{
			m_pDeviceContext->PSSetShaderResources(i, 1, &m_arrShaderResourceView[i]);
		}
	}

	void VulkanRenderDevice::SetTexture(Uniform* uniform,Texture* pTexture)
	{
		SetTexture(uniform->m_index,pTexture,TRUE);
	}

	void VulkanRenderDevice::SetSamplerState(Uniform* uniform,SamplerState* pSampler)
	{
		uint32 index = uniform->m_index;

		SetTexture(index,pSampler->GetTexture(),pSampler->GetSRGB() == pSampler->GetTexture()->GetSRGB());

		VulkanSamplerStateObject* pVulkanSampler = (VulkanSamplerStateObject*)pSampler;
		if (pVulkanSampler->m_pImpl == NULL)
		{
			pVulkanSampler->RT_StreamComplete();
		}
		if (pVulkanSampler->m_pImpl != m_arrVulkanSampler[index])
		{
			if (m_nFirstDirtySamplerState == M_MAX_UNSIGNED)
			{
				m_nFirstDirtySamplerState = m_nLastDirtySamplerState = index;
			}
			else
			{
				if (index < m_nFirstDirtySamplerState)
				{
					m_nFirstDirtySamplerState = index;
				}
				if (index > m_nLastDirtySamplerState)
				{
					m_nLastDirtySamplerState = index;
				}
			}

			m_arrVulkanSampler[index] = pVulkanSampler->m_pImpl;
			m_bSamplerStatesDirty = true;
		}
	}

	void VulkanRenderDevice::CommitChanges()
	{
		if (m_bTexturesDirty && m_nFirstDirtyTexture < M_MAX_UNSIGNED)
		{
			m_pDeviceContext->PSSetShaderResources(m_nFirstDirtyTexture, m_nLastDirtyTexture - m_nFirstDirtyTexture + 1,
				&m_arrShaderResourceView[m_nFirstDirtyTexture]);
			m_nFirstDirtyTexture = m_nLastDirtyTexture = M_MAX_UNSIGNED;
			m_bTexturesDirty = false;
		}

		if (m_bSamplerStatesDirty && m_nFirstDirtySamplerState < M_MAX_UNSIGNED)
		{
			m_pDeviceContext->PSSetSamplers(m_nFirstDirtySamplerState, m_nLastDirtySamplerState - m_nFirstDirtySamplerState + 1,
				&m_arrVulkanSampler[m_nFirstDirtySamplerState]);

			m_nFirstDirtySamplerState = m_nLastDirtySamplerState = M_MAX_UNSIGNED;
			m_bSamplerStatesDirty = false;
		}
	}

	void VulkanRenderDevice::SetValue(Uniform* uniform, const float* values, UINT nSize)
	{
		ASSERT(uniform);
		ASSERT(values);

		ConstantBuffer* pConstantBuffer = (ConstantBuffer*)(uniform->m_pVulkanCBPtr);

		ASSERT(nSize <= uniform->m_nCBSize);
		pConstantBuffer->SetParameter(uniform->m_nCBOffset, nSize, values);
	}

	void VulkanRenderDevice::SetValue(Uniform* uniform, int value)
	{
		SetValue(uniform,(const float*)&value,sizeof(int));
	}

	void VulkanRenderDevice::SetValue(Uniform* uniform, float value)
	{
		SetValue(uniform,(const float*)&value,sizeof(float));
	}

	void VulkanRenderDevice::SetValue(Uniform* uniform, const Vector2& value)
	{
		SetValue(uniform,(const float*)&value,sizeof(Vector2));
	}

	void VulkanRenderDevice::SetValue(Uniform* uniform, const Vector3& value)
	{
		SetValue(uniform,(const float*)&value,sizeof(Vector3));
	}

	void VulkanRenderDevice::SetValue(Uniform* uniform, const Vector4* values, UINT count)
	{
		SetValue(uniform,(const float*)values,sizeof(Vector4) * count);
	}

	void VulkanRenderDevice::SetValue(Uniform* uniform, const Matrix4* values, UINT count)
	{
		SetValue(uniform,(const float*)values,sizeof(Matrix4) * count);
	}

	void VulkanRenderDevice::SetValue(Uniform* uniform, const ColourValue& value)
	{
		SetValue(uniform,(const float*)&value,12);
	}

	void VulkanRenderDevice::SetVertexDeclaration(const VertexDeclaration* pDec)
	{
		VulkanVertexDeclaration* pVulkanDec = (VulkanVertexDeclaration*)pDec;

		if (m_pCurInput != pVulkanDec->m_pImpl)
		{
			m_pDeviceContext->IASetInputLayout(pVulkanDec->m_pImpl);

			m_pCurInput = pVulkanDec->m_pImpl;
		}
	}

	void VulkanRenderDevice::SetIndexBuffer(IndexBuffer* pIB)
	{
		VulkanIndexBuffer* buffer = (VulkanIndexBuffer*)pIB;
		if (buffer != m_pIndexBuffer)
		{
			if (buffer)
				m_pDeviceContext->IASetIndexBuffer(buffer->GetD3DIndexBuffer(),
				buffer->GetStride() == sizeof(unsigned short) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
			else
				m_pDeviceContext->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);

			m_pIndexBuffer = buffer;
		}
	}

	void VulkanRenderDevice::SetVertexBuffer(int index, VertexBuffer* pVB)
	{
		VulkanVertexBuffer* pVulkanVertexBuffer = (VulkanVertexBuffer*)pVB;
		IVulkanBuffer* pBuffer = pVulkanVertexBuffer->GetD3DVertexBuffer();
		UINT nStride = pVulkanVertexBuffer->GetStride();
		UINT offset = 0; // no stream offset, this is handled in _render instead
		m_pDeviceContext->IASetVertexBuffers(index, 1, &pBuffer, &nStride, &offset);
	}

	void VulkanRenderDevice::DrawRenderable(const Renderable* pRenderable,Technique* pTech)
	{
		if (pRenderable == NULL)
			return;

		CommitChanges();	

		HRESULT hr = S_OK;

		D3D_PRIMITIVE_TOPOLOGY ePrimitiveType = VulkanMapping::GetD3DPrimitiveType(pRenderable->m_ePrimitiveType);

		const RefPtr<SubMeshData>& pSubMeshData = pRenderable->m_pSubMeshData;

		UINT nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : pRenderable->m_pIndexBuffer->GetNumber();
		UINT nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;
		
		UINT nVertexCount = pSubMeshData ? pSubMeshData->m_nVertexCount : pRenderable->m_pVertexBuffer->GetNumber();
		UINT nVertexStart = pSubMeshData ? pSubMeshData->m_nVertexStart : 0;

		UINT nPrimCount = 0;
		if (ePrimitiveType == D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		{
			nPrimCount = nIndexCount / 3;
		}
		else if (ePrimitiveType == D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
		{
			nPrimCount = nIndexCount - 2;
		}
		else if (ePrimitiveType == D3D_PRIMITIVE_TOPOLOGY_LINELIST)
		{
			nPrimCount = nIndexCount / 2;
		}

		m_pDeviceContext->IASetPrimitiveTopology( ePrimitiveType );

		m_pDeviceContext->DrawIndexed(nIndexCount,nIndexStart,nVertexStart);
	}

	void VulkanRenderDevice::ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s)
	{
		HRESULT hr = S_OK;
 
		float ColorRGBA[4];
		ColorRGBA[0] = c.r;
		ColorRGBA[1] = c.g;
		ColorRGBA[2] = c.b;
		ColorRGBA[3] = c.a;
		
		if (bColor)
		{
			for (UINT i = 0; i < MAX_RENDERTARGETS; ++i)
			{
				if (m_pRenderTarget[i])
				{
					m_pDeviceContext->ClearRenderTargetView(m_pRenderTarget[i], ColorRGBA);
				}
			}
		}

		if (bDepth || bStencil)
		{
			unsigned depthClearFlags = 0;
			if (bDepth)
				depthClearFlags |= Vulkan_CLEAR_DEPTH;
			if (bStencil)
				depthClearFlags |= Vulkan_CLEAR_STENCIL;
			m_pDeviceContext->ClearDepthStencilView(m_pDepthStencil, depthClearFlags, z, (uint8)s);
		}

		ASSERT(hr == S_OK && "Clear buffer failed.");
	}

	Matrix4 VulkanRenderDevice::MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf)
	{
		float yScale = Math::Tan(Math::HALF_PI - fovy*0.5f);
		float xScale = yScale/Aspect;
		float inv = 1.f/(zn - zf);

		out[0][0] = xScale; out[0][1] = 0.f;    out[0][2] = 0.f;    out[0][3] = 0.f;
		out[1][0] = 0.f;    out[1][1] = yScale; out[1][2] = 0.f;    out[1][3] = 0.f;
		out[2][0] = 0.f;    out[2][1] = 0.f;    out[2][2] = zf*inv; out[2][3] = zn*zf*inv;
		out[3][0] = 0.f;    out[3][1] = 0.f;    out[3][2] = -1;     out[3][3] = 0.f;

		 return out;
	}

	Matrix4 VulkanRenderDevice::MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf)
	{
		out[0][0] = 2/width;
		out[1][1] = 2/height;
		out[2][2] = 1/(zn-zf);
		out[3][3] = 1.f;
		out[2][3] = zn/(zn-zf);
		out[1][0] = out[2][0] = out[3][0] = 
			out[0][1] = out[0][2] = out[0][3] =
			out[1][2] = out[1][3] =
			out[2][1] = 
			out[3][1] = out[3][2] = 0;

		return out;
	}

	Matrix4 VulkanRenderDevice::MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf)
	{
		out[0][0] = 2/(right-left);
		out[1][1] = 2/(top-bottom);
		out[2][2] = 1/(zn-zf);
		out[3][3] = 1.f;
		out[2][3] = zn/(zn-zf);
		out[1][0] = out[2][0] = out[3][0] = 
			out[0][1] = out[0][2] = 
			out[1][2] = 
			out[2][1] = 
			out[3][1] = out[3][2] = 0;
		out[0][3] = (left+right)/(left-right);
		out[1][3] = (top+bottom)/(bottom-top);
		return out;
	}

	void VulkanRenderDevice::BeginProfile(const char* pszLale)
	{
// 		wchar_t buf[128]; 
// 		size_t outCount=0; 
// 		mbstowcs_s(&outCount, buf, pszLale, _countof(buf)-1); 
// 		D3DPERF_BeginEvent(0xff00ff00, buf); 
	}

	void VulkanRenderDevice::EndProfile()
	{
		//D3DPERF_EndEvent();
	}

	bool VulkanRenderDevice::CheckTextureFormat(PixelFormat eFormat,TEXTURE_USAGE eUsage)
	{
// 		DWORD D3DUsage =  VulkanMapping::GetD3DTextureUsage(eUsage); 
// 		D3DFORMAT D3DFormat = VulkanMapping::GetD3DFormat(eFormat);
// 
// 		HRESULT hr = D3DXCheckTextureRequirements(m_pD3DDevice, NULL, NULL, NULL, D3DUsage, &D3DFormat, D3DPOOL_DEFAULT);
// 		return hr == D3D_OK;

		return false;
	}

	void VulkanRenderDevice::NotifyResourceCreated(VulkanResource* pResource)
	{
	}

	void VulkanRenderDevice::NotifyResourceDestroyed(VulkanResource* pResource)
	{
	}

	bool VulkanRenderDevice::BuildDeviceCapabilities()
	{
		GetDeviceCapabilities()->SetShadowMapColorFormat(PF_A8R8G8B8);
		GetDeviceCapabilities()->SetShadowMapDepthFormat(PF_D24S8);
	
		GetDeviceCapabilities()->log();
		return true;
	}
}



