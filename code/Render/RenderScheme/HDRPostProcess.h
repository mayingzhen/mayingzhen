#ifndef _HDR_POST_PROCESS__H__
#define _HDR_POST_PROCESS__H__

namespace ma
{
	class BlurPostProcess;

	class HDRPostProcess : public PostProcess
	{
	public:
		HDRPostProcess(Texture* pInputTex, Texture* pOutputTex);

		~HDRPostProcess();

		void	Init();

		void	Shutdown();

		void	Render();

	private:

		void	CreateRTtexture();

		void	LoadShader();

		void	MeasureLuminance();

		void	AdaptedLum();

		void	BrightPassDownSample();

		void	BloomPass();

		void	ToneMapping();

		void	GetSampleOffsets2x2(int width, int height,std::vector<Vector4>& texCoordOffSet);


	public:

		enum
		{
			NUM_SUM_LUM = 3,
			NUM_DownSamplers = 2
		};

		enum
		{
			AdaptedTex_CUR = 0,
			AdaptedTex_LAST = 1,
			AdaptedTex_NUM = 2,
		};

		Texture*			m_lumTexs[NUM_SUM_LUM + 1]; 
		Texture*			m_DownSampleTex[NUM_DownSamplers + 1];
		Texture*			m_GlowTex[NUM_DownSamplers + 1];
		Texture*			m_AdaptedTex[AdaptedTex_NUM];

		BlurPostProcess*	m_pBlurPP[NUM_DownSamplers + 1];

		Technique*			m_SumLogTech;
		Technique*			m_SumLumIterativeTech[NUM_SUM_LUM];
		Technique*			m_SumLumFinal;

		Technique*			m_AdaptedLumTech;
		Technique*			m_BrightPassTech;
		Technique*			m_DownSamplersTech[NUM_DownSamplers];
		Technique*			m_ToneMappingTech;

	};

}



#endif