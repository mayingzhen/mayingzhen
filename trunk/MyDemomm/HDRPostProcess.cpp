#include "HDRPostProcess.h"
#include "GameApp.h"
#include "D3DRender.h"

CHDRPostProcess g_HDRPostProcess;

CHDRPostProcess::CHDRPostProcess()
{
	//m_SumLums.resize(NUM_TONEMAP_TEXTURES);
	memset(m_lumTexs, 0, sizeof(m_lumTexs) ); 
	memset(m_DownSampleTex, 0, sizeof(m_DownSampleTex) );
	memset(m_GlowTex, 0, sizeof(m_GlowTex) );
	memset(m_BureTempTex, 0, sizeof(m_BureTempTex) );
	m_GlowMergedTex = NULL;
	memset(m_AdaptedTex, 0, sizeof(m_AdaptedTex) );
}

CHDRPostProcess::~CHDRPostProcess()
{
	for (int i = 0; i < NUM_SUM_LUM + 1; ++i)
	{
		SAFE_RELEASE(m_lumTexs[i]);
	}
	for (int i = 0; i < 3 + 1; ++i)
	{
		SAFE_RELEASE(m_DownSampleTex[i]);
		SAFE_RELEASE(m_GlowTex[i]);
		SAFE_RELEASE(m_BureTempTex[i]);
	}
	SAFE_RELEASE(m_GlowMergedTex);
	for (int i =0; i < AdaptedTex_NUM; ++i)
	{
		SAFE_RELEASE(m_AdaptedTex[i]);
	}
}

void CHDRPostProcess::Init()
{
}

HRESULT CHDRPostProcess::CreateRTtexture()
{
	HRESULT hr = D3D_OK;

	int len = 1;
	for (int i = NUM_SUM_LUM + 1 - 1; i >= 0; --i)
	{
		hr = g_pD3DDevice->CreateTexture( len, len, 1, D3DUSAGE_RENDERTARGET,
			D3DFMT_R16F, D3DPOOL_DEFAULT, &m_lumTexs[i], NULL );
		if( FAILED( hr ) )
			return hr;

		len *= 4;
	}

	int width = g_nWndWidth;
	int height = g_nWndHeigh;

	//hr = g_pD3DDevice->CreateTexture( width / 2, height / 2, 1, D3DUSAGE_RENDERTARGET,
	//	D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &m_DownSampleTex[0], NULL );
	
	for (int i = 0; i < 3; ++i)
	{
		hr = g_pD3DDevice->CreateTexture( width / (2 << i), height / (2 << i), 1, D3DUSAGE_RENDERTARGET,
			D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &m_DownSampleTex[i], NULL );

		hr = g_pD3DDevice->CreateTexture( width / (2 << i), height / (2 << i), 1, D3DUSAGE_RENDERTARGET,
			D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &m_GlowTex[i], NULL );

		hr = g_pD3DDevice->CreateTexture( width / (2 << i), height / (2 << i), 1, D3DUSAGE_RENDERTARGET,
			D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &m_BureTempTex[i], NULL );
	}

	hr = g_pD3DDevice->CreateTexture( width / 2, height / 2, 1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &m_GlowMergedTex, NULL );

	for (int i = 0; i < AdaptedTex_NUM; ++i)
	{
		hr = g_pD3DDevice->CreateTexture( 1, 1, 1, D3DUSAGE_RENDERTARGET,
			D3DFMT_R32F, D3DPOOL_DEFAULT, &m_AdaptedTex[i], NULL );
	}

	return hr;
}

HRESULT CHDRPostProcess::LoadShader()
{
	HRESULT hr = D3D_OK;

	m_SumLogTech.loadResource("shader\\HDRPostProcess\\SumLuminanceLog.fx");
	m_SumLumIterativeTech.loadResource("shader\\HDRPostProcess\\SumLuminanceIterative.fx");
	m_AdaptedLumTech.loadResource("shader\\HDRPostProcess\\AdaptedLum.fx");
	m_BrightPassTech.loadResource("shader\\HDRPostProcess\\BrightPass.fx");
	m_DownSamplersTech.loadResource("shader\\HDRPostProcess\\DownSample.fx");
	m_BlurXTech.loadResource("shader\\HDRPostProcess\\BlurX.fx");
	m_BlurYTech.loadResource("shader\\HDRPostProcess\\BlurY.fx");
	m_GlowMergeTech.loadResource("shader\\HDRPostProcess\\GlowMerge.fx");
	m_ToneMappingTech.loadResource("shader\\HDRPostProcess\\ToneMaping.fx");

	return hr;
}


void CHDRPostProcess::Render(LPDIRECT3DTEXTURE9 pInputTex, LPDIRECT3DTEXTURE9 pOutputTex)
{
	PROFILE_LABEL_PUSH("HDRPostProcess");

	HRESULT hr = D3D_OK;

	LPDIRECT3DSURFACE9 pOldRT0 = NULL;
	g_pD3DDevice->GetRenderTarget(0, &pOldRT0);

	MeasureLuminance();

	AdaptedLum();

	BrightPassDownSample(pInputTex);

	BloomPass();

 	if (pOutputTex == NULL)
 	{
		g_pD3DDevice->SetRenderTarget(0,pOldRT0);
 	}
 	else
 	{
 		LPDIRECT3DSURFACE9 pOutSurf = NULL;
 		hr = pOutputTex->GetSurfaceLevel( 0, &pOutSurf );
 		g_pD3DDevice->SetRenderTarget(0, pOutSurf);	
 	}
 	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	ToneMapping(pInputTex);

	if (pOutputTex != NULL)
	{
		g_pD3DDevice->SetRenderTarget(0,pOldRT0);
	}

	PROFILE_LABEL_POP("HDRPostProcess");
}

void CHDRPostProcess::MeasureLuminance()
{
	HRESULT hr = D3D_OK;

	LPDIRECT3DSURFACE9 pLumSurf = NULL;
	hr = m_lumTexs[0]->GetSurfaceLevel( 0, &pLumSurf );
	g_pD3DDevice->SetRenderTarget(0,pLumSurf);
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	D3DXVECTOR4 texCoordOffset[2];
	GetSampleOffsets4x4(g_nWndWidth, g_nWndHeigh, texCoordOffset, 2);

	m_SumLogTech.begin();
	m_SumLogTech.BeginPass(0);

	LPD3DXEFFECT effect = g_pCurCEffect->effect();
	hr = effect->SetVectorArray("tex_coord_offset", texCoordOffset, 2);

	g_SceneQuad.Render();

	m_SumLogTech.endPass();
	m_SumLogTech.end();
	/////////////////	

	for (int i = 0; i < NUM_SUM_LUM; ++i)
	{
		//m_SumLums[i].Render();
		//m_SumLums[i].Init( m_lumTexs[i], m_lumTexs[i + 1], "SumLumIterative" );

		LPDIRECT3DSURFACE9 pLumSurf = NULL;
		hr = m_lumTexs[i]->GetSurfaceLevel( 0, &pLumSurf );
		g_pD3DDevice->SetRenderTarget(0, pLumSurf);
		g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		m_SumLumIterativeTech.begin();
		m_SumLumIterativeTech.BeginPass(0);

		LPD3DXEFFECT effect = g_pCurCEffect->effect();
		bool res = g_pCurCEffect->SetTexture("g_TextureSrc", m_lumTexs[i + 1]);
		hr = effect->SetVectorArray("tex_coord_offset", texCoordOffset, 2);

		g_SceneQuad.Render();

		m_SumLumIterativeTech.endPass();
		m_SumLumIterativeTech.end();

	}
}

void CHDRPostProcess::AdaptedLum()
{
	HRESULT hr = D3D_OK;

	//m_AdaptedLum.Render();
	//std::vector<PDIRECT3DTEXTURE9> inAdapTexs;
	//inAdapTexs.push_back(m_lumTexs[ m_SumLums.size() ]);
	//inAdapTexs.push_back(m_AdaptedTex[AdaptedTex_LAST]);
	//m_AdaptedLum.Init( inAdapTexs, m_AdaptedTex[AdaptedTex_CUR], "AdaptedLum" );
	
	// Swap current & last luminance
	LPDIRECT3DTEXTURE9 pTexSwap = m_AdaptedTex[AdaptedTex_LAST];
	m_AdaptedTex[AdaptedTex_LAST] = m_AdaptedTex[AdaptedTex_CUR];
	m_AdaptedTex[AdaptedTex_CUR] = pTexSwap;
	
	LPDIRECT3DSURFACE9 pAdapSurfCur = NULL;
	hr = m_AdaptedTex[AdaptedTex_CUR]->GetSurfaceLevel( 0, &pAdapSurfCur );
	g_pD3DDevice->SetRenderTarget(0,pAdapSurfCur);
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	m_AdaptedLumTech.begin();
	m_AdaptedLumTech.BeginPass(0);

	LPD3DXEFFECT effect = g_pCurCEffect->effect();
	hr = effect->SetTexture("g_TextureSrc", m_lumTexs[NUM_SUM_LUM]);
	hr = effect->SetTexture("last_lum_tex", m_AdaptedTex[AdaptedTex_LAST]);
	hr = effect->SetFloat("frame_delta",g_ElapsedTime);

	g_SceneQuad.Render();

	m_AdaptedLumTech.endPass();
	m_AdaptedLumTech.end();

	//////////////////////////
}

void CHDRPostProcess::BrightPassDownSample(LPDIRECT3DTEXTURE9 pInputTex)
{
	HRESULT hr = D3D_OK;
	//m_BrightPassDownSampler.Render();
	//m_BrightPassDownSampler.Init( pInputTex, m_DownSampleTex[0], "bright_pass_downsampler2x2" );
	m_BrightPassTech.begin();
	m_BrightPassTech.BeginPass(0);

	LPDIRECT3DSURFACE9 pDownSampleSurf = NULL;
	hr = m_DownSampleTex[0]->GetSurfaceLevel( 0, &pDownSampleSurf );
	g_pD3DDevice->SetRenderTarget(0, pDownSampleSurf);
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	LPD3DXEFFECT effect = g_pCurCEffect->effect();
	hr = effect->SetTexture("g_TextureSrc", pInputTex);
	hr = effect->SetTexture("g_TextureLum", m_AdaptedTex[AdaptedTex_CUR]);

	g_SceneQuad.Render();

	m_BrightPassTech.endPass();
	m_BrightPassTech.end();
	/////////////////////

	for (int i = 0; i < 2; ++i)
	{
		//m_DownSamplers->Render();
		//m_DownSamplers[i].Init( m_DownSampleTex[i], m_DownSampleTex[i + 1], "downsampler2x2"  );
		m_DownSamplersTech.begin();
		m_DownSamplersTech.BeginPass(0);

		LPDIRECT3DSURFACE9 pDownSampleSurf = NULL;
		hr = m_DownSampleTex[i + 1]->GetSurfaceLevel( 0, &pDownSampleSurf );
		g_pD3DDevice->SetRenderTarget(0, pDownSampleSurf);
		g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		LPD3DXEFFECT effect = g_pCurCEffect->effect();
		hr = effect->SetTexture("g_TextureSrc", m_DownSampleTex[i]);

		g_SceneQuad.Render();

		m_DownSamplersTech.endPass();
		m_DownSamplersTech.end();
	}
}


void CHDRPostProcess::BloomPass()
{
	HRESULT hr = D3D_OK;

	for (int i = 0; i < 3; ++i)
	{
		//m_Blurs[i].Render();
		//m_Blurs[i].Init( m_DownSampleTex[i], m_GlowTex[i], "" );
		LPDIRECT3DSURFACE9 pBureTempSurf = NULL;
		hr = m_BureTempTex[i]->GetSurfaceLevel( 0, &pBureTempSurf );
		g_pD3DDevice->SetRenderTarget(0, pBureTempSurf);
		g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		/// x
		m_BlurXTech.begin();
		m_BlurXTech.BeginPass(0);

		LPD3DXEFFECT effect = g_pCurCEffect->effect();

		float width = g_nWndWidth;
		float heigh = g_nWndHeigh;

		D3DXVECTOR2 texSize = D3DXVECTOR2(width, 1.0f / width);

		std::vector<float> color_weight;
		std::vector<float> tex_coord_offset;
		CalSampleOffsetsGauss(8,1,width,3.0f,color_weight,tex_coord_offset);
		hr = effect->SetFloatArray( "color_weight", &(color_weight[0]), 8 );
		hr = effect->SetFloatArray( "tex_coord_offset", &(tex_coord_offset[0]), 8 );
		hr = effect->SetValue( "src_tex_size", &(texSize), sizeof(D3DXVECTOR2) );
		hr = effect->SetTexture( "g_TextureSrc", m_DownSampleTex[i] );

		g_SceneQuad.Render();

		m_BlurXTech.endPass();
		m_BlurXTech.end();

		/// y
		m_BlurYTech.begin();
		m_BlurYTech.BeginPass(0);

		LPDIRECT3DSURFACE9 pGlowSurf = NULL;
		hr = m_GlowTex[i]->GetSurfaceLevel( 0, &pGlowSurf );
		g_pD3DDevice->SetRenderTarget(0, pGlowSurf);
		g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		texSize = D3DXVECTOR2(heigh, 1.0f / heigh);

		effect = g_pCurCEffect->effect();
		CalSampleOffsetsGauss(8,1,heigh,3.0f,color_weight,tex_coord_offset);
		hr = effect->SetFloatArray( "color_weight", &(color_weight[0]), 8 );
		hr = effect->SetFloatArray( "tex_coord_offset", &(tex_coord_offset[0]), 8 );
		hr = effect->SetValue( "src_tex_size", &(texSize), sizeof(D3DXVECTOR2) );
		hr = effect->SetTexture( "g_TextureSrc", m_BureTempTex[i] );

		g_SceneQuad.Render(); 

		m_BlurYTech.endPass();
		m_BlurYTech.end();
	}

	//m_GlowMerge.Render();
	//std::vector<PDIRECT3DTEXTURE9> inGlowTexs( m_GlowTex[0], m_GlowTex[2] );
	//m_GlowMerge.Init( inGlowTexs, m_GlowMergedTex, "glow_merger" );
	LPDIRECT3DSURFACE9 pGlowMergeSurf = NULL;
	hr = m_GlowMergedTex->GetSurfaceLevel( 0, &pGlowMergeSurf );
	g_pD3DDevice->SetRenderTarget(0, pGlowMergeSurf);
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	m_GlowMergeTech.begin();
	m_GlowMergeTech.BeginPass(0);

	LPD3DXEFFECT effect = g_pCurCEffect->effect();
	hr = effect->SetTexture("glow_tex_0", m_GlowTex[0]);
	hr = effect->SetTexture("glow_tex_1", m_GlowTex[1]);
	hr = effect->SetTexture("glow_tex_2", m_GlowTex[2]);

	g_SceneQuad.Render();

	m_GlowMergeTech.endPass();
	m_GlowMergeTech.end();
	//////////////////////////
}

void CHDRPostProcess::ToneMapping(LPDIRECT3DTEXTURE9 pInputTex)
{
	HRESULT hr;
	//m_ToneMapping.Render();
	//std::vector<PDIRECT3DTEXTURE9> inToneMapTexs;
	//inToneMapTexs.push_back(pInputTex);
	//inToneMapTexs.push_back(m_AdaptedTex[AdaptedTex_CUR]);
	//inToneMapTexs.push_back(m_GlowMergedTex);
	//m_ToneMapping.Init( inToneMapTexs, pOutputTex, "ToneMapping" );

	m_ToneMappingTech.begin();
	m_ToneMappingTech.BeginPass(0);

	LPD3DXEFFECT effect = g_pCurCEffect->effect();
	hr = effect->SetTexture("g_TextureSrc", pInputTex);
	hr = effect->SetTexture("g_TextureLum", m_AdaptedTex[AdaptedTex_CUR]);
	hr = effect->SetTexture("g_TextureBloom", m_GlowMergedTex);

	g_SceneQuad.Render();

	m_ToneMappingTech.endPass();
	m_ToneMappingTech.end();
}

void CHDRPostProcess::GetSampleOffsets4x4(int width, int height,D3DXVECTOR4 texCoordOffSet[],int nArray)
{
	float const tu = 1.0f / width;
	float const tv = 1.0f / height;

	// Sample from the 16 surrounding points.
	int index = 0;
	for (int y = -1; y <= 2; y += 2)
	{
		for (int x = -1; x <= 2; x += 4) 
		{
			if (index >= nArray)
				return;

			texCoordOffSet[index].x = (x + 0) * tu;
			texCoordOffSet[index].y = y * tv;
			texCoordOffSet[index].z = (x + 2) * tu;
			texCoordOffSet[index].w = y * tv;

			++ index;
		}
	}
}

float CHDRPostProcess::GaussianDistribution(float x, float y, float rho)
{
	float g = 1.0f / sqrtf( 2.0f * D3DX_PI * rho * rho );
	g *= expf( -( x * x + y * y ) / ( 2 * rho * rho ) );

	return g;
}

void CHDRPostProcess::CalSampleOffsetsGauss(int kernelRaidius, float multiplier,
						   int texSize, float deviation,
						   std::vector<float>& color_weight,
						   std::vector<float>& tex_coord_offset)
{
	color_weight.clear();
	tex_coord_offset.clear();
	color_weight.resize(kernelRaidius);
	tex_coord_offset.resize(kernelRaidius);

	std::vector<float> tmp_weights(kernelRaidius * 2, 0);
	std::vector<float> tmp_offset(kernelRaidius * 2, 0);

	float const tu = 1.0f / texSize;

	float sum_weight = 0;
	for (int i = 0; i < 2 * kernelRaidius; ++ i)
	{
		float weight = GaussianDistribution(static_cast<float>(i - kernelRaidius), 0, kernelRaidius / deviation);
		tmp_weights[i] = weight;
		sum_weight += weight;
	}
	for (int i = 0; i < 2 * kernelRaidius; ++ i)
	{
		tmp_weights[i] /= sum_weight;
	}

	// Fill the offsets
	for (int i = 0; i < kernelRaidius; ++ i)
	{
		tmp_offset[i]                  = static_cast<float>(i - kernelRaidius);
		tmp_offset[i + kernelRaidius] = static_cast<float>(i);
	}

	// Bilinear filtering taps
	// Ordering is left to right.
	for (int i = 0; i < kernelRaidius; ++ i)
	{
		float const scale = tmp_weights[i * 2] + tmp_weights[i * 2 + 1];
		float const frac = tmp_weights[i * 2] / scale;

		tex_coord_offset[i] = (tmp_offset[i * 2] + (1 - frac)) * tu;
		color_weight[i] = multiplier * scale;
	}
}



