#pragma once

namespace ma
{
	class ScreenQuad
	{
	public:
		static void Init();
		
		static void Shoutdown();

		static void Render(Technique* pTechnique, RenderCommand* pCommand, SceneContext* sc);

		static Renderable* GetRenderable();

		static RefPtr<Renderable> CreateRenderable();
	};

}

