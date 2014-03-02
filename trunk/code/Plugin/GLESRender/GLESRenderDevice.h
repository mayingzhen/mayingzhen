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

		virtual Texture*			CreateTexture(const char* pszPath = NULL);

		virtual Texture*			CreateTexture(int nWidth,int nHeight,FORMAT format = FMT_A8R8G8B8,bool bDepthStencil = false);

		virtual VertexDeclaration*	CreateVertexDeclaration();

		virtual VertexBuffer*		CreateVertexBuffer();

		virtual IndexBuffer*		CreateIndexBuffer();

		virtual	ShaderProgram*		CreateShaderProgram(Technique* pTech,const char* pVSFile,const char* pPSFile,const char* pszDefine);

		virtual const char*			GetShaderPath();

		virtual void				ConvertUV(float& fTop,float& fLeft,float& fRight,float& fBottom);

		virtual float				GetHalfPixelOffset(float fHalfPiexl);
		
		//// Render
		virtual	void				SetRenderTarget(Texture* pTexture,int index = 0);

		virtual	Texture*			GetRenderTarget(int index = 0);

		virtual void				SetDepthStencil(Texture* pTexture);

		virtual Texture*			GetDepthStencil();

		virtual void				SetViewport(const Rectangle& rect);

		virtual Rectangle			GetViewport();

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
		//FrameBufferHandle				m_hDefaultFrameBuffer;
		FrameBufferHandle				m_hOffecreenFrameBuffer;	

		GLESDeviceContext*				m_pDeviceContext;	
	};
}

#endif


