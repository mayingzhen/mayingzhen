#ifndef  _D3D9RENDERDevice__H__
#define  _D3D9RENDERDevice__H__

namespace ma
{
	class IndexBuffer;
	class VertexBuffer;

	class D3D9RenderDevice : public IRenderDevice
	{
	public:
		D3D9RenderDevice();

		~D3D9RenderDevice();

		/// Vido Buffer 
		virtual Texture*			CreateTexture(const char* pszPath = NULL);

		virtual Texture*			CreateTexture(int nWidth,int nHeight,FORMAT format = FMT_A8R8G8B8,bool bDepthStencil = false);

		virtual VertexDeclaration*	CreateVertexDeclaration();

		virtual VertexBuffer*		CreateVertexBuffer();

		virtual IndexBuffer*		CreateIndexBuffer();

		virtual ShaderProgram*		CreateShaderProgram(Technique* pTech,const char* pVSFile,const char* pPSFile,const char* pszDefine);

		virtual const char*			GetShaderPath();

		virtual void				ConvertUV(float& fTop,float& fLeft,float& fRight,float& fBottom);

		virtual float				GetHalfPixelOffset(float fHalfPiexl);

		virtual	void				SetRenderTarget(Texture* pTexture,int index = 0);

		virtual	Texture*			GetRenderTarget(int index = 0);

		virtual void				SetDepthStencil(Texture* pTexture);

		virtual Texture*			GetDepthStencil();

		virtual void				SetViewport(const Rectangle& rect);

		virtual Rectangle			GetViewport();

		virtual void				SetRenderState(const RenderState& state);

		virtual void				DrawRenderable(const Renderable* pRenderable,Technique* pTech);

		virtual void				DrawDyRenderable(const Renderable* pRenderable,Technique* pTech);

		virtual	void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const ColourValue & c, float z, int s);

		virtual	void				Init(HWND wndhandle);

		virtual void				BeginRender();
	
		virtual void				EndRender();
		
		virtual	Matrix4				MakePerspectiveMatrix(Matrix4& out, float fovy, float Aspect, float zn, float zf);

		virtual	Matrix4				MakeOrthoMatrix(Matrix4& out, float width, float height, float zn, float zf);

		virtual Matrix4				MakeOrthoMatrixOffCenter(Matrix4& out, float left, float right, float bottom, float top, float zn, float zf);

		virtual	void				BeginProfile(const char* pszLale);

		virtual	void				EndProfile();

		virtual	bool				CheckTextureFormat(FORMAT eFormat,USAGE eUsage);

		LPDIRECT3DDEVICE9			GetDXDevive() {return m_pD3DDevice;}
	
	private:
		LPDIRECT3D9						m_pD3D;			 // The main D3D object
		UINT							m_nAdapterToUse;
		D3DPRESENT_PARAMETERS			m_d3dpp;         // Parameters for CreateDevice/Reset
		LPDIRECT3DDEVICE9				m_pD3DDevice;
		HWND							m_hWnd;

		RenderState						m_curState;
	};

	LPDIRECT3DDEVICE9 GetD3D9DxDevive();
}

#endif


