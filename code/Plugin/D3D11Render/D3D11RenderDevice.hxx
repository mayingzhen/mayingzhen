#include "D3D11RenderDevice.h"
#include "D3D11Texture.h"
#include "D3D11ConstantBuffer.h"


namespace ma
{

	ID3D11Device* GetD3D11DxDevive()
	{
		D3D11RenderDevice* pRender = (D3D11RenderDevice*)GetRenderDevice();
		ASSERT(pRender);
		return pRender->GetDXDevive();
	}

	ID3D11DeviceContext* GetD3D11DxDeviveContext()
	{
		D3D11RenderDevice* pRender = (D3D11RenderDevice*)GetRenderDevice();
		ASSERT(pRender);
		return pRender->GetDXDeviveContext();
	}

	D3D11RenderDevice::D3D11RenderDevice()
	{
		m_pD3DDevice = NULL;
		m_pDeviceContext = NULL;
		m_pSwapChain = NULL;

		ClearAllStates();

		m_bRenderTargetsDirty = true;
		m_bRasterizerStateDirty = true;
		m_bDepthStateDirty = true;
		m_bBlendStateDirty = true;
		m_pShader = NULL;
		m_pVertexDecl = NULL;

		firstDirtyVB_ = 0;
		lastDirtyVB_ = 0;
		memset(vertexBuffers_,0,sizeof(vertexBuffers_));
		memset(vertexSizes_,0,sizeof(vertexSizes_));
		memset(vertexOffsets_,0,sizeof(vertexOffsets_));
		//memset(elementMasks_,0,sizeof(elementMasks_));
	

	 	memset(textures_,0,sizeof(textures_));
		memset(shaderResourceViews_,0,sizeof(shaderResourceViews_));
		firstDirtyTexture_ = M_MAX_UNSIGNED;
		lastDirtyTexture_ = M_MAX_UNSIGNED;
		texturesDirty_ = true;

		memset(d3d11Samplers_,0,sizeof(d3d11Samplers_));
		memset(samplerStates,0,sizeof(samplerStates));
		firstDirtySamplerState_ = M_MAX_UNSIGNED;
		lastDirtySamplerState_ = M_MAX_UNSIGNED;
		samplerStatesDirty_ = true;

		m_pDepthStencil = NULL;
		memset(m_pRenderTarget,0,sizeof(m_pRenderTarget));


		for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
		{
			constantBuffers_[VS][i] = 0;
			constantBuffers_[PS][i] = 0;
		}
	}

	D3D11RenderDevice::~D3D11RenderDevice()
	{
	}

	void D3D11RenderDevice::ClearAllStates()
	{
	}

	Texture* D3D11RenderDevice::CreateTexture()
	{
		return new D3D11Texture();
	}

	Texture* D3D11RenderDevice::CreateRenderTarget(int nWidth,int nHeight,PixelFormat format,bool bTypeLess,bool bSRGB)
	{
		return new D3D11Texture(nWidth,nHeight,format,bTypeLess,bSRGB,USAGE_RENDERTARGET);
	}

	Texture* D3D11RenderDevice::CreateDepthStencil(int nWidth,int nHeight,PixelFormat format,bool bTypeLess)
	{
		return new D3D11Texture(nWidth,nHeight,format,bTypeLess,false,USAGE_DEPTHSTENCIL);
	}

	VertexDeclaration* D3D11RenderDevice::CreateVertexDeclaration()
	{
		return new D3D11VertexDeclaration();
	}

	VertexBuffer*	D3D11RenderDevice::CreateVertexBuffer()
	{
		return new D3D11VertexBuffer();
	}

	IndexBuffer*	D3D11RenderDevice::CreateIndexBuffer()
	{
		return new D3D11IndexBuffer();
	}

	ShaderProgram*	D3D11RenderDevice::CreateShaderProgram()
	{
		return new D3D11ShaderProgram();
	}


	void D3D11RenderDevice::Shoutdown()
	{
		m_hWnd = NULL;

		for (map<unsigned, ID3D11RasterizerState*>::iterator it = rasterizerStates_.begin(); 
			it != rasterizerStates_.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}

		for (map<unsigned, ID3D11DepthStencilState*>::iterator it = depthStates_.begin(); 
			it != depthStates_.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}

		for (map<unsigned, ID3D11BlendState*>::iterator it = blendStates_.begin(); 
			it != blendStates_.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}

		for (map<SamplerState, ID3D11SamplerState*>::iterator it = SamplerStatesAll_.begin(); 
			it != SamplerStatesAll_.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}
		SamplerStatesAll_.clear();

		for (map<uint64, ID3D11InputLayout* >::iterator it = vertexDeclarations_.begin(); 
			it != vertexDeclarations_.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}

		constantBufferAll_.clear();

		SAFE_RELEASE(m_pDeviceContext);
		SAFE_RELEASE(m_pSwapChain);

// #if defined(DEBUG) || defined(_DEBUG)
// 		ID3D11Debug *d3dDebug;
// 		HRESULT hr = m_pD3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
// 		if (SUCCEEDED(hr))
// 		{
// 			hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
// 		}
// 		if (d3dDebug != NULL)			
// 			d3dDebug->Release();
// #endif

		SAFE_RELEASE(m_pD3DDevice);
	}

	void D3D11RenderDevice::Init(HWND wndhandle)
	{
		m_hWnd = wndhandle;

#if defined(DEBUG) || defined(_DEBUG)
		UINT nCreateFlags = D3D10_CREATE_DEVICE_DEBUG;
#else
		UINT nCreateFlags = 0;
#endif

		D3D11CreateDevice(
			0,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			nCreateFlags,
			0,
			0,
			D3D11_SDK_VERSION,
			&m_pD3DDevice,
			0,
			&m_pDeviceContext
			);

		ASSERT(m_pD3DDevice && m_pDeviceContext);
		if(!m_pD3DDevice || !m_pDeviceContext)
		{
			LogError("Failed to create D3D11 device");
			return;
		}

		RECT rect;
		GetClientRect(m_hWnd,&rect);
		
		UINT width = rect.right - rect.left;
		UINT height = rect.bottom - rect.top;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		memset(&swapChainDesc, 0, sizeof swapChainDesc);
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = (UINT)width;
		swapChainDesc.BufferDesc.Height = (UINT)height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = wndhandle;
		swapChainDesc.SampleDesc.Count = (UINT)1/*multiSample*/;
		swapChainDesc.SampleDesc.Quality = 1/*multiSample*/ > 1 ? 0xffffffff : 0;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGIDevice* dxgiDevice = 0;
		m_pD3DDevice->QueryInterface(IID_IDXGIDevice, (void**)&dxgiDevice);
		IDXGIAdapter* dxgiAdapter = 0;
		dxgiDevice->GetParent(IID_IDXGIAdapter, (void**)&dxgiAdapter);
		IDXGIFactory* dxgiFactory = 0;
		dxgiAdapter->GetParent(IID_IDXGIFactory, (void**)&dxgiFactory);
		dxgiFactory->CreateSwapChain(m_pD3DDevice, &swapChainDesc, &m_pSwapChain);
		// After creating the swap chain, disable automatic Alt-Enter fullscreen/windowed switching
		// (the application will switch manually if it wants to)
		dxgiFactory->MakeWindowAssociation(wndhandle, DXGI_MWA_NO_ALT_ENTER);

		dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();

		ASSERT(m_pSwapChain);
		if (m_pSwapChain == NULL)
		{
			LogError("Failed to create D3D11 swap chain");
			return;
		}

		UpdateSwapChain(width,height);

		BuildDeviceCapabilities();
	}

	bool D3D11RenderDevice::UpdateSwapChain(int width, int height)
	{
		ID3D11RenderTargetView* defaultRenderTargetView;

		ID3D11Texture2D* defaultDepthTexture;
		ID3D11DepthStencilView* defaultDepthStencilView;

		m_pSwapChain->ResizeBuffers(1, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

		// Create default rendertarget view representing the backbuffer
		ID3D11Texture2D* backbufferTexture;
		m_pSwapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backbufferTexture);
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
		D3D11_TEXTURE2D_DESC depthDesc;
		memset(&depthDesc, 0, sizeof depthDesc);
		depthDesc.Width = (UINT)width;
		depthDesc.Height = (UINT)height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.SampleDesc.Count = (UINT)1/*multiSample_*/;
		depthDesc.SampleDesc.Quality = 1/*multiSample_*/ > 1 ? 0xffffffff : 0;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthDesc.CPUAccessFlags = 0;
		depthDesc.MiscFlags = 0;
		m_pD3DDevice->CreateTexture2D(&depthDesc, 0, &defaultDepthTexture);
		if (defaultDepthTexture)
			m_pD3DDevice->CreateDepthStencilView(defaultDepthTexture, 0, &defaultDepthStencilView);
		else
		{
			LogError("Failed to create backbuffer depth-stencil texture");
			return false;
		}

		m_pDepthStencil = defaultDepthStencilView;
		m_pRenderTarget[0] = defaultRenderTargetView;

		defaultRenderTargetTexture = new D3D11Texture();
		defaultRenderTargetTexture->SetRenderTargetView(defaultRenderTargetView);
	
		defaultDepthStencilTexture = new D3D11Texture();
		defaultDepthStencilTexture->SetDepthStencilView(defaultDepthStencilView);
		defaultDepthStencilTexture->SetTexture2D(defaultDepthTexture);	

		SetViewport(Rectangle(0, 0, (float)width, (float)height));

		return true;
	}
	
	bool D3D11RenderDevice::TestDeviceLost()
	{
		return false;
	}

	bool D3D11RenderDevice::Rest()
	{
		return true;
	}

	void D3D11RenderDevice::BeginRender()
	{
		ID3D11ShaderResourceView* pTextures[MAX_TEXTURE_UNITS];
		for (int i = 0;i< MAX_TEXTURE_UNITS;++i)
		{
			pTextures[i] = NULL;
		}
		GetD3D11DxDeviveContext()->VSSetShaderResources(0, MAX_TEXTURE_UNITS, &pTextures[0]);
		GetD3D11DxDeviveContext()->PSSetShaderResources(0, MAX_TEXTURE_UNITS, &pTextures[0]);
	}

	void D3D11RenderDevice::EndRender()
	{
		HRESULT hr = S_OK;
		hr = m_pSwapChain->Present(0,0);
		ASSERT( hr == S_OK);
	}

	void D3D11RenderDevice::SetFrameBuffer(FrameBuffer* pFB)
	{
		for (uint32 i = 0; i < MAX_RENDERTARGETS; ++i)
		{
			D3D11Texture* pD3D11Texture = (D3D11Texture*)(pFB->m_arrColor[i].get());
			if (pD3D11Texture)
				m_pRenderTarget[i] = pD3D11Texture->GetRenderTargetView();
			else
				m_pRenderTarget[i] = NULL;
		}

		D3D11Texture* pD3D11Texture = (D3D11Texture*)(pFB->m_pDepthStencil.get());
		m_pDepthStencil = pD3D11Texture->GetDepthStencilView();
	
		m_pDeviceContext->OMSetRenderTargets(MAX_RENDERTARGETS, &m_pRenderTarget[0], m_pDepthStencil);
	}

	void D3D11RenderDevice::SetRenderTarget(Texture* pTexture,int index)
	{

	}


	void D3D11RenderDevice::SetDepthStencil(Texture* pTexture)
	{

	}

	Texture* D3D11RenderDevice::GetDefaultRenderTarget(int index)
	{
		return defaultRenderTargetTexture.get();
	}

	Texture* D3D11RenderDevice::GetDefaultDepthStencil()
	{
		return defaultDepthStencilTexture.get();
	}

	void D3D11RenderDevice::SetViewport(const Rectangle& rect)
	{
		D3D11_VIEWPORT vp;
		vp.TopLeftX = rect.left;
		vp.TopLeftY = rect.top;
		vp.Width = rect.width();
		vp.Height = rect.height();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		m_pDeviceContext->RSSetViewports(1,&vp);
	}

	Rectangle D3D11RenderDevice::GetViewport()
	{
		Rectangle rect;
		UINT num = 1;

		D3D11_VIEWPORT vp[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		m_pDeviceContext->RSGetViewports(&num,vp);
		rect.left = (float)vp[0].TopLeftX;
		rect.top = (float)vp[0].TopLeftY;
		rect.bottom = rect.top + (float)vp[0].Height;
		rect.right = rect.left + (float)vp[0].Width;

		return rect;
	}

	void D3D11RenderDevice::SetDepthBias(float constantBias, float slopeScaleBias/* = 0.0f*/)
	{

	}

	void D3D11RenderDevice::SetCullingMode(CULL_MODE mode)
	{
		if (mode != m_renderState.m_eCullMode)
		{
			m_renderState.m_eCullMode = mode;
			m_bRasterizerStateDirty = true;
		}
	}

	void D3D11RenderDevice::SetBlendMode(BLEND_MODE mode)
	{
		if (mode != m_renderState.m_eBlendMode)
		{
			m_renderState.m_eBlendMode = mode;
			m_bBlendStateDirty = true;
		}
	}

	void D3D11RenderDevice::SetDepthWrite(bool b)
	{
		if (b != m_renderState.m_bDepthWrite)
		{
			m_renderState.m_bDepthWrite = b;
			m_bDepthStateDirty = true;
		}
	}

	void D3D11RenderDevice::SetColorWrite(bool b)
	{
		if (b != m_renderState.m_bColorWrite)
		{
			m_renderState.m_bColorWrite = b;
			m_bBlendStateDirty = true;
		}
	}

	void D3D11RenderDevice::SetSRGBWrite(bool b)
	{
		//m_pD3DDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, b ? TRUE : FALSE);
	}

	void D3D11RenderDevice::SetStencilEnable(bool b)
	{
		if (b != m_renderState.m_bStencil)
		{
			m_renderState.m_bStencil = b;
			m_bDepthStateDirty = true;
		}
	}

	void D3D11RenderDevice::SetStencilBufferParams(CompareFunction func/* = CMPF_ALWAYS_PASS*/, 
		uint32 refValue/* = 0*/, uint32 mask/* = 0xFFFFFFFF*/, uint32 writeMask/* = 0xFFFFffff*/,
		StencilOperation stencilFailOp/* = SOP_KEEP*/, 
		StencilOperation depthFailOp/* = SOP_KEEP*/,
		StencilOperation passOp/* = SOP_KEEP*/, 
		bool twoSidedOperation/* = false*/)
	{

	}

	void D3D11RenderDevice::SetDepthCheckMode(CompareFunction mode)
	{
		if (mode != m_renderState.m_eDepthCheckMode)	
		{
			m_renderState.m_eDepthCheckMode = mode;
			m_bDepthStateDirty = true;
		}
	}

	void D3D11RenderDevice::SetTexture(uint32 index,Texture* pTexture)
	{
		if (pTexture != textures_[index])
		{
			if (firstDirtyTexture_ == M_MAX_UNSIGNED)
				firstDirtyTexture_ = lastDirtyTexture_ = index;
			else
			{
				if (index < firstDirtyTexture_)
					firstDirtyTexture_ = index;
				if (index > lastDirtyTexture_)
					lastDirtyTexture_ = index;
			}

			textures_[index] = pTexture;
			shaderResourceViews_[index] = pTexture ? ((D3D11Texture*)pTexture)->GetShaderResourceView() : 0;
			texturesDirty_ = true;
		}
	}

	void D3D11RenderDevice::SetTexture(Uniform* uniform,Texture* pTexture)
	{
		SetTexture(uniform->m_index,pTexture);
	}

	ID3D11SamplerState* D3D11RenderDevice::CreateOrGetSamplerState(SamplerState* pSampler)
	{
		map<SamplerState, ID3D11SamplerState*>::iterator it = SamplerStatesAll_.find(*pSampler);
		if (it != SamplerStatesAll_.end())
		{
			return it->second;
		}
		else
		{
			ID3D11SamplerState* sample = NULL;

			D3D11_SAMPLER_DESC samplerDesc;
			memset(&samplerDesc, 0, sizeof samplerDesc);

			samplerDesc.Filter = D3D11Mapping::GetD3D11Filter(pSampler->GetFilterMode()); 
			samplerDesc.AddressU =  D3D11Mapping::GetD3D11Wrap(pSampler->GetWrapMode());
			samplerDesc.AddressV = D3D11Mapping::GetD3D11Wrap(pSampler->GetWrapMode());
			samplerDesc.AddressW = D3D11Mapping::GetD3D11Wrap(pSampler->GetWrapModeW());
			samplerDesc.MaxAnisotropy = 1;//graphics_->GetTextureAnisotropy();
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			if (pSampler->GetFilterMode() == TFO_SHADOWCOMPARE)
			{
				samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;	 
			}

			m_pD3DDevice->CreateSamplerState(&samplerDesc, &sample);

			SamplerStatesAll_[*pSampler] = sample;

			return sample;
		}
	}

	void D3D11RenderDevice::SetSamplerState(Uniform* uniform,SamplerState* pSampler)
	{
		SetTexture(uniform,pSampler->GetTexture());

		uint32 index = uniform->m_index;
		if (pSampler != samplerStates[index])
		{
			if (firstDirtySamplerState_ == M_MAX_UNSIGNED)
				firstDirtySamplerState_ = lastDirtySamplerState_ = index;
			else
			{
				if (index < firstDirtySamplerState_)
					firstDirtySamplerState_ = index;
				if (index > lastDirtySamplerState_)
					lastDirtySamplerState_ = index;
			}

			samplerStates[index] = pSampler;
			d3d11Samplers_[index] = CreateOrGetSamplerState(pSampler);
			samplerStatesDirty_ = true;
		}
	}

	void D3D11RenderDevice::CommitChanges()
	{
		if (texturesDirty_ && firstDirtyTexture_ < M_MAX_UNSIGNED)
		{
			m_pDeviceContext->PSSetShaderResources(firstDirtyTexture_, lastDirtyTexture_ - firstDirtyTexture_ + 1,
				&shaderResourceViews_[firstDirtyTexture_]);
			firstDirtyTexture_ = lastDirtyTexture_ = M_MAX_UNSIGNED;
			texturesDirty_ = false;
		}

		if (samplerStatesDirty_ && firstDirtySamplerState_ < M_MAX_UNSIGNED)
		{
			m_pDeviceContext->PSSetSamplers(firstDirtySamplerState_, lastDirtySamplerState_ - firstDirtySamplerState_ + 1,
				&d3d11Samplers_[firstDirtySamplerState_]);

			firstDirtySamplerState_ = lastDirtySamplerState_ = M_MAX_UNSIGNED;
			samplerStatesDirty_ = false;
		}

		if (vertexDeclarationDirty_ && m_pShader && m_pShader->GetByteVSCodeSize())
		{
			uint64 newVertexDeclarationHash = m_pVertexDecl->GetHash();

			// Do not create input layout if no vertex buffers / elements
			if (newVertexDeclarationHash)
			{
				if (newVertexDeclarationHash != vertexDeclarationHash_)
				{
					map<unsigned long long, ID3D11InputLayout* >::iterator
						it = vertexDeclarations_.find(newVertexDeclarationHash);
					if (it == vertexDeclarations_.end())
					{
						D3D11_INPUT_ELEMENT_DESC d3dve[MAX_ELEMENT];
						for (int i = 0; i < m_pVertexDecl->GetElementCount(); ++i)
						{
							const VertexElement& element = m_pVertexDecl->GetElement(i);
							d3dve[i].SemanticName = D3D11Mapping::GetD3DDeclUsage(element.Usage);
							d3dve[i].SemanticIndex = element.UsageIndex;
							d3dve[i].Format = D3D11Mapping::GetD3DDeclType(element.Type);
							d3dve[i].InputSlot = 0;
							d3dve[i].AlignedByteOffset = element.Offset;
							d3dve[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
							d3dve[i].InstanceDataStepRate =0;
						}
						
						ID3D11InputLayout* pD3D11VertexDecl = NULL;
						GetD3D11DxDevive()->CreateInputLayout(&d3dve[0], m_pVertexDecl->GetElementCount(), m_pShader->GetByteVSCode(),
							m_pShader->GetByteVSCodeSize(), &pD3D11VertexDecl);
						ASSERT(pD3D11VertexDecl);
						if (pD3D11VertexDecl == NULL)
						{
							LogError("Failed to create input layout");
						}

						vertexDeclarations_.insert(std::make_pair(newVertexDeclarationHash, pD3D11VertexDecl));
						m_pDeviceContext->IASetInputLayout(pD3D11VertexDecl);
					}
					else
					{
						m_pDeviceContext->IASetInputLayout(it->second);
					}

					vertexDeclarationHash_ = newVertexDeclarationHash;
				}
			}

			vertexDeclarationDirty_ = false;
		}

		if (m_bBlendStateDirty)
		{
			unsigned newBlendStateHash = (unsigned)((m_renderState.m_bColorWrite ? 1 : 0) | (m_renderState.m_eBlendMode << 1));
			if (newBlendStateHash != blendStateHash_)
			{
				map<unsigned, ID3D11BlendState*>::iterator i = blendStates_.find(newBlendStateHash);
				if (i == blendStates_.end())
				{
					//PROF(CreateBlendState);

					D3D11_BLEND_DESC stateDesc;
					memset(&stateDesc, 0, sizeof stateDesc);
					stateDesc.AlphaToCoverageEnable = false;
					stateDesc.IndependentBlendEnable = false;

					D3D11Mapping::GetD3DBlend(m_renderState.m_eBlendMode,
						stateDesc.RenderTarget[0].BlendEnable,
						stateDesc.RenderTarget[0].SrcBlend,
						stateDesc.RenderTarget[0].DestBlend,
						stateDesc.RenderTarget[0].BlendOp);

					D3D11Mapping::GetD3DBlend(m_renderState.m_eBlendMode,
						stateDesc.RenderTarget[0].BlendEnable,
						stateDesc.RenderTarget[0].SrcBlendAlpha,
						stateDesc.RenderTarget[0].DestBlendAlpha,
						stateDesc.RenderTarget[0].BlendOpAlpha);

					stateDesc.RenderTarget[0].RenderTargetWriteMask = m_renderState.m_bColorWrite ? D3D11_COLOR_WRITE_ENABLE_ALL : 0x0;

					ID3D11BlendState* newBlendState = 0;
					HRESULT hr = m_pD3DDevice->CreateBlendState(&stateDesc, &newBlendState);
					if (!newBlendState)
					{
						if (hr == D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS)
						{

						}
						LogError("Failed to create blend state");
					}

					blendStates_.insert( std::make_pair(newBlendStateHash, newBlendState) );
					
					m_pDeviceContext->OMSetBlendState(newBlendState, 0, M_MAX_UNSIGNED);
				}
				else
				{
					m_pDeviceContext->OMSetBlendState(i->second, 0, M_MAX_UNSIGNED);
				}
				blendStateHash_ = newBlendStateHash;
			}

			m_bBlendStateDirty = false;
		}

		if (m_bDepthStateDirty)
		{
			unsigned newDepthStateHash =
				(m_renderState.m_bDepthWrite ? 1 : 0) | 
				(m_renderState.m_bStencil ? 2 : 0) | 
				(m_renderState.m_eDepthCheckMode << 2) | 
				((m_renderState.m_nStencilMask & 0xff) << 5) |
				((m_renderState.m_nStencilWriteMask & 0xff) << 13) | (m_renderState.m_eStencilfunc << 21) |
				((m_renderState.m_eStencilFail + m_renderState.m_eDepthFailOp * 5 + m_renderState.m_eStencilPass * 25) << 24);
			if (newDepthStateHash != depthStateHash_ || stencilRefDirty_)
			{
				map<unsigned, ID3D11DepthStencilState*>::iterator i = depthStates_.find(newDepthStateHash);
				if (i == depthStates_.end())
				{
					D3D11_DEPTH_STENCIL_DESC stateDesc;
					memset(&stateDesc, 0, sizeof stateDesc);
					stateDesc.DepthEnable = TRUE;
					stateDesc.DepthWriteMask = m_renderState.m_bDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
					stateDesc.DepthFunc = D3D11Mapping::get(m_renderState.m_eDepthCheckMode); ;
					stateDesc.StencilEnable = m_renderState.m_bStencil ? TRUE : FALSE;
					stateDesc.StencilReadMask = (unsigned char)m_renderState.m_nStencilMask;
					stateDesc.StencilWriteMask = (unsigned char)m_renderState.m_nStencilWriteMask;
					stateDesc.FrontFace.StencilFailOp = D3D11Mapping::get(m_renderState.m_eStencilFail);
					stateDesc.FrontFace.StencilDepthFailOp = D3D11Mapping::get(m_renderState.m_eDepthFailOp);
					stateDesc.FrontFace.StencilPassOp = D3D11Mapping::get(m_renderState.m_eStencilPass);
					stateDesc.FrontFace.StencilFunc = D3D11Mapping::get(m_renderState.m_eStencilfunc);
					
					// BackFace
					stateDesc.BackFace.StencilFailOp = D3D11Mapping::get(m_renderState.m_eStencilFail);
					stateDesc.BackFace.StencilDepthFailOp = D3D11Mapping::get(m_renderState.m_eStencilFail);
					stateDesc.BackFace.StencilPassOp = D3D11Mapping::get(m_renderState.m_eStencilFail);
					stateDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

					ID3D11DepthStencilState* newDepthState = 0;
					m_pD3DDevice->CreateDepthStencilState(&stateDesc, &newDepthState);
					if (!newDepthState)
						LogError("Failed to create depth state");

					depthStates_.insert( std::make_pair(newDepthStateHash, newDepthState) );

					m_pDeviceContext->OMSetDepthStencilState(newDepthState, stencilRef_);
				}
				else
				{
					m_pDeviceContext->OMSetDepthStencilState(i->second, stencilRef_);
				}
				
				depthStateHash_ = newDepthStateHash;
			}

			m_bDepthStateDirty = false;
			stencilRefDirty_ = false;
		}

		if (m_bRasterizerStateDirty)
		{
			unsigned depthBits = 24;
			//if (depthStencil_ && depthStencil_->GetParentTexture()->GetFormat() == DXGI_FORMAT_R16_TYPELESS)
			//	depthBits = 16;
			//int scaledDepthBias = (int)(constantDepthBias_ * (1 << depthBits));

			unsigned newRasterizerStateHash =
				(m_renderState.m_bScissorTest ? 1 : 0) | 
				(m_renderState.m_eFillMode << 1) | 
				(m_renderState.m_eCullMode << 3) | 
				((*((unsigned*)&m_renderState.m_fConstantBias) & 0x1fff) << 5) |
				((*((unsigned*)&m_renderState.m_fSlopeScaleBias) & 0x1fff) << 18);
			if (newRasterizerStateHash != rasterizerStateHash_)
			{
				map<unsigned, ID3D11RasterizerState*>::iterator i = rasterizerStates_.find(newRasterizerStateHash);
				if (i == rasterizerStates_.end())
				{
					D3D11_RASTERIZER_DESC stateDesc;
					memset(&stateDesc, 0, sizeof stateDesc);
					stateDesc.FillMode = D3D11Mapping::get(m_renderState.m_eFillMode);
					stateDesc.CullMode = D3D11Mapping::get(m_renderState.m_eCullMode);
					stateDesc.FrontCounterClockwise = TRUE;
					stateDesc.DepthBias = (int)m_renderState.m_fConstantBias;
					stateDesc.DepthBiasClamp = 0.0;
					stateDesc.SlopeScaledDepthBias = m_renderState.m_fSlopeScaleBias;
					stateDesc.DepthClipEnable = TRUE;
					stateDesc.ScissorEnable = m_renderState.m_bScissorTest ? TRUE : FALSE;
					stateDesc.MultisampleEnable = FALSE;
					stateDesc.AntialiasedLineEnable = FALSE;

					ID3D11RasterizerState* newRasterizerState = 0;
					m_pD3DDevice->CreateRasterizerState(&stateDesc, &newRasterizerState);
					if (!newRasterizerState)
						LogError("Failed to create rasterizer state");

					rasterizerStates_.insert(std::make_pair(newRasterizerStateHash, newRasterizerState));

					m_pDeviceContext->RSSetState(newRasterizerState);
				}
				else
				{
					m_pDeviceContext->RSSetState(i->second);
				}

				rasterizerStateHash_ = newRasterizerStateHash;
			}

			m_bRasterizerStateDirty = false;
		}

		for (unsigned i = 0; i < dirtyConstantBuffers_.size(); ++i)
			dirtyConstantBuffers_[i]->Apply();
		dirtyConstantBuffers_.clear();

	}

	ConstantBuffer* D3D11RenderDevice::GetOrCreateConstantBuffer(ShaderType type, unsigned index, unsigned size)
	{
		// Ensure that different shader types and index slots get unique buffers, even if the size is same
		unsigned key = type | (index << 1) | (size << 4);
		map<unsigned, RefPtr<ConstantBuffer> >::iterator i = constantBufferAll_.find(key);
		if (i != constantBufferAll_.end())
		{
			return i->second.get();
		}
		else
		{
			RefPtr<ConstantBuffer> newConstantBuffer(new ConstantBuffer(/*context_*/));
			newConstantBuffer->SetSize(size);
			constantBufferAll_[key] = newConstantBuffer;
			return newConstantBuffer.get();
		}
	}

	void D3D11RenderDevice::SetValue(Uniform* uniform, const float* values, UINT nSize)
	{
		ASSERT(uniform);
		ASSERT(values);

		ConstantBuffer* pConstantBuffer = (ConstantBuffer*)(uniform->m_pD3D11CBPtr);

		if (!pConstantBuffer->IsDirty())
			dirtyConstantBuffers_.push_back(pConstantBuffer);
		ASSERT(nSize <= uniform->m_nCBSize);
		pConstantBuffer->SetParameter(uniform->m_nCBOffset, nSize, values);
	}

	void D3D11RenderDevice::SetValue(Uniform* uniform, float value)
	{
		SetValue(uniform,(const float*)&value,sizeof(float));
	}

	void D3D11RenderDevice::SetValue(Uniform* uniform, const Vector2& value)
	{
		SetValue(uniform,(const float*)&value,sizeof(Vector2));
	}

	void D3D11RenderDevice::SetValue(Uniform* uniform, const Vector3& value)
	{
		SetValue(uniform,(const float*)&value,sizeof(Vector3));
	}

	void D3D11RenderDevice::SetValue(Uniform* uniform, const Vector4* values, UINT count)
	{
		SetValue(uniform,(const float*)values,sizeof(Vector4) * count);
	}

	void D3D11RenderDevice::SetValue(Uniform* uniform, const Matrix4* values, UINT count)
	{
		SetValue(uniform,(const float*)values,sizeof(Matrix4) * count);
	}

	void D3D11RenderDevice::SetValue(Uniform* uniform, const ColourValue& value)
	{
		SetValue(uniform,(const float*)&value,12);
	}

	void D3D11RenderDevice::SetShaderProgram(ShaderProgram* pShader)
	{
		m_pShader = (D3D11ShaderProgram*)pShader;

		bool vsBuffersChanged = false;
		bool psBuffersChanged = false;

		for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
		{
			ID3D11Buffer* vsBuffer = m_pShader->vsConstantBuffers_[i] ? m_pShader->vsConstantBuffers_[i]->GetD3D11Buffer() : NULL;
			if (vsBuffer != constantBuffers_[VS][i])
			{
				constantBuffers_[VS][i] = vsBuffer;
				//shaderParameterSources_[i] = (const void*)M_MAX_UNSIGNED;
				vsBuffersChanged = true;
			}

			ID3D11Buffer* psBuffer = m_pShader->psConstantBuffers_[i] ? m_pShader->psConstantBuffers_[i]->GetD3D11Buffer() : NULL;
			if (psBuffer != constantBuffers_[PS][i])
			{
				constantBuffers_[PS][i] = psBuffer;
				//shaderParameterSources_[i] = (const void*)M_MAX_UNSIGNED;
				psBuffersChanged = true;
			}
		}

		if (vsBuffersChanged)
			m_pDeviceContext->VSSetConstantBuffers(0, MAX_SHADER_PARAMETER_GROUPS, &constantBuffers_[VS][0]);
		if (psBuffersChanged)
			m_pDeviceContext->PSSetConstantBuffers(0, MAX_SHADER_PARAMETER_GROUPS, &constantBuffers_[PS][0]);
	}

	void D3D11RenderDevice::SetVertexDeclaration(VertexDeclaration* pDec)
	{
		m_pVertexDecl = (D3D11VertexDeclaration*)pDec;
		vertexDeclarationDirty_ = true;
	}

	void D3D11RenderDevice::SetIndexBuffer(IndexBuffer* pIB)
	{
		D3D11IndexBuffer* buffer = (D3D11IndexBuffer*)pIB;
		if (buffer != indexBuffer_)
		{
			if (buffer)
				m_pDeviceContext->IASetIndexBuffer(buffer->GetD3DIndexBuffer(),
				buffer->GetStride() == sizeof(unsigned short) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
			else
				m_pDeviceContext->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);

			indexBuffer_ = buffer;
		}
	}

	void D3D11RenderDevice::SetVertexBuffer(int index, VertexBuffer* pVB)
	{
		D3D11VertexBuffer* pD3D11VertexBuffer = (D3D11VertexBuffer*)pVB;
		ID3D11Buffer* pBuffer = pD3D11VertexBuffer->GetD3DVertexBuffer();
		UINT nStride = pD3D11VertexBuffer->GetStride();
		UINT offset = 0; // no stream offset, this is handled in _render instead
		m_pDeviceContext->IASetVertexBuffers(index, 1, &pBuffer, &nStride, &offset);
	}

	void D3D11RenderDevice::DrawRenderable(const Renderable* pRenderable,Technique* pTech)
	{
		if (pRenderable == NULL)
			return;

		CommitChanges();	

		HRESULT hr = S_OK;

		D3D_PRIMITIVE_TOPOLOGY ePrimitiveType = D3D11Mapping::GetD3DPrimitiveType(pRenderable->m_ePrimitiveType);

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

	void D3D11RenderDevice::ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s)
	{
		HRESULT hr = S_OK;
 
		float ColorRGBA[4];
		ColorRGBA[0] = c.r;
		ColorRGBA[1] = c.g;
		ColorRGBA[2] = c.b;
		ColorRGBA[3] = c.a;


		if ((bColor) && m_pRenderTarget[0])
			m_pDeviceContext->ClearRenderTargetView(m_pRenderTarget[0], ColorRGBA);

		unsigned depthClearFlags = 0;
		if (bColor)
			depthClearFlags |= D3D11_CLEAR_DEPTH;
		if (bStencil)
			depthClearFlags |= D3D11_CLEAR_STENCIL;
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencil, depthClearFlags, z, (UINT8)s);

		ASSERT(hr == S_OK && "Clear buffer failed.");
	}

	Matrix4 D3D11RenderDevice::MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf)
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

	Matrix4 D3D11RenderDevice::MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf)
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

	Matrix4 D3D11RenderDevice::MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf)
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

	void D3D11RenderDevice::BeginProfile(const char* pszLale)
	{
// 		wchar_t buf[128]; 
// 		size_t outCount=0; 
// 		mbstowcs_s(&outCount, buf, pszLale, _countof(buf)-1); 
// 		D3DPERF_BeginEvent(0xff00ff00, buf); 
	}

	void D3D11RenderDevice::EndProfile()
	{
		//D3DPERF_EndEvent();
	}

	bool D3D11RenderDevice::CheckTextureFormat(PixelFormat eFormat,TEXTURE_USAGE eUsage)
	{
// 		DWORD D3DUsage =  D3D11Mapping::GetD3DTextureUsage(eUsage); 
// 		D3DFORMAT D3DFormat = D3D11Mapping::GetD3DFormat(eFormat);
// 
// 		HRESULT hr = D3DXCheckTextureRequirements(m_pD3DDevice, NULL, NULL, NULL, D3DUsage, &D3DFormat, D3DPOOL_DEFAULT);
// 		return hr == D3D_OK;

		return false;
	}

	void D3D11RenderDevice::NotifyResourceCreated(D3D11Resource* pResource)
	{
	}

	void D3D11RenderDevice::NotifyResourceDestroyed(D3D11Resource* pResource)
	{
	}

	bool D3D11RenderDevice::BuildDeviceCapabilities()
	{
		GetDeviceCapabilities()->SetShadowMapColorFormat(PF_NULL);
		GetDeviceCapabilities()->SetShadowMapDepthFormat(PF_D24S8);
	
		GetDeviceCapabilities()->log();
		return true;
	}
}



