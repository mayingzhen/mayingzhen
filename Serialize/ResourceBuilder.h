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

class MeshData;
class SkeletonData;
class AnimationData;

//Sub for resource of different version loading
namespace ResourceBuilder
{

	//Skeleton*	LoadSkeleton(SerializeListener* pSL);

	//Animation*	LoadAnimation(SerializeListener* pSL);

	//Animation*	LoadLegacyAnimation(SerializeListener* pSL,ISkeleton* pSkel);

	SERIALIZE_API bool			LoadMesh(MeshData* pMesh,SerializeListener* pSL);

	SERIALIZE_API bool			SaveMesh(MeshData* pMesh,SerializeListener* pSL);

	SERIALIZE_API MeshData*		LoadMeshFromBinaryFile(const char* filename);

	SERIALIZE_API SkeletonData* LoadSkeletonFromBinaryFile(const char* fileName);

	SERIALIZE_API AnimationData* LoadAnimationFromBinaryFile(const char* fileName);

};

#endif// __ResourceBuilder_H__