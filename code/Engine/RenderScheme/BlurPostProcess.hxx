#include "BlurPostProcess.h"

namespace ma
{
	BlurPostProcess::BlurPostProcess()
	{

	}

	BlurPostProcess::~BlurPostProcess()
	{
	}

	void BlurPostProcess::Init()
	{
		m_pBlurXTech = CreateTechnique("BlurX","Blur","Blur","BLUR_X 1"); 
		m_pBlurYTech = CreateTechnique("BlurY","Blur","Blur","BLUR_Y 1");
	}

	void BlurPostProcess::Reset(Texture* pInput, Texture* pOutput)
	{
		m_pInputTex = pOutput;
		m_pOutputTex = pOutput;

		m_InputSampler = CreateSamplerState(pInput,CLAMP,TFO_TRILINEAR,false);

		PixelFormat eformat = pInput->GetFormat();
		m_pBureTempTex = GetRenderSystem()->CreateRenderTarget(-1,-1,eformat,false,false);

		m_pBureTempSampler = CreateSamplerState(m_pBureTempTex.get(),CLAMP,TFO_TRILINEAR,false);

		Vector2 texSize = Vector2((float)pInput->GetWidth(), 1.0f / (float)pInput->GetWidth());

		CalSampleOffsetsGauss(8,1,pInput->GetWidth(),3.0f,m_color_weightX,m_tex_coord_offsetX);

		//m_BlurXTech->SetParameter( "color_weight", Any(color_weight) );
		//m_BlurXTech->SetParameter( "tex_coord_offset", Any(tex_coord_offset) );
		m_pBlurXTech->SetParameter( "src_tex_size", Any(texSize) );
		m_pBlurXTech->SetParameter( "g_SamplerSrc", Any(m_InputSampler) );

		texSize = Vector2((float)pInput->GetHeight(), 1.0f / (float)pInput->GetHeight());
		CalSampleOffsetsGauss(8,1,pInput->GetHeight(),3.0f,m_color_weightY,m_tex_coord_offsetY);

		//m_BlurYTech->SetParameter( "color_weight", Any(color_weight) );
		//m_BlurYTech->SetParameter( "tex_coord_offset", Any(tex_coord_offset) );
		m_pBlurYTech->SetParameter( "src_tex_size", Any(texSize) );
		m_pBlurYTech->SetParameter( "g_SamplerSrc", Any(m_pBureTempSampler) );
	}


	void BlurPostProcess::Shutdown()
	{
		m_pBlurXTech = NULL;
		m_pBlurYTech = NULL;
	}

	void BlurPostProcess::Render()
	{
		RENDER_PROFILE(BlurPostProcess);

		/// x
		{
			FrameBuffer fb;
			fb.AttachColor(0,m_pBureTempTex.get());
			fb.AttachDepthStencil(GetRenderSystem()->GetDefaultDepthStencil().get());
			GetRenderSystem()->SetFrameBuffer(&fb);
			GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White,1.0f,0);

			ShaderProgram* pShader = m_pBlurXTech->GetShaderProgram();
			GetRenderSystem()->SetValue( pShader->GetUniform("color_weight"),(Vector4*)(&m_color_weightX[0]),m_color_weightX.size() / 4 );
			GetRenderSystem()->SetValue( pShader->GetUniform("tex_coord_offset"),(Vector4*)(&m_tex_coord_offsetX[0]),m_tex_coord_offsetX.size() / 4 );

			ScreenQuad::Render(m_pBlurXTech.get());

			Texture* pSrc = NULL;
			GetRenderSystem()->SetValue( pShader->GetUniform("g_SamplerSrc"), pSrc );
		}

		/// y
		{
			FrameBuffer fb;
			fb.AttachColor(0,m_pOutputTex.get());
			fb.AttachDepthStencil(GetRenderSystem()->GetDefaultDepthStencil().get());
			GetRenderSystem()->SetFrameBuffer(&fb);
			GetRenderSystem()->ClearBuffer(true,true,true,ColourValue::White,1.0f,0);

			ShaderProgram* pShader = m_pBlurYTech->GetShaderProgram();
			GetRenderSystem()->SetValue( pShader->GetUniform("color_weight"),(Vector4*)(&m_color_weightY[0]),m_color_weightY.size() / 4 );
			GetRenderSystem()->SetValue( pShader->GetUniform("tex_coord_offset"),(Vector4*)(&m_tex_coord_offsetY[0]),m_tex_coord_offsetY.size() / 4 );

			ScreenQuad::Render(m_pBlurYTech.get());
		}

	}


	float BlurPostProcess::GaussianDistribution(float x, float y, float rho)
	{
		float g = 1.0f / sqrtf( 2.0f * Math::PI * rho * rho );
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