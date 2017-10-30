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
#include "MetalRenderPass.h"
#include "MetalTechniqueh.h"
#include "MetalRenderCommand.h"

#import <UIKit/UIkit.h>
#import <UIKit/UIDevice.h>
#import <cmath>

namespace ma
{

	id<MTLDevice> GetMetalDevive()
	{
		MetalRenderDevice* pRender = (MetalRenderDevice*)GetRenderDevice();
		ASSERT(pRender);
		return pRender->GetMetalDevive();
	}

	MetalRenderDevice::MetalRenderDevice()
	{
		ClearAllStates();
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
    
    ConstantBuffer*	MetalRenderDevice::CreateConstantBuffer(UINT nSize)
    {
        MetalConstantBuffer* pConstBuffer = new MetalConstantBuffer();
        pConstBuffer->SetSize(nSize);
        return pConstBuffer;
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
    
    Technique* MetalRenderDevice::CreateTechnique()
    {
        return new MetalTechnique();
    }
    
    RenderPass*	MetalRenderDevice::CreateRenderPass()
    {
        return new MetalRenderPass();
    }
    
    RenderCommand* MetalRenderDevice::CreateRenderCommand()
    {
        return new MetalRenderCommand();
    }

	void MetalRenderDevice::Shoutdown()
	{
		//m_hWnd = NULL;

		MetalRasterizerStateObject::Clear();
		MetalDepthStencilStateObject::Clear();
		MetalBlendStateObject::Clear();
		MetalVertexDeclaration::Clear();
		MetalSamplerStateObject::Clear();
		MetalConstantBuffer::Clear();

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
        m_layer.pixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
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
        
        MTLRenderPassDescriptor* pass_desc = [[MTLRenderPassDescriptor renderPassDescriptor] retain];
 
        id<MTLTexture> colorTex = m_drawable.texture;
        pass_desc.colorAttachments[0].texture = colorTex;
        pass_desc.colorAttachments[0].storeAction = MTLStoreActionStore;
        
        CGSize screen_size = [[UIScreen mainScreen] bounds].size;
        CGFloat scale = [UIScreen mainScreen].scale;
        CGSize resolution = CGSizeMake(screen_size.width * scale, screen_size.height * scale);
        
        NSUInteger width = resolution.width;
        NSUInteger height = resolution.height;
        
        {
            MTLPixelFormat pixelFormat = MTLPixelFormatDepth32Float;
            MTLTextureDescriptor* desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:pixelFormat width:width height:height mipmapped: NO];
            desc.usage = MTLTextureUsageShaderRead | MTLTextureUsageRenderTarget;
            desc.textureType = MTLTextureType2D;
            desc.sampleCount = 1;
            pass_desc.depthAttachment.texture = [[m_device newTextureWithDescriptor:desc] autorelease];
            
            pass_desc.depthAttachment.storeAction = MTLStoreActionDontCare;
        }
        
        if (0)
        {
            MTLPixelFormat pixelFormat = MTLPixelFormatStencil8;
            MTLTextureDescriptor* desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:pixelFormat width:width height:height mipmapped: NO];
            desc.textureType = MTLTextureType2D;
            desc.sampleCount = 1;
            pass_desc.stencilAttachment.texture = [[m_device newTextureWithDescriptor:desc] autorelease];
            
            pass_desc.stencilAttachment.storeAction = MTLStoreActionDontCare;
        }
        
        m_pDefaultRenderPass = new MetalRenderPass();
        
        MetalTexture* pCo = new MetalTexture();
        pCo->m_descFormat = MTLPixelFormatBGRA8Unorm_sRGB;
        pCo->m_native = pass_desc.colorAttachments[0].texture;
        m_pDefaultRenderPass->m_arrColor[0] = pCo;
        
        MetalTexture* pDs = new MetalTexture();
        pDs->m_descFormat = MTLPixelFormatDepth32Float;
        pDs->m_native = pass_desc.depthAttachment.texture;
        m_pDefaultRenderPass->m_pDepthStencil = pDs;
        
        m_pDefaultRenderPass->m_pass_desc = pass_desc;
        
		//UpdateSwapChain(width,height);

		BuildDeviceCapabilities();
	}

	bool MetalRenderDevice::UpdateSwapChain(int width, int height)
	{
		return true;
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
        
        m_pDefaultRenderPass->m_pass_desc.colorAttachments[0].texture = m_drawable.texture;
        
        //m_encoder = [m_command_buffer renderCommandEncoderWithDescriptor: m_pass_desc];
        
        // create and initialize render pass descriptor
        // create a parallel render command encoder
        //id <MTLParallelRenderCommandEncoder> parallelRCE =
        //[m_command_buffer parallelRenderCommandEncoderWithDescriptor:renderPassDesc];
	}

	void MetalRenderDevice::EndRender()
	{
        //[m_encoder endEncoding];
        
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
        
        
        MetalConstantBuffer::OnEndFrame();

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
    
    RenderPass* MetalRenderDevice::GetDefaultRenderPass()
    {
        return m_pDefaultRenderPass.get();
    }
    
	Matrix4 MetalRenderDevice::MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf)
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
		GetDeviceCapabilities()->SetShadowMapDepthFormat(PF_D16F);
        GetDeviceCapabilities()->SetTextureDXTSupported(false);
	
		GetDeviceCapabilities()->log();
		return true;
	}
}



