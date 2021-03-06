#pragma once

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
	class RenderCommand;

	struct RenderItem
	{
		RenderItem(Renderable* renderable, Technique* tech)
		{
			m_renderable = renderable;
			m_tech = tech;
		}

		RenderItem()
		{

		}

		Renderable* m_renderable = nullptr;
		Technique* m_tech = nullptr;
	};

	class RenderQueue : public Referenced
	{
	public:
		RenderQueue();

		~RenderQueue();

		void					AddRenderObj(Renderable* pRenderObj, Technique* pTech);

		void					AddComputeObj(Renderable* pRenderObj, Technique* pTech);

		void					Render(RenderPass* pPass, SceneContext* sc);

		void					Render(RenderCommand* pCommand, SceneContext* sc);

		void					Compute(SceneContext* sc);

		void					Clear();

	private:
		void					ParallelRender(RenderCommand* pCommand, SceneContext* sc, RenderItem* pNodeStart, uint32_t nNodeCount);

	private:

		typedef std::vector<RenderItem> VEC_RENDERABLE;

		std::array<std::vector<RenderItem>,RL_Count>	m_vecRenderList;

		VEC_RENDERABLE			m_vecCompute;
	};
}

