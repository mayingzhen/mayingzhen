#ifndef __SHADOWMAP_H__
#define __SHADOWMAP_H__

#include "public.h"

//#define PIX_DEBUG  // PIX for window 调试的时候创建D24S8纹理失败

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	bool Create(int iSizeX,int iSizeY);
	void Destroy(void);

	// start rendering to texture
	void EnableRendering(void);
	// stop rendering to texture
	void DisableRendering(void);

	inline D3DVIEWPORT9 &GetViewport(void) { return m_Viewport; }

	LPDIRECT3DTEXTURE9 GetColorTexture() {return m_pTexture;}

#ifdef PIX_DEBUG
	LPDIRECT3DTEXTURE9 GetDepthTexture() {return m_pTexture;}
#else
	LPDIRECT3DTEXTURE9 GetDepthTexture() {return m_pDSTexture;}
#endif
	
	D3DXMATRIX* GetTexScaleBiasMat() {return &m_TexScaleBiasMat;}

public:
	int m_iBytesPerTexel;

	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DSURFACE9 m_pSurface;

	LPDIRECT3DTEXTURE9 m_pDSTexture;
	LPDIRECT3DSURFACE9 m_pDSSurface;
	
	D3DVIEWPORT9 m_Viewport;

	// temp
	LPDIRECT3DSURFACE9 m_pOldDSSurface;
	LPDIRECT3DSURFACE9 m_pOldRenderTarget;
	D3DVIEWPORT9 m_OldViewport;

	D3DXMATRIX m_TexScaleBiasMat;
	D3DXMATRIX m_TexMat;

	int m_nSizeX;
	int m_nSizeY;
};

#endif
