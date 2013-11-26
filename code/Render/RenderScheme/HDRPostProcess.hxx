#include "HDRPostProcess.h"
#include "BlurPostProcess.h"

namespace ma
{
	HDRPostProcess::HDRPostProcess(Texture* pInputTex, Texture* pOutputTex)
		:PostProcess(pInputTex,pOutputTex)
	{
		memset(m_lumTexs, 0, sizeof(m_lumTexs) ); 
		memset(m_DownSampleTex, 0, sizeof(m_DownSampleTex) );
		memset(m_GlowTex, 0, sizeof(m_GlowTex) );
		//m_GlowMergedTex = NULL;
		memset(m_AdaptedTex, 0, sizeof(m_AdaptedTex) );
	}

	HDRPostProcess::~HDRPostProcess()
	{
	}

	void HDRPostProcess::Init()
	{
		CreateRTtexture();

		LoadShader();
	}

	void HDRPostProcess::Shutdown()
	{

	}

	void HDRPostProcess::CreateRTtexture()
	{
		int len = 1;
		for (int i = NUM_SUM_LUM + 1 - 1; i >= 0; --i)
		{
			m_lumTexs[i] =  GetRenderSystem()->CreateRenderTarget( len, len, FMT_R16F);
			len *= 4;
		}

		int width = 0;
		int height = 0;
		Platform::GetInstance().GetWindowSize(width,height);

		for (int i = 0; i < 3; ++i)
		{
			int nWidth = width / (2 << i);
			int nHeight = height / (2 << i);

			m_DownSampleTex[i] = GetRenderSystem()->CreateRenderTarget(nWidth,nHeight,FMT_A16B16G16R16F);

			m_GlowTex[i] = GetRenderSystem()->CreateRenderTarget(nWidth,nHeight,FMT_A16B16G16R16F);

			m_pBlurPP[i] = new BlurPostProcess(m_DownSampleTex[i],m_GlowTex[i]);
			m_pBlurPP[i]->Init();
		}

		//m_GlowMergedTex = GetRenderSystem()->CreateRenderTarget(width / 2, height / 2,FMT_A16B16G16R16F);

		for (int i = 0; i < AdaptedTex_NUM; ++i)
		{
			m_AdaptedTex[i] = GetRenderSystem()->CreateRenderTarget( 1, 1,FMT_R32F);
		}

	}

	void HDRPostProcess::LoadShader()
	{
		m_SumLogTech = new Technique("SumLuminanceLog","Screen","HDR/SumLuminanceLog",NULL); 
		m_SumLumIterativeTech = new Technique("SumLuminanceIterative","Screen","HDR/SumLuminanceIterative",NULL);
		m_SumLumFinal = new Technique("SumLuminanceFinal","Screen","HDR/SumLuminanceFinal",NULL);

		m_AdaptedLumTech = new Technique("AdaptedLum","Screen","HDR/AdaptedLum",NULL); 
		m_BrightPassTech = new Technique("BrightPass","Screen","HDR/BrightPass",NULL); 
		m_DownSamplersTech = new Technique("DownSample","Screen","HDR/DownSample",NULL); 
		//m_GlowMergeTech = new Technique("GlowMerge","Screen","HDR/GlowMerge",NULL); 
		m_ToneMappingTech = new Technique("ToneMaping","Screen","HDR/ToneMaping",NULL);
	}


	void HDRPostProcess::Render()
	{
		RENDER_PROFILE(HDRPostProcess);

		MeasureLuminance();

		AdaptedLum();

		BrightPassDownSample();

		BloomPass();

		ToneMapping();
	}

	void HDRPostProcess::MeasureLuminance()
	{
		RENDER_PROFILE(MeasureLuminance);

		GetRenderSystem()->SetRenderTarget(m_lumTexs[0]);
		//GetRenderSystem()->SetViewPort(Rectangle(0,0,m_lumTexs[0]->getWidth(),m_lumTexs[0]->getHeight()))

		GetRenderSystem()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

		Vector4 texCoordOffset[2];
		GetSampleOffsets2x2(m_pInputTex->getWidth(), m_pInputTex->getHeight(), texCoordOffset);

		m_SumLogTech->GetParameter("g_SamplerSrc")->setTexture(m_pInputTex);
		m_SumLogTech->GetParameter("tex_coord_offset")->setVector4Array(texCoordOffset,2); 

		ScreenQuad::Render(m_SumLogTech);

		//GetRenderSystem()->PopRenderTargert();

		for (int i = 1; i < NUM_SUM_LUM; ++i)
		{
			GetRenderSystem()->SetRenderTarget(m_lumTexs[i]);

			GetRenderSystem()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);
			//GetRenderSystem()->SetViewPort(Rectangle(0,0,m_lumTexs[i]->getWidth(),m_lumTexs[i]->getHeight()));

			GetSampleOffsets2x2(m_lumTexs[i -1]->getWidth(), m_lumTexs[i -1]->getHeight(), texCoordOffset);

			m_SumLumIterativeTech->GetParameter("g_SamplerSrc")->setTexture(m_lumTexs[i -1]);
			m_SumLumIterativeTech->GetParameter("tex_coord_offset")->setVector4Array(texCoordOffset,2);

			ScreenQuad::Render(m_SumLumIterativeTech);

			//GetRenderSystem()->PopRenderTargert();
		}

		GetRenderSystem()->SetRenderTarget(m_lumTexs[NUM_SUM_LUM]);
		
		GetRenderSystem()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);
		//GetRenderSystem()->SetViewPort(Rectangle(0,0,m_lumTexs[NUM_SUM_LUM]->getWidth(),m_lumTexs[NUM_SUM_LUM]->getHeight()));
		
		GetSampleOffsets2x2(m_lumTexs[NUM_SUM_LUM -1]->getWidth(), m_lumTexs[NUM_SUM_LUM -1]->getHeight(), texCoordOffset);
		
		m_SumLumFinal->GetParameter("g_SamplerSrc")->setTexture(m_lumTexs[NUM_SUM_LUM -1]);
		m_SumLumFinal->GetParameter("tex_coord_offset")->setVector4Array(texCoordOffset,2);

		ScreenQuad::Render(m_SumLumFinal);

		//GetRenderSystem()->PopRenderTargert();
	}

	void HDRPostProcess::AdaptedLum()
	{
		RENDER_PROFILE(AdaptedLum);

		// Swap current & last luminance
		Texture* pTexSwap = m_AdaptedTex[AdaptedTex_LAST];
		m_AdaptedTex[AdaptedTex_LAST] = m_AdaptedTex[AdaptedTex_CUR];
		m_AdaptedTex[AdaptedTex_CUR] = pTexSwap;	

		GetRenderSystem()->SetRenderTarget(m_AdaptedTex[AdaptedTex_CUR]);

		GetRenderSystem()->ClearBuffer(true,true,true,Color(0,0,0,0), 1.0f, 0);

		m_AdaptedLumTech->GetParameter("g_SamplerSrc")->setTexture(m_lumTexs[NUM_SUM_LUM]);
		m_AdaptedLumTech->GetParameter("g_SamplerLastLum")->setTexture(m_AdaptedTex[AdaptedTex_LAST]);
		m_AdaptedLumTech->GetParameter("frame_delta")->setFloat( GetTimer()->GetFrameDeltaTime() );

		ScreenQuad::Render(m_AdaptedLumTech);

		//GetRenderSystem()->PopRenderTargert();
	}

	void HDRPostProcess::BrightPassDownSample()
	{
		RENDER_PROFILE(BrightPassDownSample);

		GetRenderSystem()->SetRenderTarget(m_DownSampleTex[0]);

		GetRenderSystem()->ClearBuffer(true,true,true,Color(0,0,0,0),1.0f,0);

		m_BrightPassTech->GetParameter("g_SamplerSrc")->setTexture(m_pInputTex);
		//m_BrightPassTech->GetParameter("g_SamplerLum")->setTexture(m_AdaptedTex[AdaptedTex_CUR]);

		ScreenQuad::Render(m_BrightPassTech);

		//GetRenderSystem()->PopRenderTargert();

		for (int i = 0; i < 2; ++i)
		{
			GetRenderSystem()->SetRenderTarget(m_DownSampleTex[i + 1]);

			GetRenderSystem()->ClearBuffer(true,true,true,Color(0,0,0,0),1.0f,0);

			m_DownSamplersTech->GetParameter("g_SamplerSrc")->setTexture(m_DownSampleTex[i]);

			ScreenQuad::Render(m_DownSamplersTech);

			//GetRenderSystem()->PopRenderTargert();
		}
	}


	void HDRPostProcess::BloomPass()
	{
		RENDER_PROFILE(BloomPass);

		for (int i = 0; i < 3; ++i)
		{
			m_pBlurPP[i]->Render();
		}

// 		GetRenderSystem()->PushRenderTarget(m_GlowMergedTex);
// 
// 		GetRenderSystem()->ClearBuffer(true,false,false,Color(0,0,0,0),1.0f,0);
// 
// 		m_GlowMergeTech->GetParameter("g_SamplerGlowTex0")->setTexture(m_GlowTex[0]);
// 		m_GlowMergeTech->GetParameter("g_SamplerGlowTex1")->setTexture(m_GlowTex[1]);
// 		m_GlowMergeTech->GetParameter("g_SamplerGlowTex2")->setTexture(m_GlowTex[2]);
// 
// 
// 		ScreenQuad::Render(m_GlowMergeTech);
// 
// 		GetRenderSystem()->PopRenderTargert();

	}

	void HDRPostProcess::ToneMapping()
	{
		RENDER_PROFILE(ToneMapping);

		if (m_pOutputTex)
			GetRenderSystem()->SetRenderTarget(m_pOutputTex);

		m_ToneMappingTech->GetParameter("gTex_Scene")->setTexture(m_pInputTex);
		m_ToneMappingTech->GetParameter("gTex_Lum")->setTexture(m_AdaptedTex[AdaptedTex_CUR]);
		//m_ToneMappingTech->GetParameter("g_SamplerBloom")->setTexture(m_GlowMergedTex);
		m_ToneMappingTech->GetParameter("gTex_Bright0")->setTexture(m_GlowTex[0]);
		m_ToneMappingTech->GetParameter("gTex_Bright1")->setTexture(m_GlowTex[1]);
		m_ToneMappingTech->GetParameter("gTex_Bright2")->setTexture(m_GlowTex[2]);

		m_ToneMappingTech->GetParameter("gExposure")->setFloat(3.0f);
		m_ToneMappingTech->GetParameter("gBloomWeight")->setVector4(Vector4(0.5f,0.3f,0.2f,0));

		ScreenQuad::Render(m_ToneMappingTech);

		//if (m_pOutputTex)
		//	GetRenderSystem()->PopRenderTargert();
	}

	void HDRPostProcess::GetSampleOffsets2x2(int width, int height, Vector4 texCoordOffSet[])
	{
		float const tu = 1.0f / width;
 		float const tv = 1.0f / height;

		texCoordOffSet[0] = Vector4(-tu, -tv, -tu,  tv);
		texCoordOffSet[1] = Vector4(tu,   tv,  tu, -tv);	
	}

}

