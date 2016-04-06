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
	
		void	Reset();

		void	Render();

		void	Shoutdown();

		void	SetHDREnabled(bool b);
		bool	GetHDREnabled() const;

	private:
		void	RenderObjecList();

	protected:
		RefPtr<Texture>			m_pShadingTex;

		RefPtr<HDRPostProcess>	m_pHdrPostprocess;
	};

}


#endif
