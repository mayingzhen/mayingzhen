#include "D3D11RenderDevice.h"
#include "D3D11Texture.h"
#include "D3D11ConstantBuffer.h"
#include "D3D11RenderState.h"
#include "D3D11SamplerState.h"

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

		memset(m_arrD3d11Sampler,0,sizeof(m_arrD3d11Sampler));
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

	Texture* D3D11RenderDevice::CreateRenderTarget(int nWidth,int nHeight,UINT32 nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType)
	{
		return new D3D11Texture(nWidth,nHeight,nMipMap,format,false,bSRGB,USAGE_RENDERTARGET,eType);
	}

	Texture* D3D11RenderDevice::CreateDepthStencil(int nWidth,int nHeight,PixelFormat format,bool bTypeLess)
	{
		return new D3D11Texture(nWidth,nHeight,1,format,bTypeLess,false,USAGE_DEPTHSTENCIL,TEXTYPE_2D);
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

	BlendState*	D3D11RenderDevice::CreateBlendState()
	{
		return new D3D11BlendStateObject();
	}

	DepthStencilState*	D3D11RenderDevice::CreateDepthStencilState()
	{
		return new D3D11DepthStencilStateObject();
	}

	RasterizerState* D3D11RenderDevice::CreateRasterizerState()
	{
		return new D3D11RasterizerStateObject();
	}

	SamplerState* D3D11RenderDevice::CreateSamplerState()
	{
		return new D3D11SamplerStateObject();
	}

	Technique*  D3D11RenderDevice::CreateTechnique()
	{
		return new Technique();
	}

	void D3D11RenderDevice::Shoutdown()
	{
		m_hWnd = NULL;

		D3D11RasterizerStateObject::Clear();
		D3D11DepthStencilStateObject::Clear();
		D3D11BlendStateObject::Clear();
		D3D11VertexDeclaration::Clear();
		D3D11SamplerStateObject::Clear();
		ConstantBuffer::Clear();

		SAFE_RELEASE(m_pDeviceContext);
		SAFE_RELEASE(m_pSwapChain);

#if defined(DEBUG) || defined(_DEBUG)
		ID3D11Debug *d3dDebug;
		HRESULT hr = m_pD3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
		if (SUCCEEDED(hr))
		{
			hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		}
		if (d3dDebug != NULL)			
			d3dDebug->Release();
#endif

		SAFE_RELEASE(m_pD3DDevice);
	}

	void D3D11RenderDevice::Init(HWND wndhandle)
	{
		m_hWnd = wndhandle;

#if defined(DEBUG) || defined(_DEBUG)
		UINT nCreateFlags = D3D11_CREATE_DEVICE_DEBUG;
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

		m_pDefaultRenderTargetTexture = new D3D11Texture();
		m_pDefaultRenderTargetTexture->SetRenderTargetView(defaultRenderTargetView);
	
		m_pDefaultDepthStencilTexture = new D3D11Texture();
		m_pDefaultDepthStencilTexture->SetDepthStencilView(defaultDepthStencilView);
		m_pDefaultDepthStencilTexture->SetTexture2D(defaultDepthTexture);	

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
			{
				m_pRenderTarget[i] = pD3D11Texture->GetRenderTargetView();

				DetachSRV(pD3D11Texture->GetShaderResourceView());
			}
			else
			{
				m_pRenderTarget[i] = NULL;
			}
		}

		D3D11Texture* pD3D11Texture = (D3D11Texture*)(pFB->m_pDepthStencil.get());
		if (pD3D11Texture)
		{
			m_pDepthStencil = pD3D11Texture->GetDepthStencilView();

			DetachSRV(pD3D11Texture->GetShaderResourceView());
		}
		else
		{
			m_pDepthStencil = NULL;
		}

		m_pDeviceContext->OMSetRenderTargets(MAX_RENDERTARGETS, &m_pRenderTarget[0], m_pDepthStencil);
	}

	void D3D11RenderDevice::SetRenderTarget(int index,Texture* pTexture,int level, int array_index, int face)
	{
		D3D11Texture* pD3D11Texture = (D3D11Texture*)(pTexture);
		if (pD3D11Texture)
		{
			m_pRenderTarget[index] = pD3D11Texture->GetRenderTargetView(level,array_index,face);
		
			DetachSRV(pD3D11Texture->GetShaderResourceView());
		}
		else
		{
			m_pRenderTarget[index] = NULL;
		}

		m_pDeviceContext->OMSetRenderTargets(MAX_RENDERTARGETS, &m_pRenderTarget[0], m_pDepthStencil);
	}


	void D3D11RenderDevice::SetDepthStencil(Texture* pTexture)
	{
		D3D11Texture* pD3D11Texture = (D3D11Texture*)(pTexture);
		m_pDepthStencil = pD3D11Texture->GetDepthStencilView();

		m_pDeviceContext->OMSetRenderTargets(MAX_RENDERTARGETS, &m_pRenderTarget[0], m_pDepthStencil);
	}

	Texture* D3D11RenderDevice::GetDefaultRenderTarget(int index)
	{
		return m_pDefaultRenderTargetTexture.get();
	}

	Texture* D3D11RenderDevice::GetDefaultDepthStencil()
	{
		return m_pDefaultDepthStencilTexture.get();
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

	void D3D11RenderDevice::SetBlendState(const BlendState* pBlendState/*,const ColourValue& blend_factor, UINT32 sample_mask*/)
	{
		D3D11BlendStateObject* pD3DllObject = (D3D11BlendStateObject*)pBlendState;
		
		if (m_pCurBlendState != pD3DllObject->m_pD3D11BlendState)
		{
			m_pDeviceContext->OMSetBlendState(pD3DllObject->m_pD3D11BlendState, 0, M_MAX_UNSIGNED);

			m_pCurBlendState = pD3DllObject->m_pD3D11BlendState;
		}
	}

	void D3D11RenderDevice::SetDepthStencilState(const DepthStencilState* pDSState, UINT nStencilRef)
	{
		D3D11DepthStencilStateObject* pD3DllObject = (D3D11DepthStencilStateObject*)pDSState;
		
		if (m_pCurDSState != pD3DllObject->m_pD3D11DSState || m_nStencilRef != nStencilRef)
		{
			m_pDeviceContext->OMSetDepthStencilState(pD3DllObject->m_pD3D11DSState, nStencilRef);

			m_pCurDSState = pD3DllObject->m_pD3D11DSState;
			m_nStencilRef = nStencilRef;
		}
	}
	
	void D3D11RenderDevice::SetRasterizerState(const RasterizerState* pRSState)
	{
		D3D11RasterizerStateObject* pD3DllObject = (D3D11RasterizerStateObject*)pRSState;

		if (m_pCurRSState != pD3DllObject->m_pD3D11RSState)
		{
			m_pDeviceContext->RSSetState(pD3DllObject->m_pD3D11RSState);

			m_pCurRSState = pD3DllObject->m_pD3D11RSState;
		}
	}

	void D3D11RenderDevice::SetTexture(uint32 index,Texture* pTexture,bool bSRGBNotEqual)
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
				m_arrShaderResourceView[index] = pTexture ? ((D3D11Texture*)pTexture)->GetShaderResourceView() : 0;
			}
			else
			{
				m_arrShaderResourceView[index] = pTexture ? ((D3D11Texture*)pTexture)->GetShaderResourceViewSRGBNotEqual() : 0;
			}
			m_bTexturesDirty = true;
		}
	}

	void D3D11RenderDevice::DetachSRV(ID3D11ShaderResourceView* rtv_src)
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

	void D3D11RenderDevice::SetTexture(Uniform* uniform,Texture* pTexture)
	{
		SetTexture(uniform->m_index,pTexture,TRUE);
	}

	void D3D11RenderDevice::SetSamplerState(Uniform* uniform,SamplerState* pSampler)
	{
		uint32 index = uniform->m_index;

		SetTexture(index,pSampler->GetTexture(),pSampler->GetSRGB() == pSampler->GetTexture()->GetSRGB());

		D3D11SamplerStateObject* pD3D11Sampler = (D3D11SamplerStateObject*)pSampler;
		if (pD3D11Sampler->m_pImpl == NULL)
		{
			pD3D11Sampler->RT_StreamComplete();
		}
		if (pD3D11Sampler->m_pImpl != m_arrD3d11Sampler[index])
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

			m_arrD3d11Sampler[index] = pD3D11Sampler->m_pImpl;
			m_bSamplerStatesDirty = true;
		}
	}

	void D3D11RenderDevice::CommitChanges()
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
				&m_arrD3d11Sampler[m_nFirstDirtySamplerState]);

			m_nFirstDirtySamplerState = m_nLastDirtySamplerState = M_MAX_UNSIGNED;
			m_bSamplerStatesDirty = false;
		}
	}

	void D3D11RenderDevice::SetValue(Uniform* uniform, const float* values, UINT nSize)
	{
		ASSERT(uniform);
		ASSERT(values);

		ConstantBuffer* pConstantBuffer = (ConstantBuffer*)(uniform->m_pD3D11CBPtr);

		ASSERT(nSize <= uniform->m_nCBSize);
		pConstantBuffer->SetParameter(uniform->m_nCBOffset, nSize, values);
	}

	void D3D11RenderDevice::SetValue(Uniform* uniform, int value)
	{
		SetValue(uniform,(const float*)&value,sizeof(int));
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

	void D3D11RenderDevice::SetVertexDeclaration(const VertexDeclaration* pDec)
	{
		D3D11VertexDeclaration* pD3D11Dec = (D3D11VertexDeclaration*)pDec;

		if (m_pCurInput != pD3D11Dec->m_pImpl)
		{
			m_pDeviceContext->IASetInputLayout(pD3D11Dec->m_pImpl);

			m_pCurInput = pD3D11Dec->m_pImpl;
		}
	}

	void D3D11RenderDevice::SetIndexBuffer(IndexBuffer* pIB)
	{
		D3D11IndexBuffer* buffer = (D3D11IndexBuffer*)pIB;
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
		
		//UINT nVertexCount = pSubMeshData ? pSubMeshData->m_nVertexCount : pRenderable->m_pVertexBuffer->GetNumber();
		UINT nVertexStart = pSubMeshData ? pSubMeshData->m_nVertexStart : 0;

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
				depthClearFlags |= D3D11_CLEAR_DEPTH;
			if (bStencil)
				depthClearFlags |= D3D11_CLEAR_STENCIL;
			m_pDeviceContext->ClearDepthStencilView(m_pDepthStencil, depthClearFlags, z, (uint8)s);
		}

		ASSERT(hr == S_OK && "Clear buffer failed.");
	}

	Matrix4 D3D11RenderDevice::MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf)
	{
		float yScale = 1.0f / Math::Tan(fovy * 0.5f);
		float xScale = yScale / Aspect;
		float zScale = zf / (zn - zf);

		out[0][0] = xScale; out[0][1] = 0.f;    out[0][2] = 0.f;    out[0][3] = 0.f;
		out[1][0] = 0.f;    out[1][1] = yScale; out[1][2] = 0.f;    out[1][3] = 0.f;
		out[2][0] = 0.f;    out[2][1] = 0.f;    out[2][2] = zScale; out[2][3] = zn * zScale;
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
		GetDeviceCapabilities()->SetShadowMapColorFormat(PF_A8R8G8B8);
		GetDeviceCapabilities()->SetShadowMapDepthFormat(PF_D24S8);
	
		GetDeviceCapabilities()->log();
		return true;
	}
}



