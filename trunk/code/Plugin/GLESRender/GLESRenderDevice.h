#ifndef  _GLESRENDERDevice__H__
#define  _GLESRENDERDevice__H__

namespace ma
{
	class IndexBuffer;
	class VertexBuffer;
	class GLESRenderTarget;
	class GLESTechnique;

	class  GLESRenderDevice : public IRenderDevice
	{
	public:
		GLESRenderDevice();

		~GLESRenderDevice();

		virtual Texture*			CreateTexture(const char* pszPath = NULL);

		virtual Texture*			CreateTexture(int nWidth,int nHeight,FORMAT format);

		virtual VertexDeclaration*	CreateVertexDeclaration();

		virtual VertexBuffer*		CreateVertexBuffer(void* pData = NULL, int nsize = 0, int nStride = 0, USAGE Usgae = USAGE_STATIC);

		virtual IndexBuffer*		CreateIndexBuffer(void* Data = NULL, int size = 0, INDEX_TYPE eIndexType = INDEX_TYPE_U16, USAGE Usgae = USAGE_STATIC);

		virtual	ShaderProgram*		CreateShaderProgram(const char* pszName,const char* pszDefine);

		virtual const char*			GetShaderPath();

		virtual void				ConvertUV(float& fTop,float& fLeft,float& fRight,float& fBottom);

		virtual float				GetHalfPixelOffset(float fHalfPiexl);
		
		//// Render
		virtual RenderTarget*		CreateRenderTarget(int nWidth = -1,int nHeight = -1,FORMAT format = FMT_A8R8G8B8);

		virtual	void				PushRenderTarget(RenderTarget* pTexture,int index = 0);

		virtual void				PopRenderTarget(int index = 0);

		virtual void				PushDepthStencil(RenderTarget* pTexture);

		virtual void				PopDepthStencil();

		virtual void				PushViewport(const Rectangle& rect);

		virtual void				PopViewport();

		virtual void				SetRenderState(const RenderState& state);

		virtual void				DrawRenderable(const Renderable* pRenderable);

		virtual void				DrawDyRenderable(const Renderable* pRenderable);

		virtual	void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s);
		

		/// Render Pass
		virtual	void				Init(HWND wndhandle);

		virtual void				BeginRender();
	
		virtual void				EndRender();
		
		virtual	Matrix4x4			MakePerspectiveMatrix(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf);

		virtual	Matrix4x4			MakeOrthoMatrix(Matrix4x4 *pOut, float width, float height, float zn, float zf);

		virtual Matrix4x4			MakeOrthoMatrixOffCenter(Matrix4x4 *pOut, float left, float right, float bottom, float top, float zn, float zf);

		FrameBufferHandle			GetOffecreenFrameBuffer() {return m_hOffecreenFrameBuffer;}

	private:
		FrameBufferHandle				m_hDefaultFrameBuffer;
		FrameBufferHandle				m_hOffecreenFrameBuffer;	

		std::stack<GLESRenderTarget*> 	m_pRenderTarget;
		std::stack<Rectangle>			m_viewport;


#if PLATFORM_WIN == 1
		HDC		m_hDC;
		HGLRC	m_hGLRC;
#endif

	};
}

#endif


