#pragma once

namespace ma
{
	class LinearizeDepth : public PostProcess
	{
	public:
		LinearizeDepth();

		~LinearizeDepth();

		virtual void	Init();

		virtual void	Reset(Texture* pInput, Texture* pOutput);

		virtual void	Shutdown();

		virtual void	Render();

	};

}