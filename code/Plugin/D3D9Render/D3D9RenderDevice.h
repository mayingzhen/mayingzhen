#ifndef  _D3D9RENDERDevice__H__
#define  _D3D9RENDERDevice__H__

namespace ma
{
	class IndexBuffer;
	class VertexBuffer;
	class RenderTarget;
	class D3D9RenderTarget;

	class D3D9RenderDevice : public IRenderDevice
	{
	public:
		D3D9RenderDevice();

		~D3D9RenderDevice();

		/// Vido Buffer 
		virtual Texture*			CreateRendTexture();

		virtual VertexDeclaration*	CreateVertexDeclaration();

		virtual VertexBuffer*		CreateVertexBuffer(void* pData = NULL, int nsize = 0, int nStride = 0, USAGE Usgae = USAGE_STATIC);

		virtual IndexBuffer*		CreateIndexBuffer(void* Data = NULL, int size = 0, INDEX_TYPE eIndexType = INDEX_TYPE_U16, USAGE Usgae = USAGE_STATIC);

		virtual ShaderProgram*		CreateShaderProgram();

		virtual const char*			GetShaderPath();

		virtual void				ConvertUV(float& fTop,float& fLeft,float& fRight,float& fBottom);

		virtual float				GetHalfPixelOffset(float fHalfPiexl);
		
		virtual RenderTarget*		CreateRenderTarget(int nWidth = -1,int nHeight = -1,FORMAT format = FMT_A8R8G8B8);

		virtual RenderTarget*		SetRenderTarget(RenderTarget* pTexture,int index = 0);

		virtual RenderTarget*		SetDepthStencil(RenderTarget* pTexture);

		virtual Rectangle			SetViewport(const Rectangle& rect);

		virtual void				SetRenderState(const RenderState& state);
		
		virtual	void				DrawRenderable(Renderable* pRenderable);

		virtual void				DrawDynamicRenderable(Renderable* pRenderable); 

		virtual	void				ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s);

		virtual	void				Init(HWND wndhandle);

		virtual void				BeginRender();
	
		virtual void				EndRender();
		
		virtual	Matrix4x4			MakePerspectiveMatrix(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf);

		virtual	Matrix4x4			MakeOrthoMatrix(Matrix4x4 *pOut, float width, float height, float zn, float zf);

		virtual Matrix4x4			MakeOrthoMatrixOffCenter(Matrix4x4 *pOut, float left, float right, float bottom, float top, float zn, float zf);

		LPDIRECT3DDEVICE9			GetDXDevive() {return m_pD3DDevice;}
	
	private:
		void						InitD3D9(HWND hWnd);

	private:
		LPDIRECT3D9						m_pD3D;			 // The main D3D object
		D3DPRESENT_PARAMETERS			m_d3dpp;         // Parameters for CreateDevice/Reset
		LPDIRECT3DDEVICE9				m_pD3DDevice;
		HWND							m_hWnd;

		enum {MAX_RENDER_TARGET = 4};
		D3D9RenderTarget*				m_pCurRenderTarget[MAX_RENDER_TARGET];
		D3D9RenderTarget*				m_pCurDepthStencil;
		
		Rectangle						m_curViewport;
	
		RenderState						m_curState;
	};

	LPDIRECT3DDEVICE9 GetD3D9DxDevive();
}

#endif


