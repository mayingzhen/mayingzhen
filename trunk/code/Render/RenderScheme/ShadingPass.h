#ifndef  _ShadingPass__H__
#define  _ShadingPass__H__

namespace ma
{
	class HDRPostProcess;

	class ShadingPass : public RenderPass
	{
		DECL_OBJECT(ShadingPass)

	public:
		void	Init();

		void	Render();

		void	ShoutDown();

	private:
		void	RenderObjecList();

	protected:
		Texture*			m_pShadingTex;

		HDRPostProcess*		m_pHdrPostprocess;
	};

}


#endif
