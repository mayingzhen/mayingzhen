#pragma once

namespace ma
{
	class SceneNode;
	class IPhysicsScene;
	class Camera;
	class MeshComponent;
	class CullTree;
	class RenderComponent;
	class Texture;

	class RenderView : public Referenced
	{

	public:

		RenderView() {}

		virtual ~RenderView() {}
	
		virtual void Update() {}

		virtual void Render() {}

	public:

		RefPtr<Camera>	m_pCamera;

		RefPtr<Scene>	m_pScene;

		std::string		m_name;

		std::shared_ptr<SceneContext>	m_pSceneproxy;
	};

	class MainRenderView : public RenderView
	{
	public:
		
		MainRenderView();

		~MainRenderView();

		void Update() override;

		void Render() override;

		void AddRenderStep(RenderStep* step) { m_vecRenderStep.push_back(step); }

	protected:

		typedef std::vector< RefPtr<RenderStep> > VEC_RENDERSTEP;
		VEC_RENDERSTEP			m_vecRenderStep;

		typedef std::vector<RenderProxy*> VEC_RENDERPROXY;
		VEC_RENDERPROXY			m_arrRenderProxy;
	};

	class ShadowMapRenderView : public RenderView
	{

	public:

		Rectangle m_veiwPort;
	};

	class CSMShadowMapRenderView : public RenderView
	{

	public:
		void Update();

		void Render();
	};

	class RenderPipeLine
	{

	};


}