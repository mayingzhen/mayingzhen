#include "HDRPostProcess.h"
#include "BlurPostProcess.h"

namespace ma
{
	HDRPostProcess::HDRPostProcess()
	{
	}

	HDRPostProcess::~HDRPostProcess()
	{
	}

	void HDRPostProcess::Init()
	{
		m_SumLogTech = CreateTechnique("SumLuminanceLog","Screen","HDR/SumLuminanceLog",NULL); 

		for (UINT i = 0; i < NUM_SUM_LUM; ++i)
		{
			m_SumLumIterativeTech[i] = CreateTechnique("SumLuminanceIterative","Screen","HDR/SumLuminanceIterative",NULL);
		}

		m_SumLumFinal = CreateTechnique("SumLuminanceFinal","Screen","HDR/SumLuminanceFinal",NULL);

		m_AdaptedLumTech = CreateTechnique("AdaptedLum","Screen","HDR/AdaptedLum",NULL); 	

		m_BrightPassTech = CreateTechnique("BrightPass","Screen","HDR/BrightPass",NULL); 

		for (UINT i = 0; i < NUM_DownSamplers; ++i)
		{
			m_DownSamplersTech[i] = CreateTechnique("DownSample","Screen","HDR/DownSample",NULL); 
		}

		m_ToneMappingTech = CreateTechnique("ToneMaping","Screen","HDR/ToneMaping",NULL);
	}

	void HDRPostProcess::Reset(Texture* pInput, Texture* pOutput)
	{
		m_pInputTex = pInput;
		m_pOutputTex = pOutput;

		int len = 1;
		for (int i = NUM_SUM_LUM + 1 - 1; i >= 0; --i)
		{
			m_lumTexs[i] =  GetRenderSystem()->CreateRenderTarget( len, len, PF_FLOAT16_R);
			len *= 4;
		}

		//Rectangle rect = m_pScene->GetViewport();

		for (int i = 0; i < NUM_DownSamplers + 1; ++i)
		{
			//int nWidth = (int)rect.width() / (2 << i);
			//int nHeight = (int)rect.height() / (2 << i);

			//m_DownSampleTex[i] = GetRenderSystem()->CreateRenderTexture(nWidth,nHeight,PF_FLOAT16_RGBA);

			//m_GlowTex[i] = GetRenderSystem()->CreateRenderTexture(nWidth,nHeight,PF_FLOAT16_RGBA);

			//m_pBlurPP[i] = new BlurPostProcess(m_DownSampleTex[i],m_GlowTex[i]);
			//m_pBlurPP[i]->Init();
		}

// 		for (int i = 0; i < AdaptedTex_NUM; ++i)
// 		{
// 			m_AdaptedTex[i] = GetRenderSystem()->CreateRenderTarget( 1, 1,PF_FLOAT32_R);
// 		}



// 		std::vector<Vector4> texCoordOffset;
// 		GetSampleOffsets2x2(m_pInputTex->GetWidth(), m_pInputTex->GetHeight(), texCoordOffset);
// 
// 		m_SumLogTech->SetParameter("g_SamplerSrc",Any(m_pInputTex));
// 		m_SumLogTech->SetParameter("tex_coord_offset",Any(texCoordOffset)); 
// 
// 		for (UINT i = 0; i < NUM_SUM_LUM; ++i)
// 		{
// 			GetSampleOffsets2x2(m_lumTexs[i -1]->GetWidth(), m_lumTexs[i -1]->GetHeight(), texCoordOffset);
// 
// 			m_SumLumIterativeTech[i]->SetParameter("g_SamplerSrc",Any(m_lumTexs[i -1]));
// 			m_SumLumIterativeTech[i]->SetParameter("tex_coord_offset",Any(texCoordOffset));
// 		}
// 
// 		GetSampleOffsets2x2(m_lumTexs[NUM_SUM_LUM -1]->GetWidth(), m_lumTexs[NUM_SUM_LUM -1]->GetHeight(), texCoordOffset);	
// 
// 		m_SumLumFinal->SetParameter("g_SamplerSrc",Any(m_lumTexs[NUM_SUM_LUM -1]));
// 		m_SumLumFinal->SetParameter("tex_coord_offset",Any(texCoordOffset));
// 
// 		m_AdaptedLumTech->SetParameter("g_SamplerSrc",Any(m_lumTexs[NUM_SUM_LUM]));
// 		m_AdaptedLumTech->SetParameter("g_SamplerLastLum",Any(m_AdaptedTex[AdaptedTex_LAST]));
// 		//m_AdaptedLumTech->GetParameter("frame_delta")->BindMethod( GetTimer(), &Time::GetFrameDeltaTime );	
// 
// 		m_BrightPassTech->SetParameter("g_SamplerSrc",Any(m_pInputTex));
// 
// 		for (UINT i = 0; i < NUM_DownSamplers; ++i)
// 		{
// 			m_DownSamplersTech[i]->SetParameter("g_SamplerSrc",Any(m_DownSampleTex[i]));
// 		}
// 
// 		m_ToneMappingTech->SetParameter("gTex_Scene",Any(m_pInputTex));
// 		m_ToneMappingTech->SetParameter("gTex_Lum",Any(m_AdaptedTex[AdaptedTex_CUR]));
// 		m_ToneMappingTech->SetParameter("gTex_Bright0",Any(m_GlowTex[0]));
// 		m_ToneMappingTech->SetParameter("gTex_Bright1",Any(m_GlowTex[1]));
// 		m_ToneMappingTech->SetParameter("gTex_Bright2",Any(m_GlowTex[2]));
// 		m_ToneMappingTech->SetParameter("gExposure",Any(3.0f));
// 		m_ToneMappingTech->SetParameter("gBloomWeight",Any(Vector4(0.5f,0.3f,0.2f,0)));
	}

	void HDRPostProcess::Shoutdown()
	{

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

// 		GetRenderSystem()->SetRenderTarget(0,m_lumTexs[0].get());
// 
// 		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White, 1.0f, 0);

		ScreenQuad::Render(m_SumLogTech.get());

		for (int i = 1; i < NUM_SUM_LUM; ++i)
		{
			//GetRenderSystem()->SetRenderTarget(0,m_lumTexs[i].get());

			//GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White, 1.0f, 0);

			ScreenQuad::Render(m_SumLumIterativeTech[i].get());
		}

		//GetRenderSystem()->SetRenderTarget(0,m_lumTexs[NUM_SUM_LUM].get());
		
		//GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White, 1.0f, 0);

		ScreenQuad::Render(m_SumLumFinal.get());
	}

	void HDRPostProcess::AdaptedLum()
	{
		RENDER_PROFILE(AdaptedLum);

		// Swap current & last luminance
		RefPtr<Texture> pTexSwap = m_AdaptedTex[AdaptedTex_LAST];
		m_AdaptedTex[AdaptedTex_LAST] = m_AdaptedTex[AdaptedTex_CUR];
		m_AdaptedTex[AdaptedTex_CUR] = pTexSwap;	

		//GetRenderSystem()->SetRenderTarget(0,m_AdaptedTex[AdaptedTex_CUR].get());

		//GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White, 1.0f, 0);

		ScreenQuad::Render(m_AdaptedLumTech.get());

	}

	void HDRPostProcess::BrightPassDownSample()
	{
		RENDER_PROFILE(BrightPassDownSample);

		//GetRenderSystem()->SetRenderTarget(0,m_DownSampleTex[0].get());

		//GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White,1.0f,0);

		ScreenQuad::Render(m_BrightPassTech.get());

		for (int i = 0; i < NUM_DownSamplers; ++i)
		{
			//GetRenderSystem()->SetRenderTarget(0,m_DownSampleTex[i + 1].get());

			//GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White,1.0f,0);

			ScreenQuad::Render(m_DownSamplersTech[i].get());
		}
	}


	void HDRPostProcess::BloomPass()
	{
		RENDER_PROFILE(BloomPass);

		for (int i = 0; i < 3; ++i)
		{
			m_pBlurPP[i]->Render();
		}
	}

	void HDRPostProcess::ToneMapping()
	{
		RENDER_PROFILE(ToneMapping);

// 		if (m_pOutputTex)
// 			GetRenderSystem()->SetRenderTarget(0,m_pOutputTex.get());

		ScreenQuad::Render(m_ToneMappingTech.get());
	}

	void HDRPostProcess::GetSampleOffsets2x2(int width, int height, std::vector<Vector4>& texCoordOffSet)
	{
		float const tu = 1.0f / width;
 		float const tv = 1.0f / height;

		texCoordOffSet.resize(2);
		texCoordOffSet[0] = Vector4(-tu, -tv, -tu,  tv);
		texCoordOffSet[1] = Vector4(tu,   tv,  tu, -tv);	
	}

}

