#ifndef  _GLESRENDERDevice__H__
#define  _GLESRENDERDevice__H__

namespace ma
{
	class IndexBuffer;
	class VertexBuffer;
	class GLESTechnique;
	class GLESDeviceContext;

	class GLESRenderDevice : public IRenderDevice
	{
	public:
		GLESRenderDevice();

		~GLESRenderDevice();

		virtual RenderDeviceType	GetRenderDeviceType() {return RenderDevice_GLES2;}

		virtual Texture*			CreateTexture();
		virtual Texture*			CreateRenderTarget(int nWidth,int nHeight,PixelFormat format,bool bSRGB);
		virtual Texture*			CreateDepthStencil(int nWidth,int nHeight,PixelFormat format,bool bTypeLess);
		virtual VertexDeclaration*	CreateVertexDeclaration();
		virtual VertexBuffer*		CreateVertexBuffer();
		virtual IndexBuffer*		CreateIndexBuffer();
		virtual	ShaderProgram*		CreateShaderProgram();
		
		//// Render
		virtual void				SetFrameBuffer(FrameBuffer* pFB);
		virtual	void				SetRenderTarget(Texture* pTexture,int index = 0);
		virtual	Texture*			GetDefaultRenderTarget(int index = 0);
		virtual void				SetDepthStencil(Texture* pTexture);
		virtual Texture*			GetDefaultDepthStencil();
		virtual void				SetViewport(const Rectangle& rect);
		virtual Rectangle			GetViewport();

		virtual	void				SetDepthBias(float constantBias, float slopeScaleBias = 0.0f);
		virtual	void				SetCullingMode(CULL_MODE mode);
		virtual void				SetBlendMode(BLEND_MODE mode);
		virtual	void				SetDepthCheckMode(CompareFunction mode);
		virtual	void				SetDepthWrite(bool b);
		virtual	void				SetColorWrite(bool b);
		virtual void				SetSRGBWrite(bool b);
		virtual void				SetStencilEnable(bool b);
		virtual void				SetStencilBufferParams(CompareFunction func = CMPF_ALWAYS_PASS, 
			uint32 refValue = 0, uint32 mask = 0xFFFFFFFF, uint32 writeMask = 0xFFFFffff,
			StencilOperation stencilFailOp = SOP_KEEP, 
			StencilOperation depthFailOp = SOP_KEEP,
			StencilOperation passOp = SOP_KEEP, 
			bool twoSidedOperation = false);

		virtual	void				SetTexture(uint32 nIndex,Texture* pTexture,bool bSRGBNotEqual);
 		virtual	void				SetTexture(Uniform* uniform,Texture* pTexture);
		virtual void				SetSamplerState(Uniform* uniform,SamplerState* pSampler);
		
		virtual void				SetValue(Uniform* uniform, float value);
		virtual void				SetValue(Uniform* uniform, const Vector2& value);
		virtual void				SetValue(Uniform* uniform, const Vector3& value);
		virtual void				SetValue(Uniform* uniform, const Vector4* values, UINT count);
		virtual void				SetValue(Uniform* uniform, const Matrix4* values, UINT count);
		virtual void				SetValue(Uniform* uniform, const ColourValue& value);
		
		virtual	void				SetVertexDeclaration(VertexDeclaration* pDec);
		virtual void				SetIndexBuffer(IndexBuffer* pIB);
		virtual	void				SetVertexBuffer(int index, VertexBuffer* pVB);

		virtual void				DrawRenderable(const Renderable* pRenderable,Technique* pTech);
		virtual void				DrawDyRenderable(const Renderable* pRenderable,Technique* pTech);

		virtual	void				Init(HWND wndhandle);
		virtual void 				Shoutdown();
		virtual bool				TestDeviceLost() {return false;}
		virtual void				BeginRender();
		virtual void				EndRender();
		virtual	void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s);

		virtual	Matrix4				MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf);
		virtual	Matrix4				MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf);
		virtual Matrix4				MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf);
	
		virtual	void				BeginProfile(const char* pszLale);
		virtual	void				EndProfile();

		virtual	bool				CheckTextureFormat(PixelFormat eFormat,TEXTURE_USAGE eUsage);

		FrameBufferHandle			GetOffecreenFrameBuffer() {return m_hOffecreenFrameBuffer;}

		void						SetStencilMaskGL(GLuint mask);

	private:
		void						BuildDeviceCapabilities();

	private:
		//FrameBufferHandle			m_hDefaultFrameBuffer;
		FrameBufferHandle			m_hOffecreenFrameBuffer;	

		GLESDeviceContext*			m_pDeviceContext;	

		bool mStencilEnabledGL;
		GLuint mStencilMaskGL;
	};
}

#endif


