#ifndef __ResourceBuilder_H__
#define __ResourceBuilder_H__

namespace ma
{
	class MeshData;
	struct SkeletonData;
	struct AnimationData;

	// Load
	//ENGINE_API MeshData*		LoadMeshFromBinaryFile(const char* filename);

	ENGINE_API SkeletonData*	LoadSkeletonFromBinaryFile(const char* fileName);

	ENGINE_API AnimationData*	LoadAnimationFromBinaryFile(const char* fileName);

	// Save
	ENGINE_API bool				SaveMeshToBinaryFile(const char* filename,MeshData* pMeshData);

	ENGINE_API bool				SaveSkeletonToBinaryFile(const char* fileName,SkeletonData* pSkelData);

	ENGINE_API bool				SaveAnimationToBinaryFile(const char* fileName,AnimationData* pAnimaData);

};

#endif// __ResourceBuilder_H__
