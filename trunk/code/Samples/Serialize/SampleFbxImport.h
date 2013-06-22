#ifndef _Sample_FbxImport_H_
#define _Sample_FbxImport_H_

namespace ma
{
	class AnimationPlay;
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
		AnimationPlay*	m_pAnimtionPlay;
		Skeleton*		m_pSkeleton;
		RenderMesh*		m_pRenderMesh;
		//Texture*		m_pRendTexture;
		//Material*		m_pRenderMaterial;
	
		// staticMesh
		RenderMesh*		m_pStaticMesh;
		//Texture*		m_pStatcMeshTexture;
		//Material*		m_pStaticMeshMaterial;

		RenderMesh*		m_pBoxMesh;
		//Texture*		m_pBoxTexture;
		Material*       m_pBoxMaterial;

		ParticleEmitter* m_pEmitter;
	};
}


#endif

