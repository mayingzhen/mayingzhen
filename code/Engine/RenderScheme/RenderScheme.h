#ifndef  _RenderScheme__H__
#define  _RenderScheme__H__



namespace ma
{
	class ShadingPass;

	class RENDER_API  RenderScheme : public Serializable
	{
	public:
		RenderScheme();

		void	Init();

		void	ShoutDown();

		void	Render();

		void	AddRenderPass(RenderPass* pPass);

		void	Serialize(Serializer& sl, const char* pszLable = "RenderScheme");

	private:
		typedef std::vector< RefPtr<RenderPass> >	VEC_RENDERPASS;
		VEC_RENDERPASS	m_arrRenderPass;
	};

	RefPtr<RenderScheme> CreateRenderScheme();


}

#endif

