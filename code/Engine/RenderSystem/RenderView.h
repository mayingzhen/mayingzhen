#pragma once

namespace ma
{
	class SceneNode;
	class IPhysicsScene;
	class Camera;
	class MeshComponent;
	class CullTree;
	class RenderScheme;
	class RenderComponent;
	class Texture;
	//class RenderQueue;
	class DirectonalLight;

	class RenderView
	{

	public:

		void Update() {}

		void Render() {}

	protected:

		RefPtr<Camera>	m_pCamera;

		RefPtr<Scene>	m_pScene;

		std::string		m_name;

		RefPtr<RenderStep> m_vecRenderStep;
	};

	class MainRenderView : public RenderView
	{

	public:

		void Update();

		void Render();

	protected:

		typedef std::vector< RefPtr<RenderComponent> > VEC_RENDERCOMP;
		VEC_RENDERCOMP			m_arrRenderComp;

		RenderStep* m_pRenderStep;
	};

	class ShadowMapRenderView : public RenderView
	{

	};

	class CSMShadowMapRenderView : public RenderView
	{

	public:
		void Update();

		void Render();
	};
}