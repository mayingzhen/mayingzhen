#ifndef __S3ResourceBuilder_H__
#define __S3ResourceBuilder_H__

namespace S3
{
	class MeshData;
	class SkeletonData;
	class AnimationData;

	// Load
	ENGINE_API MeshData*		LoadMeshFromBinaryFile(const char* filename);

	ENGINE_API SkeletonData*	LoadSkeletonFromBinaryFile(const char* fileName);

	ENGINE_API AnimationData*	LoadAnimationFromBinaryFile(const char* fileName);

	// Save
	ENGINE_API bool				SaveMeshToBinaryFile(const char* filename,MeshData* pMeshData);

	ENGINE_API bool				SaveSkeletonToBinaryFile(const char* fileName,SkeletonData* pSkelData);

	ENGINE_API bool				SaveAnimationToBinaryFile(const char* fileName,AnimationData* pAnimaData);

};

#endif// __ResourceBuilder_H__
