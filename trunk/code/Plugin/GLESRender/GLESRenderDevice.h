#ifndef  _GLESRENDERDevice__H__
#define  _GLESRENDERDevice__H__

namespace ma
{
	class IndexBuffer;
	class VertexBuffer;
	class GLESTechnique;

	class  GLESRenderDevice : public IRenderDevice
	{
	public:
		GLESRenderDevice();

		~GLESRenderDevice();

		virtual Texture*			CreateTexture(const char* pszPath = NULL);

		virtual Texture*			CreateTexture(int nWidth,int nHeight,FORMAT format,bool bDepthStencil);

		virtual VertexDeclaration*	CreateVertexDeclaration();

		virtual VertexBuffer*		CreateVertexBuffer(void* pData = NULL, int nsize = 0, int nStride = 0, USAGE Usgae = USAGE_NO);

		virtual IndexBuffer*		CreateIndexBuffer(void* Data = NULL, int size = 0, INDEX_TYPE eIndexType = INDEX_TYPE_U16, USAGE Usgae = USAGE_NO);

		virtual	ShaderProgram*		CreateShaderProgram(Technique* pTech,const char* pszName,const char* pszDefine);

		virtual const char*			GetShaderPath();

		virtual void				ConvertUV(float& fTop,float& fLeft,float& fRight,float& fBottom);

		virtual float				GetHalfPixelOffset(float fHalfPiexl);
		
		//// Render
		virtual	void				PushRenderTarget(Texture* pTexture,int index = 0);

		virtual void				PopRenderTarget(int index = 0);

		virtual void				PushDepthStencil(Texture* pTexture);

		virtual void				PopDepthStencil();

		virtual void				PushViewport(const Rectangle& rect);

		virtual void				PopViewport();

		virtual void				SetRenderState(const RenderState& state);

		virtual void				DrawRenderable(const Renderable* pRenderable,Technique* pTech);

		virtual void				DrawDyRenderable(const Renderable* pRenderable,Technique* pTech);

		virtual	void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s);
		

		/// Render Pass
		virtual	void				Init(HWND wndhandle);

		virtual void				BeginRender();
	
		virtual void				EndRender();
		
		virtual	Matrix4x4			MakePerspectiveMatrix(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf);

		virtual	Matrix4x4			MakeOrthoMatrix(Matrix4x4 *pOut, float width, float height, float zn, float zf);

		virtual Matrix4x4			MakeOrthoMatrixOffCenter(Matrix4x4 *pOut, float left, float right, float bottom, float top, float zn, float zf);

		virtual	void				BeginProfile(const char* pszLale);

		virtual	void				EndProfile();

		virtual	bool				CheckTextureFormat(FORMAT eFormat,USAGE eUsage);

		FrameBufferHandle			GetOffecreenFrameBuffer() {return m_hOffecreenFrameBuffer;}

	private:
		FrameBufferHandle				m_hDefaultFrameBuffer;
		FrameBufferHandle				m_hOffecreenFrameBuffer;	

		std::stack<GLESTexture*> 		m_pRenderTarget;
		std::stack<Rectangle>			m_viewport;


#if PLATFORM_WIN == 1
		HDC		m_hDC;
		HGLRC	m_hGLRC;
#endif

	};
}

#endif


