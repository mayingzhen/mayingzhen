#ifndef  _ShadowMap__H__
#define  _ShadowMap__H__

#define PIX_DEBUG  // PIX for window 调试的时候创建D24S8纹理失败

namespace ma
{
	class ShadowMap
	{
	public:
		ShadowMap();
		
		~ShadowMap();

		void				Init(LPDIRECT3DDEVICE9 pDxDevice,int iSizeX = 1024,int iSizeY = 1024);

		UINT				GetCasterNumber() {return m_arrCasterList.size();}

		IRenderItem*		GetCasterByIndex(UINT index) {return m_arrCasterList[index];}

		void				ClearCaster() {m_arrCasterList.clear();}

		void				FindCaster(IRenderItem** arrRenderItem,int nNumber);

		void				CalculateMatrix(Camera* pCamera,int nSpitIndex,Light* pLight);

		void				Destroy();

		void				BeginRender();

		void				EndRender();

		const D3DXMATRIX&	GetViewMatrix() {return m_viewMat;}

		const D3DXMATRIX&	GetProjMatrix() {return m_projMat;}

		const D3DXMATRIX&	GetTexMatrix()  {return m_TexMat;}

		D3DVIEWPORT9&		GetViewport(void) { return m_Viewport; }

		LPDIRECT3DTEXTURE9	GetColorTexture() {return m_pTexture;}

#ifdef PIX_DEBUG
		LPDIRECT3DTEXTURE9 GetDepthTexture() {return m_pTexture;}
#else
		LPDIRECT3DTEXTURE9	GetDepthTexture() {return m_pDSTexture;}
#endif

	private:
		D3DXMATRIX CalculateCropMatrix();

	private:
		LPDIRECT3DTEXTURE9			m_pTexture;
		LPDIRECT3DSURFACE9			m_pSurface;
		LPDIRECT3DTEXTURE9			m_pDSTexture;
		LPDIRECT3DSURFACE9			m_pDSSurface;
		D3DVIEWPORT9				m_Viewport;
		LPDIRECT3DSURFACE9			m_pOldDSSurface;
		LPDIRECT3DSURFACE9			m_pOldRenderTarget;
		D3DVIEWPORT9				m_OldViewport;
		LPDIRECT3DDEVICE9			m_pDxDevice;

		D3DXMATRIX					m_viewMat;
		D3DXMATRIX					m_projMat;
		D3DXMATRIX					m_TexScaleBiasMat;
		D3DXMATRIX					m_TexMat;

		int							m_nSizeX;
		int							m_nSizeY;

		Light*						m_pLight; 
		Camera*						m_pCamera;
		int							m_nIndex; 
		std::vector<IRenderItem*>	m_arrCasterList;	
	};
}

#endif
