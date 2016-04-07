#ifndef _HDR_POST_PROCESS__H__
#define _HDR_POST_PROCESS__H__

#include "PostProcess.h"

namespace ma
{
	class BlurPostProcess;

	class HDRPostProcess : public PostProcess
	{
	public:
		HDRPostProcess();

		~HDRPostProcess();

		void	Init();

		void	Reset(Texture* pInput, Texture* pOutput);

		void	Shoutdown();

		void	Render();

	private:
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

		RefPtr<Texture>		m_lumTexs[NUM_SUM_LUM + 1]; 
		RefPtr<Texture>		m_DownSampleTex[NUM_DownSamplers + 1];
		RefPtr<Texture>		m_GlowTex[NUM_DownSamplers + 1];
		RefPtr<Texture>		m_AdaptedTex[AdaptedTex_NUM];

		RefPtr<BlurPostProcess>	m_pBlurPP[NUM_DownSamplers + 1];

		RefPtr<Technique>	m_SumLogTech;
		RefPtr<Technique>	m_SumLumIterativeTech[NUM_SUM_LUM];
		RefPtr<Technique>	m_SumLumFinal;

		RefPtr<Technique>	m_AdaptedLumTech;
		RefPtr<Technique>	m_BrightPassTech;
		RefPtr<Technique>	m_DownSamplersTech[NUM_DownSamplers];
		RefPtr<Technique>	m_ToneMappingTech;

	};

}



#endif