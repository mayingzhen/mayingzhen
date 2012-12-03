#ifndef __ResourceBuilder_H__
#define __ResourceBuilder_H__



class SerializeListener;
class Skeleton;
class ISkeleton;
class Animation;
class MeshData;

class IMeshData;
class ISkeletonData;
class IAnimationData;

//Sub for resource of different version loading
namespace ResourceBuilder
{

	//Skeleton*	LoadSkeleton(SerializeListener* pSL);

	//Animation*	LoadAnimation(SerializeListener* pSL);

	//Animation*	LoadLegacyAnimation(SerializeListener* pSL,ISkeleton* pSkel);

	COMMON_API bool			LoadMesh(MeshData* pMesh,SerializeListener* pSL);

	COMMON_API bool			SaveMesh(MeshData* pMesh,SerializeListener* pSL);

	COMMON_API IMeshData* LoadMeshFromBinaryFile(const char* filename);

	COMMON_API ISkeletonData* LoadSkeletonFromBinaryFile(const char* fileName);

	COMMON_API IAnimationData* LoadAnimationFromBinaryFile(const char* fileName);

};

#endif// __ResourceBuilder_H__