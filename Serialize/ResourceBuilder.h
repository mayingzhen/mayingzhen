#ifndef __ResourceBuilder_H__
#define __ResourceBuilder_H__

namespace ma
{
	class MeshData;
	class SkeletonData;
	class AnimationData;

	// Load
	SERIALIZE_API MeshData*			LoadMeshFromBinaryFile(const char* filename);

	SERIALIZE_API SkeletonData*		LoadSkeletonFromBinaryFile(const char* fileName);

	SERIALIZE_API AnimationData*	LoadAnimationFromBinaryFile(const char* fileName);

	// Save
	SERIALIZE_API bool				SaveMeshToBinaryFile(const char* filename,MeshData* pMeshData);

	SERIALIZE_API bool				SaveSkeletonToBinaryFile(const char* fileName,SkeletonData* pSkelData);

	SERIALIZE_API bool				SaveAnimationToBinaryFile(const char* fileName,AnimationData* pAnimaData);

};

#endif// __ResourceBuilder_H__
