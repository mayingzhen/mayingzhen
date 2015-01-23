#ifndef  _ShadingPass__H__
#define  _ShadingPass__H__

namespace ma
{
	class HDRPostProcess;

	class ShadingPass : public RenderPass
	{
	public:
		ShadingPass(Scene* pScene);
		
		void	Init();

		void	Render();

		void	ShoutDown();

	private:
		void	RenderObjecList();

	protected:
		RefPtr<Texture>		m_pShadingTex;

		HDRPostProcess*		m_pHdrPostprocess;

		bool				m_bIsHDRRending;
	};

}


#endif
