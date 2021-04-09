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

		void					AddRenderObj(int stage, Renderable* pRenderObj, Technique* pTech);

		void					AddComputeObj(Renderable* pRenderObj, Technique* pTech);
		
		void					SetCamera(Camera* pCamera);

		void					SetLightViewProj(const Matrix4& matLightViewProj);

		void					SetMainLight(Light* pMainLight, ColourValue cAmbient);

		SceneContext*			GetSceneContext() { return m_renderContext.get(); }

		void					Render(RenderPass* pPass);

		void					Render(RenderPass* pPass, int stageBegin, int stageEnd);

		void					Render(RenderCommand* pRenderCommand);

		void					Render(RenderCommand* pRenderCommand, int stageBegin, int stageEnd);

		void					Compute();

		void					Clear();

	private:

		typedef std::vector<RenderItem> VEC_RENDERABLE;

		VEC_RENDERABLE			m_vecRenderList[RL_Count];

		VEC_RENDERABLE			m_vecCompute;

		RefPtr<SceneContext>	m_renderContext;
	};
}

