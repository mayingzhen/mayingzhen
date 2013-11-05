#include "BlurPostProcess.h"

namespace ma
{
	BlurPostProcess::BlurPostProcess(Texture* pInputTex, Texture* pOutputTex)
		:PostProcess(pInputTex,pOutputTex)
	{

	}

	BlurPostProcess::~BlurPostProcess()
	{

	}

	void BlurPostProcess::Init()
	{
		m_BlurXTech = new Technique("BlurX","Blur/Blur","Blur/Blur","BLUR_X 1"); 
		m_BlurYTech = new Technique("BlurY","Blur/Blur","Blur/Blur","BLUR_Y 1");

		int nWidth = m_pInputTex->getWidth();
		int nHeight = m_pInputTex->getHeight();
		FORMAT eformat = m_pInputTex->getFormat();
		m_BureTempTex = GetRenderSystem()->CreateRenderTarget(nWidth,nHeight,eformat);
	}

	void BlurPostProcess::Shutdown()
	{
		SAFE_DELETE(m_BlurXTech);
		SAFE_DELETE(m_BlurYTech);
	}

	void BlurPostProcess::Render()
	{
		RENDER_PROFILE(BlurPostProcess);

		/// x
		GetRenderSystem()->PushRenderTarget(m_BureTempTex);

		GetRenderSystem()->ClearBuffer(true,true,true,Color(0,0,0,0),1.0f,0);

		float width = m_pInputTex->getWidth();
		float heigh = m_pInputTex->getWidth();

		Vector2 texSize = Vector2(width, 1.0f / width);

		std::vector<float> color_weight;
		std::vector<float> tex_coord_offset;
		CalSampleOffsetsGauss(8,1,width,3.0f,color_weight,tex_coord_offset);

		m_BlurXTech->GetParameter("color_weight")->setFloatArray( &(color_weight[0]), 8 );
		m_BlurXTech->GetParameter("tex_coord_offset")->setFloatArray( &(tex_coord_offset[0]), 8 );
		m_BlurXTech->GetParameter("src_tex_size")->setFloatArray( texSize, 2 );

		m_BlurXTech->GetParameter("g_SamplerSrc")->setTexture(m_pInputTex);
	
		ScreenQuad::Render(m_BlurXTech);

		GetRenderSystem()->PopRenderTargert();

		/// y
		GetRenderSystem()->PushRenderTarget(m_pOutputTex);

		GetRenderSystem()->ClearBuffer(true,true,true,Color(0,0,0,0),1.0f,0);

		texSize = Vector2(heigh, 1.0f / heigh);
		CalSampleOffsetsGauss(8,1,heigh,3.0f,color_weight,tex_coord_offset);

		m_BlurYTech->GetParameter("color_weight")->setFloatArray( &(color_weight[0]), 8 );
		m_BlurYTech->GetParameter("tex_coord_offset")->setFloatArray( &(tex_coord_offset[0]), 8 );
		m_BlurYTech->GetParameter("src_tex_size")->setFloatArray( texSize, 2 );

		m_BlurYTech->GetParameter("g_SamplerSrc")->setTexture(m_BureTempTex);

		ScreenQuad::Render(m_BlurYTech);

		GetRenderSystem()->PopRenderTargert();
	}


	float BlurPostProcess::GaussianDistribution(float x, float y, float rho)
	{
		float g = 1.0f / sqrtf( 2.0f * PI * rho * rho );
		g *= expf( -( x * x + y * y ) / ( 2 * rho * rho ) );

		return g;
	}

	void BlurPostProcess::CalSampleOffsetsGauss(int kernelRaidius, float multiplier,
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
}