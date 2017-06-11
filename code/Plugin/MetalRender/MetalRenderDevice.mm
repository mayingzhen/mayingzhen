#include "MetalRenderDevice.h"
#include "MetalTexture.h"
#include "MetalConstantBuffer.h"
#include "MetalRenderState.h"
#include "MetalSamplerState.h"
#include "MetalVertexDeclaration.h"
#include "MetalVertexBuffer.h"
#include "MetalIndexBuffer.h"
#include "MetalShaderProgram.h"
#include "MetalMapping.h"

#import <UIKit/UIkit.h>
#import <UIKit/UIDevice.h>

namespace ma
{

	id<MTLDevice> GetMetalDevive()
	{
		MetalRenderDevice* pRender = (MetalRenderDevice*)GetRenderDevice();
		ASSERT(pRender);
		return pRender->GetMetalDevive();
	}

	//IMetalDeviceContext* GetMetalDxDeviveContext()
	//{
	//	MetalRenderDevice* pRender = (MetalRenderDevice*)GetRenderDevice();
	//	ASSERT(pRender);
	//	return pRender->GetDXDeviveContext();
	//}

	MetalRenderDevice::MetalRenderDevice()
	{
		//m_pD3DDevice = NULL;
		//m_pDeviceContext = NULL;
		//m_pSwapChain = NULL;

		ClearAllStates();

		//m_bRenderTargetsDirty = true;

		//m_nFirstDirtyVB = 0;
		//m_nLastDirtyVB = 0;
		//memset(m_arrVertexBuffers,0,sizeof(m_arrVertexBuffers));
		//memset(m_arrVertexSize,0,sizeof(m_arrVertexSize));
		//memset(m_arrVertexOffset,0,sizeof(m_arrVertexOffset));

	 	//memset(m_arrTexture,0,sizeof(m_arrTexture));
		//memset(m_arrShaderResourceView,0,sizeof(m_arrShaderResourceView));
		//m_nFirstDirtyTexture = M_MAX_UNSIGNED;
		//m_nLastDirtyTexture = M_MAX_UNSIGNED;
		//m_bTexturesDirty = true;

		//memset(m_arrMetalSampler,0,sizeof(m_arrMetalSampler));
		//m_nFirstDirtySamplerState = M_MAX_UNSIGNED;
		//m_nLastDirtySamplerState = M_MAX_UNSIGNED;
		//m_bSamplerStatesDirty = true;

		//m_pDepthStencil = NULL;
		//memset(m_pRenderTarget,0,sizeof(m_pRenderTarget));

		//m_pCurInput = NULL;
		//m_pCurDSState = NULL;
		//m_nStencilRef = 0;
		//m_pCurBlendState = NULL;
		//m_pCurRSState = NULL;
	}

	MetalRenderDevice::~MetalRenderDevice()
	{
	}

	void MetalRenderDevice::ClearAllStates()
	{
	}

	Texture* MetalRenderDevice::CreateTexture()
	{
		return new MetalTexture();
	}

	Texture* MetalRenderDevice::CreateRenderTarget(int nWidth,int nHeight,UINT32 nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType)
	{
		return new MetalTexture(nWidth,nHeight,nMipMap,format,false,bSRGB,USAGE_RENDERTARGET,eType);
	}

	Texture* MetalRenderDevice::CreateDepthStencil(int nWidth,int nHeight,PixelFormat format,bool bTypeLess)
	{
		return new MetalTexture(nWidth,nHeight,1,format,bTypeLess,false,USAGE_DEPTHSTENCIL,TEXTYPE_2D);
	}

	VertexDeclaration* MetalRenderDevice::CreateVertexDeclaration()
	{
		return new MetalVertexDeclaration();
	}

	VertexBuffer*	MetalRenderDevice::CreateVertexBuffer()
	{
		return new MetalVertexBuffer();
	}

	IndexBuffer*	MetalRenderDevice::CreateIndexBuffer()
	{
		return new MetalIndexBuffer();
	}

	ShaderProgram*	MetalRenderDevice::CreateShaderProgram()
	{
		return new MetalShaderProgram();
	}

	BlendState*	MetalRenderDevice::CreateBlendState()
	{
		return new MetalBlendStateObject();
	}

	DepthStencilState*	MetalRenderDevice::CreateDepthStencilState()
	{
		return new MetalDepthStencilStateObject();
	}

	RasterizerState* MetalRenderDevice::CreateRasterizerState()
	{
		return new MetalRasterizerStateObject();
	}

	SamplerState* MetalRenderDevice::CreateSamplerState()
	{
		return new MetalSamplerStateObject();
	}

	void MetalRenderDevice::Shoutdown()
	{
		//m_hWnd = NULL;

		MetalRasterizerStateObject::Clear();
		MetalDepthStencilStateObject::Clear();
		MetalBlendStateObject::Clear();
		MetalVertexDeclaration::Clear();
		MetalSamplerStateObject::Clear();
		ConstantBuffer::Clear();

	}

	void MetalRenderDevice::Init(HWND wndhandle)
	{
		//m_hWnd = wndhandle;


        //ASSERT(m_device == nil && m_command_queue == nil && m_command_buffer.mBuffer == nil);
    
        m_device = MTLCreateSystemDefaultDevice();
        
        m_command_queue = [m_device newCommandQueue];
        
          _inflight_semaphore = dispatch_semaphore_create(/*kInFlightCommandBuffers*/3);
    
        //m_command_buffer.mBuffer = [[m_command_queue commandBuffer] retain];
   
        CALayer* layer = (CALayer*)wndhandle;
        if ([layer isKindOfClass:[CAMetalLayer class]])
        {
            //LogError("Layer of view is not CAMetalLayer");
            //return ;
        }
        m_layer = [(CAMetalLayer*)layer retain];
        //view.opaque = YES;
        
        
        //param.ZBufferBits = 32;
        //param.StencilBits = 8;
        // 像素格式只支持RGBA8
        //m_pixel_format = PixelFormat::A8R8G8B8();
        //m_depth_pixel_format = PixelFormat::D24S8();
        m_layer.device = m_device;
        m_layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        //m_layer.drawableSize = CGSizeMake(param.Width, param.Height);
        m_layer.framebufferOnly = YES;
        m_layer.presentsWithTransaction = NO;
        m_layer.contentsScale = [UIScreen mainScreen].scale;
        
        //memcpy(&m_param, &param, sizeof(SwapChainParam));
        //m_msaa = (param.MSAA == 0 ? 1 : param.MSAA);
        //m_width = param.Width;
        //m_height = param.Height;
        //m_device_impl = impl;
        //m_render_pass = new MetalRenderPass(impl->GetDevice());
        //m_render_pass->SetBoundSwapChain(this);
        m_drawable = [[m_layer nextDrawable] retain];
        
        m_pass_desc = [[MTLRenderPassDescriptor renderPassDescriptor] retain];
        m_pipe_desc = [[MTLRenderPipelineDescriptor alloc] init];
 
        id<MTLTexture> colorTex = m_drawable.texture;
        m_pass_desc.colorAttachments[0].texture = colorTex;
        m_pass_desc.colorAttachments[0].storeAction = MTLStoreActionStore;
        m_pipe_desc.colorAttachments[0].pixelFormat = colorTex.pixelFormat;
        
        CGSize screen_size = [[UIScreen mainScreen] bounds].size;
        CGFloat scale = [UIScreen mainScreen].scale;
        CGSize resolution = CGSizeMake(screen_size.width * scale, screen_size.height * scale);
        
        NSUInteger width = resolution.width;
        NSUInteger height = resolution.height;
        
        {
            MTLPixelFormat pixelFormat = MTLPixelFormatDepth32Float;
            MTLTextureDescriptor* desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:pixelFormat width:width height:height mipmapped: NO];
            desc.textureType = MTLTextureType2D;
            desc.sampleCount = 1;
            m_pass_desc.depthAttachment.texture = [[m_device newTextureWithDescriptor:desc] autorelease];
            
            m_pass_desc.depthAttachment.storeAction = MTLStoreActionDontCare;
            m_pipe_desc.depthAttachmentPixelFormat = m_pass_desc.depthAttachment.texture.pixelFormat;
        }
        
        {
            MTLPixelFormat pixelFormat = MTLPixelFormatStencil8;
            MTLTextureDescriptor* desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:pixelFormat width:width height:height mipmapped: NO];
            desc.textureType = MTLTextureType2D;
            desc.sampleCount = 1;
            m_pass_desc.stencilAttachment.texture = [[m_device newTextureWithDescriptor:desc] autorelease];
            
            m_pass_desc.stencilAttachment.storeAction = MTLStoreActionDontCare;
            m_pipe_desc.stencilAttachmentPixelFormat = m_pass_desc.stencilAttachment.texture.pixelFormat;
        }
        

        
		//UpdateSwapChain(width,height);

		BuildDeviceCapabilities();
	}

	bool MetalRenderDevice::UpdateSwapChain(int width, int height)
	{
        /*
		IMetalRenderTargetView* defaultRenderTargetView;

		IMetalTexture2D* defaultDepthTexture;
		IMetalDepthStencilView* defaultDepthStencilView;

		m_pSwapChain->ResizeBuffers(1, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

		// Create default rendertarget view representing the backbuffer
		IMetalTexture2D* backbufferTexture;
		m_pSwapChain->GetBuffer(0, IID_IMetalTexture2D, (void**)&backbufferTexture);
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
		Metal_TEXTURE2D_DESC depthDesc;
		memset(&depthDesc, 0, sizeof depthDesc);
		depthDesc.Width = (UINT)width;
		depthDesc.Height = (UINT)height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		//depthDesc.SampleDesc.Count = (UINT)1/*multiSample_;
		//depthDesc.SampleDesc.Quality = 1/*multiSample_ > 1 ? 0xffffffff : 0;
		depthDesc.Usage = Metal_USAGE_DEFAULT;
		depthDesc.BindFlags = Metal_BIND_DEPTH_STENCIL;
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

		m_pDefaultRenderTargetTexture = new MetalTexture();
		m_pDefaultRenderTargetTexture->SetRenderTargetView(defaultRenderTargetView);
	
		m_pDefaultDepthStencilTexture = new MetalTexture();
		m_pDefaultDepthStencilTexture->SetDepthStencilView(defaultDepthStencilView);
		m_pDefaultDepthStencilTexture->SetTexture2D(defaultDepthTexture);	

		SetViewport(Rectangle(0, 0, (float)width, (float)height));
        */

		return true;
	}
	
	bool MetalRenderDevice::TestDeviceLost()
	{
		return false;
	}

	bool MetalRenderDevice::Rest()
	{
		return true;
	}

	void MetalRenderDevice::BeginRender()
	{
        dispatch_semaphore_wait(_inflight_semaphore, DISPATCH_TIME_FOREVER);
        
        m_command_buffer = [m_command_queue commandBuffer];
        
        m_drawable = [m_layer nextDrawable];
        
        MTLRenderPassColorAttachmentDescriptor* colorAttachment = m_pass_desc.colorAttachments[0];
        colorAttachment.texture = m_drawable.texture;
        
        
        m_encoder = [m_command_buffer renderCommandEncoderWithDescriptor: m_pass_desc];
        //if (label)
        //{
        //    m_encoder.label = [[[NSString alloc] initWithBytes: label->data() length: label->size() * sizeof(wchar_t) encoding: NSUTF32LittleEndianStringEncoding] autorelease];
        //}
       // ResetStates();
        //m_clear_dirty = false;
        
        

        
        /*
		IMetalShaderResourceView* pTextures[MAX_TEXTURE_UNITS];
		for (int i = 0;i< MAX_TEXTURE_UNITS;++i)
		{
			pTextures[i] = NULL;
		}
		GetMetalDxDeviveContext()->VSSetShaderResources(0, MAX_TEXTURE_UNITS, &pTextures[0]);
		GetMetalDxDeviveContext()->PSSetShaderResources(0, MAX_TEXTURE_UNITS, &pTextures[0]);
         */
	}

	void MetalRenderDevice::EndRender()
	{
        [m_encoder endEncoding];
        
        // call the view's completion handler which is required by the view since it will signal its semaphore and set up the next buffer
        __block dispatch_semaphore_t block_sema = _inflight_semaphore;
        [m_command_buffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
            
            // GPU has completed rendering the frame and is done using the contents of any buffers previously encoded on the CPU for that frame.
            // Signal the semaphore and allow the CPU to proceed and construct the next frame.
            dispatch_semaphore_signal(block_sema);
        }];
        
        
        // schedule a present once the framebuffer is complete
        //id <CAMetalDrawable>  _currentDrawable = [m_layer nextDrawable];
        [m_command_buffer presentDrawable:m_drawable];
        
        // finalize rendering here. this will push the command buffer to the GPU
        [m_command_buffer commit];

	}

	void MetalRenderDevice::SetFrameBuffer(FrameBuffer* pFB)
	{
		for (uint32 i = 0; i < MAX_RENDERTARGETS; ++i)
		{
            MetalTexture* pMetalTexture = (MetalTexture*)(pFB->m_arrColor[i].get());
            
            if (pMetalTexture == NULL)
                continue;

            m_pass_desc.colorAttachments[i].texture = pMetalTexture->m_native;
            m_pass_desc.colorAttachments[i].resolveTexture = nil;
            m_pass_desc.colorAttachments[i].storeAction = MTLStoreActionStore;
            m_pipe_desc.colorAttachments[i].pixelFormat = pMetalTexture->m_descFormat;
            m_pass_desc.colorAttachments[i].loadAction = MTLLoadActionLoad;
		}
        
        if (pFB->m_pDepthStencil)
        {
            MetalTexture* pMetalTexture = (MetalTexture*)(pFB->m_pDepthStencil.get());
            
            m_pass_desc.depthAttachment.texture = pMetalTexture->GetNative();
            m_pass_desc.depthAttachment.storeAction = MTLStoreActionStore;
            m_pipe_desc.depthAttachmentPixelFormat = pMetalTexture->m_descFormat;
            
            m_pass_desc.depthAttachment.loadAction = MTLLoadActionLoad;
            m_pass_desc.stencilAttachment.loadAction = MTLLoadActionDontCare;
        }
	}

	void MetalRenderDevice::SetRenderTarget(int index,Texture* pTexture,int level, int array_index, int face)
	{
        /*
		MetalTexture* pMetalTexture = (MetalTexture*)(pTexture);
		if (pMetalTexture)
		{
			m_pRenderTarget[index] = pMetalTexture->GetRenderTargetView(level,array_index,face);
		
			DetachSRV(pMetalTexture->GetShaderResourceView());
		}
		else
		{
			m_pRenderTarget[index] = NULL;
		}

		m_pDeviceContext->OMSetRenderTargets(MAX_RENDERTARGETS, &m_pRenderTarget[0], m_pDepthStencil);
         */
	}


	void MetalRenderDevice::SetDepthStencil(Texture* pTexture)
	{
        /*
		MetalTexture* pMetalTexture = (MetalTexture*)(pTexture);
		m_pDepthStencil = pMetalTexture->GetDepthStencilView();

		m_pDeviceContext->OMSetRenderTargets(MAX_RENDERTARGETS, &m_pRenderTarget[0], m_pDepthStencil);
         */
	}

    
	Texture* MetalRenderDevice::GetDefaultRenderTarget(int index)
	{
        return NULL;//m_pDefaultRenderTargetTexture.get();
	}

	Texture* MetalRenderDevice::GetDefaultDepthStencil()
	{
        return NULL;//m_pDefaultDepthStencilTexture.get();
	}
     

	void MetalRenderDevice::SetViewport(const Rectangle& rect)
	{
        /*
		Metal_VIEWPORT vp;
		vp.TopLeftX = rect.left;
		vp.TopLeftY = rect.top;
		vp.Width = rect.width();
		vp.Height = rect.height();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		m_pDeviceContext->RSSetViewports(1,&vp);
         */
	}

	Rectangle MetalRenderDevice::GetViewport()
	{
		Rectangle rect;
        
        CGSize screen_size = [[UIScreen mainScreen] bounds].size;
        CGFloat scale = [UIScreen mainScreen].scale;
        CGSize resolution = CGSizeMake(screen_size.width * scale, screen_size.height * scale);
        
        NSUInteger width = resolution.width;
        NSUInteger height = resolution.height;
        
        rect.left = 0;
        rect.top = 0;
        rect.bottom = height;
        rect.right = width;
        
        /*
		UINT num = 1;

		Metal_VIEWPORT vp[Metal_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		m_pDeviceContext->RSGetViewports(&num,vp);
		rect.left = (float)vp[0].TopLeftX;
		rect.top = (float)vp[0].TopLeftY;
		rect.bottom = rect.top + (float)vp[0].Height;
		rect.right = rect.left + (float)vp[0].Width;
         */

		return rect;
	}

	void MetalRenderDevice::SetBlendState(const BlendState* pBlendState/*,const ColourValue& blend_factor, UINT32 sample_mask*/)
	{
		MetalBlendStateObject* pD3DllObject = (MetalBlendStateObject*)pBlendState;
		

        
        /*
		if (m_pCurBlendState != pD3DllObject->m_pMetalBlendState)
		{
			m_pDeviceContext->OMSetBlendState(pD3DllObject->m_pMetalBlendState, 0, M_MAX_UNSIGNED);

			m_pCurBlendState = pD3DllObject->m_pMetalBlendState;
		}
         */
	}

	void MetalRenderDevice::SetDepthStencilState(const DepthStencilState* pDSState, UINT nStencilRef)
	{
		MetalDepthStencilStateObject* pMetalDSState = (MetalDepthStencilStateObject*)pDSState;
        if (pMetalDSState && pMetalDSState->m_pMetalDSState != m_pDSState)
        {
            m_pDSState = pMetalDSState->m_pMetalDSState;
            
            [m_encoder setDepthStencilState:m_pDSState];
        }
	}
	
	void MetalRenderDevice::SetRasterizerState(const RasterizerState* pRSState)
	{
		m_pCurRSState = (MetalRasterizerStateObject*)pRSState;

        /*
		if (m_pCurRSState != pD3DllObject->m_pMetalRSState)
		{
			m_pDeviceContext->RSSetState(pD3DllObject->m_pMetalRSState);

			m_pCurRSState = pD3DllObject->m_pMetalRSState;
		}
         */
	}

	void MetalRenderDevice::SetTexture(uint32 index,Texture* pTexture,bool bSRGBNotEqual)
	{
        if (pTexture)
        {
            MetalTexture* pMetalTexure = (MetalTexture*)pTexture;
        
            [m_encoder setFragmentTexture:pMetalTexure->GetNative() atIndex:index];
        }
	}

	void MetalRenderDevice::SetTexture(Uniform* uniform,Texture* pTexture)
	{
		SetTexture(uniform->m_index,pTexture,TRUE);
	}

	void MetalRenderDevice::SetSamplerState(Uniform* uniform,SamplerState* pSampler)
	{
        SetTexture(uniform, pSampler->GetTexture());
        
        MetalSamplerStateObject* pMetalSampler = (MetalSamplerStateObject*)pSampler;
        if (pMetalSampler->m_pImpl == nil)
        {
            pMetalSampler->RT_StreamComplete();
        }
        
        [m_encoder setFragmentSamplerState:pMetalSampler->m_pImpl atIndex:uniform->m_index];
	}

	void MetalRenderDevice::CommitChanges()
	{
        /*
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
				&m_arrMetalSampler[m_nFirstDirtySamplerState]);

			m_nFirstDirtySamplerState = m_nLastDirtySamplerState = M_MAX_UNSIGNED;
			m_bSamplerStatesDirty = false;
		}
         */
        
	}

	void MetalRenderDevice::SetValue(Uniform* uniform, const float* values, UINT nSize)
	{
		ASSERT(uniform);
		ASSERT(values);

		ConstantBuffer* pConstantBuffer = (ConstantBuffer*)(uniform->m_pD3D11CBPtr);

		ASSERT(nSize <= uniform->m_nCBSize);
		pConstantBuffer->SetParameter(uniform->m_nCBOffset, nSize, values);
	}

	void MetalRenderDevice::SetValue(Uniform* uniform, int value)
	{
		SetValue(uniform,(const float*)&value,sizeof(int));
	}

	void MetalRenderDevice::SetValue(Uniform* uniform, float value)
	{
		SetValue(uniform,(const float*)&value,sizeof(float));
	}

	void MetalRenderDevice::SetValue(Uniform* uniform, const Vector2& value)
	{
		SetValue(uniform,(const float*)&value,sizeof(Vector2));
	}

	void MetalRenderDevice::SetValue(Uniform* uniform, const Vector3& value)
	{
		SetValue(uniform,(const float*)&value,sizeof(Vector3));
	}

	void MetalRenderDevice::SetValue(Uniform* uniform, const Vector4* values, UINT count)
	{
		SetValue(uniform,(const float*)values,sizeof(Vector4) * count);
	}

	void MetalRenderDevice::SetValue(Uniform* uniform, const Matrix4* values, UINT count)
	{
		SetValue(uniform,(const float*)values,sizeof(Matrix4) * count);
	}

	void MetalRenderDevice::SetValue(Uniform* uniform, const ColourValue& value)
	{
		SetValue(uniform,(const float*)&value,12);
	}

	void MetalRenderDevice::SetVertexDeclaration(const VertexDeclaration* pDec)
	{
        /*
		MetalVertexDeclaration* pMetalDec = (MetalVertexDeclaration*)pDec;

		if (m_pCurInput != pMetalDec->m_pImpl)
		{
			m_pDeviceContext->IASetInputLayout(pMetalDec->m_pImpl);

			m_pCurInput = pMetalDec->m_pImpl;
		}
         */
	}

	void MetalRenderDevice::SetIndexBuffer(IndexBuffer* pIB)
	{
        //if ([arg.name isEqualToString:@"vertices"])
        {
            //[ m_encoder setVertexBuffer:pIB->_im.vertex_buffer offset:0 atIndex:arg.index];
        }
        
        /*
		MetalIndexBuffer* buffer = (MetalIndexBuffer*)pIB;
		if (buffer != m_pIndexBuffer)
		{
			if (buffer)
				m_pDeviceContext->IASetIndexBuffer(buffer->GetD3DIndexBuffer(),
				buffer->GetStride() == sizeof(unsigned short) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
			else
				m_pDeviceContext->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);

			m_pIndexBuffer = buffer;
		}
         */
	}

	void MetalRenderDevice::SetVertexBuffer(int index, VertexBuffer* pVB)
	{
        MetalVertexBuffer* pMetalVertexBuffer = (MetalVertexBuffer*)(pVB);
        id<MTLBuffer> vb = pMetalVertexBuffer->GetMetalVertexBuffer();
        
        [m_encoder setVertexBuffer:vb offset:0 atIndex:index + 2];
        
        /*
		MetalVertexBuffer* pMetalVertexBuffer = (MetalVertexBuffer*)pVB;
		IMetalBuffer* pBuffer = pMetalVertexBuffer->GetD3DVertexBuffer();
		UINT nStride = pMetalVertexBuffer->GetStride();
		UINT offset = 0; // no stream offset, this is handled in _render instead
		m_pDeviceContext->IASetVertexBuffers(index, 1, &pBuffer, &nStride, &offset);
         */
	}

	void MetalRenderDevice::DrawRenderable(const Renderable* pRenderable,Technique* pTech)
	{
        
		if (pRenderable == NULL)
			return;
        
		CommitChanges();	

		MTLPrimitiveType ePrimitiveType = MetalMapping::GetPrimitiveType(pRenderable->m_ePrimitiveType);

		const RefPtr<SubMeshData>& pSubMeshData = pRenderable->m_pSubMeshData;

		UINT nIndexCount = pSubMeshData ? pSubMeshData->m_nIndexCount : pRenderable->m_pIndexBuffer->GetNumber();
		//UINT nIndexStart = pSubMeshData ? pSubMeshData->m_nIndexStart : 0;
		
		//UINT nVertexCount = pSubMeshData ? pSubMeshData->m_nVertexCount : pRenderable->m_pVertexBuffer->GetNumber();
		//UINT nVertexStart = pSubMeshData ? pSubMeshData->m_nVertexStart : 0;
        
        MetalIndexBuffer* pMetalIndexBuffer = (MetalIndexBuffer*)(pRenderable->m_pIndexBuffer.get());
        id<MTLBuffer> ib = pMetalIndexBuffer->GetMetalIndexBuffer();
        
        [m_encoder drawIndexedPrimitives:ePrimitiveType indexCount:nIndexCount indexType:MTLIndexTypeUInt16 indexBuffer:ib indexBufferOffset:0];
    
	}

	void MetalRenderDevice::ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s)
	{
        /*
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
				depthClearFlags |= Metal_CLEAR_DEPTH;
			if (bStencil)
				depthClearFlags |= Metal_CLEAR_STENCIL;
			m_pDeviceContext->ClearDepthStencilView(m_pDepthStencil, depthClearFlags, z, (uint8)s);
		}

		ASSERT(hr == S_OK && "Clear buffer failed.");
         */
	}

	Matrix4 MetalRenderDevice::MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf)
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

	Matrix4 MetalRenderDevice::MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf)
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

	Matrix4 MetalRenderDevice::MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf)
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

	void MetalRenderDevice::BeginProfile(const char* pszLale)
	{
// 		wchar_t buf[128]; 
// 		size_t outCount=0; 
// 		mbstowcs_s(&outCount, buf, pszLale, _countof(buf)-1); 
// 		D3DPERF_BeginEvent(0xff00ff00, buf); 
	}

	void MetalRenderDevice::EndProfile()
	{
		//D3DPERF_EndEvent();
	}

	bool MetalRenderDevice::CheckTextureFormat(PixelFormat eFormat,TEXTURE_USAGE eUsage)
	{
// 		DWORD D3DUsage =  MetalMapping::GetD3DTextureUsage(eUsage); 
// 		D3DFORMAT D3DFormat = MetalMapping::GetD3DFormat(eFormat);
// 
// 		HRESULT hr = D3DXCheckTextureRequirements(m_pD3DDevice, NULL, NULL, NULL, D3DUsage, &D3DFormat, D3DPOOL_DEFAULT);
// 		return hr == D3D_OK;

		return false;
	}

	void MetalRenderDevice::NotifyResourceCreated(MetalResource* pResource)
	{
	}

	void MetalRenderDevice::NotifyResourceDestroyed(MetalResource* pResource)
	{
	}

	bool MetalRenderDevice::BuildDeviceCapabilities()
	{
		GetDeviceCapabilities()->SetShadowMapColorFormat(PF_A8R8G8B8);
		GetDeviceCapabilities()->SetShadowMapDepthFormat(PF_D24S8);
	
		GetDeviceCapabilities()->log();
		return true;
	}
}



