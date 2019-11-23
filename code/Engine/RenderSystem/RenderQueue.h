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

		void					AddLight(Light* pLight);

		RenderContext*			GetRenderContext() { return m_renderContext.get(); }

	private:

		std::map<int, BatchRenderable> m_mapRenderList;

		RefPtr<RenderContext> m_renderContext;

		// CameraInfo
		MatViewProj				m_matViewProj;
		float					m_fNear;
		float					m_fFar;
		Vector3					m_vEyeWordPos;

		// Light
		std::vector< RefPtr<Light> > m_vecLight;

	};
}

