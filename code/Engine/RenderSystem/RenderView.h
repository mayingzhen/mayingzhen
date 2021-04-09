#pragma once

namespace ma
{
	class SceneNode;
	class IPhysicsScene;
	class Camera;
	class MeshComponent;
	class CullTree;
	class MainRenderStep;
	class RenderComponent;
	class Texture;
	class DirectonalLight;

	class LightProcess : public Referenced
	{
	public:
		LightProcess();

		void	AddLightObj(LightProxy* pLight) { m_vecLight.push_back(pLight); }

		void	Clear() { m_vecLight.clear(); }

		void	Render();


	private:
		typedef std::vector< RefPtr<LightProxy> > VEC_LIGHTPROXY;

		VEC_LIGHTPROXY			m_vecLight;

		RefPtr<Technique>		m_pAmbientLight;

		RefPtr<RenderStep>		m_pLightStep;
	};

	class RenderView : public Referenced
	{

	public:

		RenderView() {}

		virtual ~RenderView() {}
	
		void Update() {}

		virtual void Render() {}

	public:

		RefPtr<Camera>	m_pCamera;

		RefPtr<Scene>	m_pScene;

		std::string		m_name;
	};

	class MainRenderView : public RenderView
	{
	public:
		
		MainRenderView();

		~MainRenderView();

		void Update();

		void Render() override;

	protected:

		typedef std::vector<RenderProxy*> VEC_RENDERPROXY;
		VEC_RENDERPROXY			m_arrRenderProxy;

		std::vector< RefPtr<RenderStep> > m_vecRenderStep;

		RefPtr<LightProcess>	m_pLigtProcess;
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