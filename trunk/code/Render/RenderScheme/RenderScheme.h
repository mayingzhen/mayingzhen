#ifndef  _RenderScheme__H__
#define  _RenderScheme__H__



namespace ma
{
	class ShadingPass;

	class RENDER_API  RenderScheme
	{
	public:
		RenderScheme();

		void	Init();

		void	ShoutDown();

		void	Render();

		void	AddRenderPass(RenderPass* pPass,bool bFront = false);

	private:
		std::list<RenderPass*>	m_arrRenderPass;
	};

	RENDER_API	RenderScheme*	GetRenderScheme();

	RENDER_API	void			SetRenderScheme(RenderScheme* pRenderScheme);
}

#endif

