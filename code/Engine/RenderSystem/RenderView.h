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
	class DeferredShadow;

	class RenderView : public Referenced
	{

	public:

		RenderView() {}

		virtual ~RenderView() {}
	
		virtual void Update() = 0;

		virtual void Render() = 0;

	public:

		RefPtr<Camera>	m_pCamera;

		RefPtr<Scene>	m_pScene;

		std::string		m_name;

		std::shared_ptr<SceneContext>	m_pSceneproxy;

		uint32_t		m_nRenderOrder = 0;
	};

	class MainRenderView : public RenderView
	{
	public:
		
		MainRenderView();

		~MainRenderView();

		void Update() override;

		void Render() override;

		void AddRenderStep(MainRenderStep* step);

		void AddExternRenderProxy(RenderProxy* pRenderProxy);

		DeferredShadow* GetDeferredShadowSetp();
		void SetDeferredShadowSetp(DeferredShadow* step);

	protected:

		typedef std::vector< RefPtr<MainRenderStep> > VEC_RENDERSTEP;
		VEC_RENDERSTEP			m_vecRenderStep;

		typedef std::vector<RenderProxy*> VEC_RENDERPROXY;
		VEC_RENDERPROXY			m_arrRenderProxy;

		std::vector<RenderProxy*> m_arrExternRenderProxy;

		RefPtr<DeferredShadow>	m_pDeferredShadow;
	};




}