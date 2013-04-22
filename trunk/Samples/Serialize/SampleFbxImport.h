#ifndef _Sample_FbxImport_H_
#define _Sample_FbxImport_H_

namespace ma
{
	class AnimationPlay;
	class Skeleton;
	class IRendMesh;
	class IRendTexture;
	class FBXImporter;

	class SampleFbxImport : public SimpleSceneView
	{
	public:
		SampleFbxImport();

		virtual void Init(Application* pApplication);

		virtual void Shutdown();

		virtual void Load();

		virtual void Unload();

		virtual void Tick(float timeElapsed);

		virtual void Render();

	private:
		void LoadSkelMesh(FBXImporter& fbxImpor);

		void LoadSaticMesh(FBXImporter& fbxImpor);

	private:

		// SkelMesh
		AnimationPlay*	m_pAnimtionPlay;
		Skeleton*		m_pSkeleton;
		IRendMesh*		m_pRenderMesh;
		IRendTexture*	m_pRendTexture;
	
		// staticMesh
		IRendMesh*		m_pStaticMesh;
		IRendTexture*	m_pStatcMeshTexture;
	};
}


#endif

