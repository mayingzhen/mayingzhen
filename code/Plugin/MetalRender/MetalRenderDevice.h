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
		virtual Texture*			CreateRenderTarget(int nWidth,int nHeight,UINT nMipMap,PixelFormat format,bool bSRGB,TEXTURE_TYPE eType);
		virtual Texture*			CreateDepthStencil(int nWidth,int nHeight,PixelFormat format,bool bTypeLess);
		virtual VertexDeclaration*	CreateVertexDeclaration();
		virtual VertexBuffer*		CreateVertexBuffer();
		virtual IndexBuffer*		CreateIndexBuffer();
		virtual ShaderProgram*		CreateShaderProgram();

		virtual BlendState*			CreateBlendState();
		virtual DepthStencilState*	CreateDepthStencilState();
		virtual RasterizerState*	CreateRasterizerState();

		virtual SamplerState*		CreateSamplerState();

		virtual	void				SetFrameBuffer(FrameBuffer* pFB);
		virtual	void				SetRenderTarget(int index,Texture* pTexture,int level = 0, int array_index = 0, int face = 0);
		virtual	Texture*			GetDefaultRenderTarget(int index = 0);
		virtual void				SetDepthStencil(Texture* pTexture);
		virtual Texture*			GetDefaultDepthStencil();
		virtual void				SetViewport(const Rectangle& rect);
		virtual Rectangle			GetViewport();

		virtual void				SetBlendState(const BlendState* pBlendState);
		virtual void				SetDepthStencilState(const DepthStencilState* pDSState,UINT nStencilRef);
		virtual void				SetRasterizerState(const RasterizerState* pRSState);
		
		virtual void				SetValue(Uniform* uniform, int value);
		virtual void				SetValue(Uniform* uniform, float value);
		virtual void				SetValue(Uniform* uniform, const Vector2& value);
		virtual void				SetValue(Uniform* uniform, const Vector3& value);
		virtual void				SetValue(Uniform* uniform, const Vector4* values, UINT count);
		virtual void				SetValue(Uniform* uniform, const Matrix4* values, UINT count);
		virtual void				SetValue(Uniform* uniform, const ColourValue& value);
		virtual	void				SetTexture(Uniform* uniform,Texture* pTexture);
		virtual	void				SetTexture(uint32 nIndex,Texture* pTexture,bool bSRGBNotEqual);
		virtual void				SetSamplerState(Uniform* uniform,SamplerState* pTexture);

		virtual	void				SetVertexDeclaration(const VertexDeclaration* pDec);
		virtual void				SetIndexBuffer(IndexBuffer* pIB);
		virtual	void				SetVertexBuffer(int index, VertexBuffer* pVB);

		virtual void				DrawRenderable(const Renderable* pRenderable,Technique* pTech);

		virtual	void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s);

		virtual	void				Init(HWND wndhandle);
		virtual void				Shoutdown();
		virtual bool				TestDeviceLost();
		virtual	bool				Rest();
		virtual void				BeginRender();
		virtual void				EndRender();
		
		virtual	Matrix4				MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf);
		virtual	Matrix4				MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf);
		virtual Matrix4				MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf);

		virtual	void				BeginProfile(const char* pszLale);
		virtual	void				EndProfile();

		// Help fun
		virtual	bool				CheckTextureFormat(PixelFormat eFormat,TEXTURE_USAGE eUsage);

		id<MTLDevice>				GetMetalDevive() {return m_device;}
        id<MTLRenderCommandEncoder> GetRenderCommandEncoder() {return m_encoder;}

		void						NotifyResourceCreated(MetalResource* pRes);

		void						NotifyResourceDestroyed(MetalResource* pRes);

		void						ClearAllStates();

	private:
		void						SetValue(Uniform* uniform, const float* values, UINT count);

		void						CommitChanges();
		
		bool						BuildDeviceCapabilities();

		bool						UpdateSwapChain(int width, int height);

		//void						DetachSRV(IMetalShaderResourceView* rtv_src);
	
	private:
        id<MTLDevice>               m_device;
        id<MTLCommandQueue>         m_command_queue;
        dispatch_semaphore_t        _inflight_semaphore;
        //MetalCommandBuffer          m_command_buffer;
        id<MTLCommandBuffer>           m_command_buffer;
        
        MTLRenderPassDescriptor*    m_pass_desc;
        MTLRenderPipelineDescriptor* m_pipe_desc;
        id<MTLRenderCommandEncoder> m_encoder;
  
        
        MetalBlendStateObject*      m_pCurBlendState;
        MetalRasterizerStateObject* m_pCurRSState;
        id<MTLDepthStencilState>    m_pDSState;

        CAMetalLayer*               m_layer;
        id<CAMetalDrawable>         m_drawable;
        
	};

	id<MTLDevice> GetMetalDevive();
	//IMetalDeviceContext* GetMetalDeviveContext();
}


