#ifndef __D3D_RENDER_H__
#define __D3D_RENDER_H__

#include "effect_file.h"
#include "ShadowMap.h"
#include "ScreenQuad.h"

class CD3DRender
{
public:
	CD3DRender(); 
	~CD3DRender(); 

	void Init();
	void Destroy();

	void Render();

	void CreateRT();
	void LoadShader();

private:
	void GBufferPass();
	void ShadowMapPass();
	void DefferdShadow();
	void DeferredLight();
	void ShadingPass();
	void RenderWater();
	void RenderSSAO();

private:
	bool m_bIsHDRRending;

	// GBuffer
	CEffectFile m_gBufferTech;
	CEffectFile m_SkinGBufferTech;
	LPDIRECT3DTEXTURE9 m_pDepthTex;
	LPDIRECT3DTEXTURE9 m_pNormalTex;

	// shadow map
	CEffectFile m_shadowMapTech;
	CEffectFile m_SkinShadowMapTech;	
	
	// defferred lighting
	CEffectFile m_DeferredLightTech;
	LPDIRECT3DTEXTURE9 m_pDiffuseTex;
	LPDIRECT3DTEXTURE9 m_pSpeculaTex;

	// defferred shadow
	CEffectFile m_DefferedShdowTech;
	LPDIRECT3DTEXTURE9 m_pShadowTex;
	//LPDIRECT3DVOLUMETEXTURE9 m_pJitterTexture;
		
	// shading
	CEffectFile m_ShadingTech;
	CEffectFile m_SkinShadingTech;
	CEffectFile m_TerrainTech;
	LPDIRECT3DTEXTURE9 m_pShdingTex;

	/// 
	CEffectFile m_WaterTech;

	/// ssao
	LPDIRECT3DTEXTURE9 m_pSSAOTex;
	LPDIRECT3DTEXTURE9 m_pBlurSSAOTex;
	LPDIRECT3DTEXTURE9 m_pBlurTempTex;
	CEffectFile m_SSAOTech;
	CEffectFile m_BilateralBlurX;
	CEffectFile m_BilateralBlurY;
};

extern CD3DRender g_D3DRender;




#endif