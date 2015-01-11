#ifndef  _GLESRENDERDevice__H__
#define  _GLESRENDERDevice__H__

namespace ma
{
	class IndexBuffer;
	class VertexBuffer;
	class GLESTechnique;
	class GLESDeviceContext;

	class  GLESRenderDevice : public IRenderDevice
	{
	public:
		GLESRenderDevice();

		~GLESRenderDevice();

		virtual RenderDeviceType	GetRenderDeviceType() {return RenderDevice_GLES2;}

		virtual Texture*			CreateTexture(const char* pszPath = NULL);
		virtual Texture*			CreateTexture(int nWidth,int nHeight,PixelFormat format = PF_A8B8G8R8,USAGE eUsage = USAGE_STATIC);
		virtual VertexDeclaration*	CreateVertexDeclaration();
		virtual VertexBuffer*		CreateVertexBuffer();
		virtual IndexBuffer*		CreateIndexBuffer();
		virtual	ShaderProgram*		CreateShaderProgram();
		
		//// Render
		virtual	void				SetRenderTarget(Texture* pTexture,int index = 0);
		virtual	Texture*			GetRenderTarget(int index = 0);
		virtual void				SetDepthStencil(Texture* pTexture);
		virtual Texture*			GetDepthStencil();
		virtual void				SetViewport(const Rectangle& rect);
		virtual Rectangle			GetViewport();

		virtual	void				SetDepthBias(float constantBias, float slopeScaleBias = 0.0f);
		virtual	void				SetCullingMode(CULL_MODE mode);
		virtual void				SetBlendMode(BLEND_MODE mode);
		virtual	void				SetDepthCheckMode(DEPTH_CHECK_MODE mode);
		virtual	void				SetDepthWrite(bool b);
		virtual	void				SetColorWrite(bool b);

		virtual	void				SetTexture(Uniform* uniform,Texture* pTexture);
		virtual	void				SetTextureWrap(int index,Wrap eWrap);
		virtual void				SetTextureFilter(int index,FilterOptions eFilter);
		
		virtual void				SetValue(Uniform* uniform, float value);
		virtual void				SetValue(Uniform* uniform, const Matrix4& value);
		virtual void				SetValue(Uniform* uniform, const Vector2& value);
		virtual void				SetValue(Uniform* uniform, const Vector3& value);
		virtual void				SetValue(Uniform* uniform, const Vector4& value);
		virtual void				SetValue(Uniform* uniform, const Vector4* values, UINT count = 1);
		virtual void				SetValue(Uniform* uniform, const ColourValue& value);
		
		virtual	void				SetVertexDeclaration(VertexDeclaration* pDec);
		virtual void				SetIndexBuffer(IndexBuffer* pIB);
		virtual	void				SetVertexBuffer(int index, VertexBuffer* pVB);

		virtual void				DrawRenderable(const Renderable* pRenderable,Technique* pTech);
		virtual void				DrawDyRenderable(const Renderable* pRenderable,Technique* pTech);

		virtual	void				Init(HWND wndhandle);
		virtual void 				ShutDown();
		virtual bool				TestDeviceLost() {return false;}
		virtual void				BeginRender();
		virtual void				EndRender();
		virtual	void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s);

		virtual void				ConvertUV(float& fTop,float& fLeft,float& fRight,float& fBottom);
		virtual float				GetHalfPixelOffset(float fHalfPiexl);
		virtual	Matrix4				MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf);
		virtual	Matrix4				MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf);
		virtual Matrix4				MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf);
	
		virtual	void				BeginProfile(const char* pszLale);
		virtual	void				EndProfile();

		virtual	bool				CheckTextureFormat(PixelFormat eFormat,USAGE eUsage);

		FrameBufferHandle			GetOffecreenFrameBuffer() {return m_hOffecreenFrameBuffer;}

	private:
		//FrameBufferHandle			m_hDefaultFrameBuffer;
		FrameBufferHandle			m_hOffecreenFrameBuffer;	

		GLESDeviceContext*			m_pDeviceContext;	
	};
}

#endif


