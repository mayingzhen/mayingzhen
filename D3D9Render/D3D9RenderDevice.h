#ifndef  _D3D9RENDERDevice__H__
#define  _D3D9RENDERDevice__H__

#include "Engine/RenderSystem/IRenderDevice.h"

namespace ma
{
	class ShadowMap;
	class LineRender;
	class ScreenQuad;

	class D3D9RENDER_API D3D9RenderDevice : public IRenderDevice
	{
	public:
		D3D9RenderDevice();

		~D3D9RenderDevice();

		virtual	void			Init(int wndhandle);

		virtual void			BeginRender();
	
		virtual void			EndRender();

		virtual IRendMesh*		CreateRendMesh();

		virtual IRendTexture*	CreateRendTexture();

		virtual void			SetCamera(Camera* pCamera);

		virtual Camera*			GetCamera();

		virtual void			ClearLightList();

		virtual void			AddLight(Light* pLigt);

		virtual void			RenderMesh(IRendMesh* pMesh);

		virtual	void			GetRenderWndSize(int& Width,int& Heigh);

		virtual void			DrawLine(const D3DXVECTOR3& p0,const D3DXVECTOR3& p1,xmUint32 dwColor);

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

		ID3DXEffect*					m_pDefault;
		ID3DXEffect*					m_pDefaultSkin;
		ID3DXEffect*					m_pGBufferTech;
		ID3DXEffect*					m_pShdowMapTech;
	 	ID3DXEffect*					m_DeferredLightTech;
		ID3DXEffect*					m_pShadingTech;

		LPDIRECT3DTEXTURE9				m_pDepthTex;
		LPDIRECT3DTEXTURE9				m_pNormalTex;
		LPDIRECT3DTEXTURE9				m_pDiffuseTex;
		LPDIRECT3DTEXTURE9				m_pSpeculaTex;
		LPDIRECT3DTEXTURE9				m_pShadowTex;

		Camera*							m_pMainCamera;

		std::vector<IRenderItem*>		m_SolidEntry;
		std::vector<IRenderItem*>		m_TransEntry;

		std::vector<Light*>				m_Ligts;

		Light*							m_mainLigt;

		ShadowMap*						m_arrShadowMap[4];

		LineRender*						m_pLineRender;

		ScreenQuad*						m_pScreenQuad;

	};
}

#endif


