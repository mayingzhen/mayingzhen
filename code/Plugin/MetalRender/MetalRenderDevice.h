#pragma once

#import <QuartzCore/CAMetalLayer.h>
#include <dispatch/semaphore.h>
#include <atomic>

namespace ma
{
	class IndexBuffer;
	class VertexBuffer;
	class ConstantBuffer;
	class MetalResource;
	class MetalShaderProgram;
	class MetalVertexDeclaration;
    class MetalBlendStateObject;
    class MetalRasterizerStateObject;
    class MetalRenderPass;
    
    /* metal fence is used to sync CPU with GPU,
     * MetalCommandBuffer creates a fence at the start of each frame, and signal this fence when GPU complete this command buffer
     * resouce can wait this fence to make sure GPU finished using this resource
     */
    struct MetalFence
    {
        MetalFence() :  mRefCount(0), mSignal(false)
        {
        }
        
    public:
        void	Aquire();
        void	Release();
        
        void	Wait();
        void	Signal();
        
        std::mutex				mLock;
        std::condition_variable	mCompleteCondVar;
        std::atomic<int>		mRefCount;
        std::atomic<bool>		mSignal;
    };
    
    struct MetalCommandBuffer
    {
        MetalCommandBuffer() : mBuffer(nil), mCompleteFence(nil)
        {}
        
        ~MetalCommandBuffer() {}
        
        id<MTLCommandBuffer>	mBuffer;
        MetalFence*				mCompleteFence;
        
        void BeginEncodeCommand();
        void EndEncodeCommand();
    };

	class MetalRenderDevice : public IRenderDevice
	{
	public:
		MetalRenderDevice();

		~MetalRenderDevice();

		virtual RenderDeviceType	GetRenderDeviceType() {return RenderDevice_METAL;}

		virtual Texture*			CreateTexture();
		virtual Texture*			CreateRenderTarget(int nWidth,int nHeight,uint32_t nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType);
		virtual Texture*			CreateDepthStencil(int nWidth,int nHeight,PixelFormat format);
		virtual VertexDeclaration*	CreateVertexDeclaration();
		virtual VertexBuffer*		CreateVertexBuffer();
		virtual IndexBuffer*		CreateIndexBuffer();
		virtual ShaderProgram*		CreateShaderProgram();
        virtual ConstantBuffer*		CreateConstantBuffer(uint32_t nSize);
		virtual BlendState*			CreateBlendState();
		virtual DepthStencilState*	CreateDepthStencilState();
		virtual RasterizerState*	CreateRasterizerState();
		virtual SamplerState*		CreateSamplerState();
        virtual Technique*			CreateTechnique();
        virtual RenderPass*			CreateRenderPass();
        virtual RenderCommand*		CreateRenderCommand();
        virtual ComputeCommad*      CreateComputeCommand();

		virtual Rectangle			GetViewport();
        virtual RenderPass*			GetDefaultRenderPass();


		virtual	void				Init(void* wndhandle);
		virtual void				Shoutdown();
		virtual	bool				Rest();
		virtual void				BeginRender();
		virtual void				EndRender();
        
        virtual void                BegineCompute();
        virtual void                EndCompute();
		
		virtual	Matrix4				MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf);
		virtual	Matrix4				MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf);
		virtual Matrix4				MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf);

		virtual	void				BeginProfile(const char* pszLale);
		virtual	void				EndProfile();

		// Help fun
		virtual	bool				CheckTextureFormat(PixelFormat eFormat,TEXTURE_USAGE eUsage);

		id<MTLDevice>				GetMetalDevive() {return m_device;}

		void						NotifyResourceCreated(MetalResource* pRes);

		void						NotifyResourceDestroyed(MetalResource* pRes);

		void						ClearAllStates();

	private:
		void						SetValue(Uniform* uniform, const float* values, uint32_t count);
		
		bool						BuildDeviceCapabilities();

		bool						UpdateSwapChain(int width, int height);
	
	public:
        id<MTLDevice>               m_device;
        id<MTLCommandQueue>         m_command_queue;
        dispatch_semaphore_t        _inflight_semaphore;
        id<MTLCommandBuffer>           m_command_buffer;
        
        RefPtr<MetalRenderPass>     m_pDefaultRenderPass;
        

        CAMetalLayer*               m_layer;
        id<CAMetalDrawable>         m_drawable;
        
	};

	id<MTLDevice> GetMetalDevive();
}


