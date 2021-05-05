#pragma once

#include "BatchRenderable.h"
#include "RenderContext.h"

namespace ma
{
	class Renderable;
	class RenderPass;
	class Technique;
	class Camera;
	class Light;
	class SceneContext;
	class LightProxy;

	class RenderQueue : public Referenced
	{
	public:
		RenderQueue();

		~RenderQueue();

		void					AddRenderObj(Renderable* pRenderObj, Technique* pTech);

		void					AddComputeObj(Renderable* pRenderObj, Technique* pTech);

		void					Render(RenderPass* pPass);

		void					Compute();

		void					Clear();

	private:
		void					ParallelRender(RenderCommand* pCommand, RenderItem* pNodeStart, uint32_t nNodeCount);

	private:

		typedef std::vector<RenderItem> VEC_RENDERABLE;

		std::array<std::vector<RenderItem>,RL_Count>	m_vecRenderList;

		VEC_RENDERABLE			m_vecCompute;
	};
}

