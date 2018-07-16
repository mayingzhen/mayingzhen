#pragma once

namespace ma
{
	class ScreenQuad
	{
	public:
		static void Init();
		
		static void Shoutdown();

		static void Render(Technique* pTechnique, RenderCommand* pCommand);

		static Renderable* GetRenderable();
	};

}

