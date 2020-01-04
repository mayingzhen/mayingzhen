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
	class RenderContext;

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

		void					Render(RenderPass* pPass);

		void					Render(RenderPass* pPass, int stageBegin, int stageEnd);

		void					Clear();
		
		void					SetCamera(Camera* pCamera);

		void					AddLight(Light* pLight, Technique* pTech);

		RenderContext*			GetRenderContext() { return m_renderContext.get(); }

		std::vector< LightInfo >& GetRenderLights() { return m_vecLight; }

	private:

		std::map<int, BatchRenderable> m_mapRenderList;

		RefPtr<RenderContext> m_renderContext;

		// Light
		std::vector< LightInfo > m_vecLight;

	};
}

