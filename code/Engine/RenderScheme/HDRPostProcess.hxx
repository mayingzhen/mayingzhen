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
			m_lumTexs[i] =  GetRenderSystem()->CreateRenderTarget( len, len, PF_FLOAT16_R);
			len *= 4;
		}

		Rectangle rect = GetRenderSystem()->GetViewPort();

		for (int i = 0; i < NUM_DownSamplers + 1; ++i)
		{
			int nWidth = (int)rect.width / (2 << i);
			int nHeight = (int)rect.height / (2 << i);

			m_DownSampleTex[i] = GetRenderSystem()->CreateRenderTarget(nWidth,nHeight,PF_FLOAT16_RGBA);

			m_GlowTex[i] = GetRenderSystem()->CreateRenderTarget(nWidth,nHeight,PF_FLOAT16_RGBA);

			m_pBlurPP[i] = new BlurPostProcess(m_DownSampleTex[i],m_GlowTex[i]);
			m_pBlurPP[i]->Init();
		}

		for (int i = 0; i < AdaptedTex_NUM; ++i)
		{
			m_AdaptedTex[i] = GetRenderSystem()->CreateRenderTarget( 1, 1,PF_FLOAT32_R);
		}
	}

	void HDRPostProcess::LoadShader()
	{
		std::vector<Vector4> texCoordOffset;
		GetSampleOffsets2x2(m_pInputTex->GetWidth(), m_pInputTex->GetHeight(), texCoordOffset);

		m_SumLogTech = CreateTechnique("SumLuminanceLog","Screen","HDR/SumLuminanceLog",NULL); 
		m_SumLogTech->SetParameter("g_SamplerSrc",Any(m_pInputTex));
		m_SumLogTech->SetParameter("tex_coord_offset",Any(texCoordOffset)); 

		for (UINT i = 0; i < NUM_SUM_LUM; ++i)
		{
			GetSampleOffsets2x2(m_lumTexs[i -1]->GetWidth(), m_lumTexs[i -1]->GetHeight(), texCoordOffset);

			m_SumLumIterativeTech[i] = CreateTechnique("SumLuminanceIterative","Screen","HDR/SumLuminanceIterative",NULL);
			m_SumLumIterativeTech[i]->SetParameter("g_SamplerSrc",Any(m_lumTexs[i -1]));
			m_SumLumIterativeTech[i]->SetParameter("tex_coord_offset",Any(texCoordOffset));
		}
		
		GetSampleOffsets2x2(m_lumTexs[NUM_SUM_LUM -1]->GetWidth(), m_lumTexs[NUM_SUM_LUM -1]->GetHeight(), texCoordOffset);	
		
		m_SumLumFinal = CreateTechnique("SumLuminanceFinal","Screen","HDR/SumLuminanceFinal",NULL);
		m_SumLumFinal->SetParameter("g_SamplerSrc",Any(m_lumTexs[NUM_SUM_LUM -1]));
		m_SumLumFinal->SetParameter("tex_coord_offset",Any(texCoordOffset));

		m_AdaptedLumTech = CreateTechnique("AdaptedLum","Screen","HDR/AdaptedLum",NULL); 
		m_AdaptedLumTech->SetParameter("g_SamplerSrc",Any(m_lumTexs[NUM_SUM_LUM]));
		m_AdaptedLumTech->SetParameter("g_SamplerLastLum",Any(m_AdaptedTex[AdaptedTex_LAST]));
		m_AdaptedLumTech->GetParameter("frame_delta")->BindMethod( GetTimer(), &Time::GetFrameDeltaTime );	

		m_BrightPassTech = CreateTechnique("BrightPass","Screen","HDR/BrightPass",NULL); 
		m_BrightPassTech->SetParameter("g_SamplerSrc",Any(m_pInputTex));

		for (UINT i = 0; i < NUM_DownSamplers; ++i)
		{
			m_DownSamplersTech[i] = CreateTechnique("DownSample","Screen","HDR/DownSample",NULL); 
			m_DownSamplersTech[i]->SetParameter("g_SamplerSrc",Any(m_DownSampleTex[i]));
		}

		m_ToneMappingTech = CreateTechnique("ToneMaping","Screen","HDR/ToneMaping",NULL);
		m_ToneMappingTech->SetParameter("gTex_Scene",Any(m_pInputTex));
		m_ToneMappingTech->SetParameter("gTex_Lum",Any(m_AdaptedTex[AdaptedTex_CUR]));
		m_ToneMappingTech->SetParameter("gTex_Bright0",Any(m_GlowTex[0]));
		m_ToneMappingTech->SetParameter("gTex_Bright1",Any(m_GlowTex[1]));
		m_ToneMappingTech->SetParameter("gTex_Bright2",Any(m_GlowTex[2]));
		m_ToneMappingTech->SetParameter("gExposure",Any(3.0f));
		m_ToneMappingTech->SetParameter("gBloomWeight",Any(Vector4(0.5f,0.3f,0.2f,0)));
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

		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White, 1.0f, 0);

		ScreenQuad::Render(m_SumLogTech.get());

		for (int i = 1; i < NUM_SUM_LUM; ++i)
		{
			GetRenderSystem()->SetRenderTarget(m_lumTexs[i]);

			GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White, 1.0f, 0);

			ScreenQuad::Render(m_SumLumIterativeTech[i].get());
		}

		GetRenderSystem()->SetRenderTarget(m_lumTexs[NUM_SUM_LUM]);
		
		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White, 1.0f, 0);

		ScreenQuad::Render(m_SumLumFinal.get());
	}

	void HDRPostProcess::AdaptedLum()
	{
		RENDER_PROFILE(AdaptedLum);

		// Swap current & last luminance
		Texture* pTexSwap = m_AdaptedTex[AdaptedTex_LAST];
		m_AdaptedTex[AdaptedTex_LAST] = m_AdaptedTex[AdaptedTex_CUR];
		m_AdaptedTex[AdaptedTex_CUR] = pTexSwap;	

		GetRenderSystem()->SetRenderTarget(m_AdaptedTex[AdaptedTex_CUR]);

		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White, 1.0f, 0);

		ScreenQuad::Render(m_AdaptedLumTech.get());

	}

	void HDRPostProcess::BrightPassDownSample()
	{
		RENDER_PROFILE(BrightPassDownSample);

		GetRenderSystem()->SetRenderTarget(m_DownSampleTex[0]);

		GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White,1.0f,0);

		ScreenQuad::Render(m_BrightPassTech.get());

		for (int i = 0; i < NUM_DownSamplers; ++i)
		{
			GetRenderSystem()->SetRenderTarget(m_DownSampleTex[i + 1]);

			GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White,1.0f,0);

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

		if (m_pOutputTex)
			GetRenderSystem()->SetRenderTarget(m_pOutputTex);

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

