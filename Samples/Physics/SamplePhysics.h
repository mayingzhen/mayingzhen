#ifndef _Sample_Physics_H_
#define _Sample_Physics_H_

namespace ma
{
	class Scene;

	class SamplePhysics : public SimpleSceneView
	{
	public:
		SamplePhysics();

		virtual void Init(Application* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

		virtual	void OnResize(int w,int h);

	private:
		void LoadSkelMesh(FBXImporter& fbxImpor);

		void LoadSaticMesh(FBXImporter& fbxImpor);

	private:
		Scene*	m_pScene;	
	};
}


#endif

