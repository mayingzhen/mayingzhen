#ifndef _BlurPostProcess__H__
#define _BlurPostProcess__H__

namespace ma
{
	class BlurPostProcess : public PostProcess
	{
	public:
		BlurPostProcess();

		~BlurPostProcess();

		virtual void		Init();

		virtual void		Reset(Texture* pInput, Texture* pOutput);

		virtual void		Shutdown();

		virtual void		Render();

	private:
		float				GaussianDistribution(float x, float y, float rho);

		void				CalSampleOffsetsGauss(int kernelRaidius, float multiplier,
			int texSize, float deviation,
			std::vector<float>& color_weight,
			std::vector<float>& tex_coord_offset);

	private:
		RefPtr<Technique>		m_pBlurXTech;

		RefPtr<Technique>		m_pBlurYTech;

		RefPtr<Texture>			m_pBureTempTex;
		RefPtr<SamplerState>	m_pBureTempSampler;

		RefPtr<SamplerState>	m_InputSampler;

		std::vector<float>		m_color_weightX;
		std::vector<float>		m_tex_coord_offsetX;

		std::vector<float>		m_color_weightY;
		std::vector<float>		m_tex_coord_offsetY;
	};
}



#endif

