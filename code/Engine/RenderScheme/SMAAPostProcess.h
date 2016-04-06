#ifndef _SMAAPostProcess__H__
#define _SMAAPostProcess__H__

namespace ma
{
	class SMAAPostProcess : public PostProcess
	{
	public:
		SMAAPostProcess(Texture* pInputTex, Texture* pOutputTex);

		~SMAAPostProcess();

		virtual void		Init(int nWidth, int nHeight);

		virtual void		Reset(int nWidth, int nHeight);

		virtual void		Shutdown();

		virtual void		Render();

	private:
		RefPtr<Technique> m_pDepthEdgeDetection;
		RefPtr<Technique> m_pColorEdgeDetection;
		RefPtr<Technique> m_pBlendWeightCalculation;
		RefPtr<Technique> m_pNeiborhoodBlending;

		RefPtr<Texture>	m_pTexEdge;
		RefPtr<Texture>	m_pTexBlend;

		RefPtr<Texture> m_pAreadTex;
		RefPtr<Texture> m_pSreachTex;

		bool m_bLinearizeDepthEnabled;
	};
}



#endif

