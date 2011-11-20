#ifndef __SHADOWMAP_H__
#define __SHADOWMAP_H__

#include <list>
#include "public.h"

class Light;
class CObject;

//#define PIX_DEBUG  // PIX for window 调试的时候创建D24S8纹理失败

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	bool Create(int iSizeX,int iSizeY);
	void Destroy();

	// start rendering to texture
	void EnableRendering();

	// stop rendering to texture
	void DisableRendering();

	inline D3DVIEWPORT9 &GetViewport(void) { return m_Viewport; }

	LPDIRECT3DTEXTURE9 GetColorTexture() {return m_pTexture;}

#ifdef PIX_DEBUG
	LPDIRECT3DTEXTURE9 GetDepthTexture() {return m_pTexture;}
#else
	LPDIRECT3DTEXTURE9 GetDepthTexture() {return m_pDSTexture;}
#endif
	
	D3DXMATRIX* GetTexScaleBiasMat() {return &m_TexScaleBiasMat;}

public:
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

	D3DXMATRIX m_viewMat;
	D3DXMATRIX m_projMat;

	int m_nIndex; 

	int m_nSizeX;
	int m_nSizeY;

	Light* m_Light; 
	std::list<CObject*> m_casterList;
};


class CShadowMapPool
{
public:
	static ShadowMap* GetOneShdowMap(int nSizeX = 1024, int nSizeY = 1024);
	static void ClearAllUseFlag();

private:
	enum {MAX_NUM_SM = 40};
	static ShadowMap m_allShadowMap[MAX_NUM_SM];
	static bool m_useFlag[MAX_NUM_SM];
	static bool m_creatFlag[MAX_NUM_SM];
};

#endif
