#ifndef _Sample_FbxImport_H_
#define _Sample_FbxImport_H_

namespace ma
{
	class AnimationObject;
	class Skeleton;
	class MeshData;
	class Texture;

	class SampleFbxImport : public Sample
	{
	public:
		SampleFbxImport();

		virtual void Load();

		virtual void UnLoad();

	private:
		void LoadSkelMesh();

		void LoadSaticMesh();

	};
}


#endif
