#ifndef _BlurPostProcess__H__
#define _BlurPostProcess__H__

namespace ma
{
	class BlurPostProcess : public PostProcess
	{
	public:
		BlurPostProcess(Scene* pScene);

		~BlurPostProcess();

		virtual void		Init();

		virtual void		Reset();

		virtual void		Shutdown();

		virtual void		Render();

	private:
		float				GaussianDistribution(float x, float y, float rho);

		void				CalSampleOffsetsGauss(int kernelRaidius, float multiplier,
			int texSize, float deviation,
			std::vector<float>& color_weight,
			std::vector<float>& tex_coord_offset);

	private:
		RefPtr<Technique>		m_BlurXTech;

		RefPtr<Technique>		m_BlurYTech;

		RefPtr<Texture>			m_BureTempTex;
	};
}



#endif

