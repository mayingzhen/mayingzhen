#pragma once

#include "BatchRenderable.h"

namespace ma
{
	class Renderable;
	class RenderPass;
	class Technique;

	class RenderQueue
	{
	public:
		RenderQueue();

		~RenderQueue();

		void					AddRenderObj(int stage, Renderable* pRenderObj, Technique* pTech);

		void					Render(RenderPass* pPass, int stageBegin, int stageEnd);

		void					Clear();
	
	private:

		std::map<int, BatchRenderable> m_mapRenderList;

	};
}

