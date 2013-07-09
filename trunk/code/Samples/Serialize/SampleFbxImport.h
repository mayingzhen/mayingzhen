#ifndef _Sample_FbxImport_H_
#define _Sample_FbxImport_H_

namespace ma
{
	class IAnimationPlay;
	class Skeleton;
	class MeshData;
	class Texture;
	class FBXImporter;

	class SampleFbxImport : public Sample
	{
	public:
		SampleFbxImport();

		virtual void Init(const Platform* pPlatform);

		virtual void Update();

		virtual void Render();

	private:
		void LoadSkelMesh(FBXImporter& fbxImpor);

		void LoadSaticMesh(FBXImporter& fbxImpor);

		void LoadBoxMesh(FBXImporter& fbxImpor);

		void LoadParticles();

	private:

		// SkelMesh
		IAnimationPlay*	m_pAnimtionPlay;
		Skeleton*		m_pSkeleton;
		RenderMesh*		m_pRenderMesh;
	
		// staticMesh
		RenderMesh*		m_pStaticMesh;


		RenderMesh*		m_pBoxMesh;
		Material*       m_pBoxMaterial;

		ParticleEmitter* m_pEmitter;
	};
}


#endif

