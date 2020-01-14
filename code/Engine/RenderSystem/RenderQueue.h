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

	struct LightInfo
	{
		uint32_t m_eType;
		ColourValue	m_cLightColor;
		float	m_fLightIntensity;
		Vector3	m_vDir;
		Vector3 m_vPos;
		float m_fRadius = 0.0f;
		RefPtr<Technique> m_pTech;
	};

	class RenderQueue : public Referenced
	{
	public:
		RenderQueue();

		~RenderQueue();

		void					AddRenderObj(int stage, Renderable* pRenderObj, Technique* pTech);

		void					AddComputeObj(Renderable* pRenderObj, Technique* pTech);
		
		void					SetCamera(Camera* pCamera);

		void					SetMainLight(Light* pMainLight, ColourValue cAmbient);

		void					AddLight(Light* pLight, Technique* pTech);

		SceneContext*			GetSceneContext() { return m_renderContext.get(); }

		std::vector< LightInfo >& GetRenderLights() { return m_vecLight; }

		void					Render(RenderPass* pPass);

		void					Render(RenderPass* pPass, int stageBegin, int stageEnd);

		void					Compute();

		void					Clear();

	private:

		std::map<int, BatchRenderable> m_mapRenderList;

		RefPtr<SceneContext> m_renderContext;

		// Light
		std::vector< LightInfo > m_vecLight;

		BatchRenderable		m_compute;
	};
}

