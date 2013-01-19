#ifndef __ResourceBuilder_H__
#define __ResourceBuilder_H__



class SerializeListener;
// class Skeleton;
// class ISkeleton;
// class Animation;
// class MeshData;

// class IMeshData;
// class ISkeletonData;
// class IAnimationData;



//Sub for resource of different version loading
namespace ma
{
	//class MeshData;
	//class SkeletonData;
	//class AnimationData;

	SERIALIZE_API MeshData*		LoadMeshFromBinaryFile(const char* filename);

	SERIALIZE_API SkeletonData* LoadSkeletonFromBinaryFile(const char* fileName);

	SERIALIZE_API AnimationData* LoadAnimationFromBinaryFile(const char* fileName);

};

#endif// __ResourceBuilder_H__