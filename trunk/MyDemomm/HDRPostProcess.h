#ifndef  C_HDR_POST_PROCESS__H__
#define  C_HDR_POST_PROCESS__H__

//#include "PostProcess.h"

#include "ScreenQuad.h"
#include "effect_file.h"

class CHDRPostProcess 
{
public:
	CHDRPostProcess();
	~CHDRPostProcess();

	void Init();
	void Render(LPDIRECT3DTEXTURE9 pInputTex, LPDIRECT3DTEXTURE9 pOutputTex);

	HRESULT CreateRTtexture();
	HRESULT LoadShader();

private:
	void MeasureLuminance();
	void AdaptedLum();
	void BrightPassDownSample(LPDIRECT3DTEXTURE9 pInputTex);
	void BloomPass();
	void ToneMapping(LPDIRECT3DTEXTURE9 pInputTex);

	static void GetSampleOffsets4x4(int width, int height,D3DXVECTOR4 texCoordOffSet[],int nArray);
	static float GaussianDistribution(float x, float y, float rho);
	static void CalSampleOffsetsGauss(int kernelRaidius, float multiplier,
		int texSize, float deviation,
		std::vector<float>& color_weight,
		std::vector<float>& tex_coord_offset);


public:

	enum
	{
		NUM_SUM_LUM = 3
	};

	enum
	{
		AdaptedTex_CUR = 0,
		AdaptedTex_LAST = 1,
		AdaptedTex_NUM = 2,
	};

// 	CPostProcess m_SumLumlog;
// 	std::vector<CPostProcess> m_SumLums;
// 	CPostProcess m_AdaptedLum;
// 	CPostProcess m_BrightPassDownSampler;
// 	CPostProcess m_DownSamplers[2];
// 	CPostProcess m_Blurs[3];
// 	CPostProcess m_GlowMerge;
// 	CPostProcess m_ToneMapping;
	
	LPDIRECT3DTEXTURE9 m_lumTexs[NUM_SUM_LUM + 1]; 
	LPDIRECT3DTEXTURE9 m_DownSampleTex[3];
	LPDIRECT3DTEXTURE9 m_GlowTex[3];
	LPDIRECT3DTEXTURE9 m_BureTempTex[3];
	LPDIRECT3DTEXTURE9 m_GlowMergedTex;
	LPDIRECT3DTEXTURE9 m_AdaptedTex[AdaptedTex_NUM];

	//std::vector<D3DXVECTOR4> m_tex_coord_offset(2);
	//D3DXVECTOR4 m_TexcoordOffsetX;
	//D3DXVECTOR4 m_TexcoordOffsetY;

	CScreenQuad m_ScreenQuad;

	CEffectFile m_SumLogTech;
	CEffectFile m_SumLumIterativeTech;
	CEffectFile m_AdaptedLumTech;
	CEffectFile m_BrightPassTech;
	CEffectFile m_DownSamplersTech;
	CEffectFile m_BlurXTech;
	CEffectFile m_BlurYTech;
	CEffectFile m_GlowMergeTech;
	CEffectFile m_ToneMappingTech;
};

extern CHDRPostProcess g_HDRPostProcess;

#endif