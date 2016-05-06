#pragma once

namespace ma
{
	class AlchemyAo : public PostProcess
	{
	public:
		AlchemyAo();

		~AlchemyAo();

		virtual void	Init();

		virtual void	Reset(Texture* pInput, Texture* pOutput);

		virtual void	Shutdown();

		virtual void	Render();

	private:
		BlurPostProcess mBlurH;
		BlurPostProcess mBlurV;

		RefPtr<Technique> m_pAlchemyAo;

		RefPtr<Texture> mTexSSAO;
		RefPtr<Texture> mTexBlur0;
		RefPtr<Texture> mTexBlur1;

	};

}