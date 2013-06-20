#ifndef  _D3D9RENDERDevice__H__
#define  _D3D9RENDERDevice__H__

#include "Engine/RenderSystem/IRenderDevice.h"

namespace ma
{
	class ShadowMap;
	class LineRender;
	class ScreenQuad;
	class UnitSphere;
	class IndexBuffer;
	class VertexBuffer;
	class RenderTarget;
	class D3D9Technique;
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

		
		//// Render
		virtual RenderTarget*	CreateRenderTarget();

		virtual RenderTarget*	SetRenderTarget(RenderTarget* pTexture,int index = 0);

		virtual Rectangle		SetViewport(const Rectangle& rect);

		virtual void			SetRenderState(const RenderState& state);
		
		virtual	void			DrawRenderable(Renderable* pRenderable);

		virtual	void			ClearBuffer(bool bColor, bool bDepth, bool bStencil,const Color & c, float z, int s);
		
		virtual void			DrawLine(const Vector3& p0,const Vector3& p1,Uint32 dwColor);


		/// Render Pass

		virtual	void			Init(HWND wndhandle);

		virtual void			BeginRender();
	
		virtual void			EndRender();

		virtual void			SetCamera(Camera* pCamera);

		virtual Camera*			GetCamera();

		virtual void			ClearLightList();

		//virtual void			AddLight(Light* pLigt);

		virtual	void			GetRenderWndSize(int& Width,int& Heigh);
		
		virtual	Matrix4x4		MakePerspectiveMatrix(Matrix4x4 *pOut, float fovy, float Aspect, float zn, float zf);

		virtual	Matrix4x4		MakeOrthoMatrix(Matrix4x4 *pOut, float width, float height, float zn, float zf);

		virtual Matrix4x4		MakeOrthoMatrixOffCenter(Matrix4x4 *pOut, float left, float right, float bottom, float top, float zn, float zf);

		LPDIRECT3DDEVICE9		GetDXDevive() {return m_pD3DDevice;}
	
	private:
		void					InitD3D9(HWND hWnd);

		void					InitRender();

		void					FlushLine();

		void					FlushRenderQueue();

		void					ShadowMapPass();

		void					GBufferPass();	

		void					DefferdShadowPass();

		void					DefferdLigtPass();

		void					ShadingPass();

	private:
		LPDIRECT3D9						m_pD3D;			 // The main D3D object
		D3DPRESENT_PARAMETERS			m_d3dpp;         // Parameters for CreateDevice/Reset
		LPDIRECT3DDEVICE9				m_pD3DDevice;
		HWND							m_hWnd;

// 		ID3DXEffect*					m_pGBufferTech;
// 		ID3DXEffect*					m_pShdowMapTech;
// 		ID3DXEffect*					m_DefferedShdowTech;
// 	 	ID3DXEffect*					m_DeferredLightTech;
// 		ID3DXEffect*					m_pShadingTech;

// 		LPDIRECT3DTEXTURE9				m_pDepthTex;
// 		LPDIRECT3DTEXTURE9				m_pNormalTex;
// 		LPDIRECT3DTEXTURE9				m_pDiffuseTex;
// 		LPDIRECT3DTEXTURE9				m_pSpeculaTex;
// 		LPDIRECT3DTEXTURE9				m_pShadowTex;

		Camera*							m_pMainCamera;

		std::vector<IRenderItem*>		m_SolidEntry;
		std::vector<IRenderItem*>		m_TransEntry;

		std::vector<Light*>				m_Ligts;

		Light*							m_mainLigt;

		ShadowMap*						m_arrShadowMap[4];

		LineRender*						m_pLineRender;

		ScreenQuad*						m_pScreenQuad;
		
		UnitSphere*						m_pUnitSphere;

		enum {MAX_RENDER_TARGET = 4};
		RenderTarget*					m_pCurRenderTarget[MAX_RENDER_TARGET];

		Rectangle						m_curViewport;
	
	};

	LPDIRECT3DDEVICE9 GetD3D9DxDevive();
}

#endif


