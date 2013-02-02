#ifndef __S3XResourceImporter_H__
#define __S3XResourceImporter_H__

namespace ma
{
	class MeshData;
	class SkeletonData;
	class AnimationData

	S3ResourceImport_API MeshData*		LoadMeshData(const char* filename);

	S3ResourceImport_API SkeletonData*	LoadSkeletonData(const char* fileName);

	S3ResourceImport_API AnimationData* LoadAnimationData(const char* fileName);

};


#endif


