#include "ResourceBuilder.h"


namespace ma
{
// 	MeshData* LoadMeshFromBinaryFile(const char* filename)
// 	{
//  		MeshData* pMeshData = new MeshData;
// 		pMeshData->Load(filename);	
// 		return pMeshData;
// 	}

	SkeletonData* LoadSkeletonFromBinaryFile(const char* filename)
	{
		SkeletonData* pSkelData = new SkeletonData();
		pSkelData->Load(filename);
		return pSkelData;
	}

	AnimationData* LoadAnimationFromBinaryFile(const char* fileName)
	{
		AnimationData* pAnimationData = new AnimationData();
		pAnimationData->Load(fileName);
		return pAnimationData;
	}

	bool SaveMeshToBinaryFile(const char* filename,MeshData* pMeshData)
	{
		if (filename == NULL || pMeshData == NULL)
			return false;

		pMeshData->Save(filename);

		return true;
	}

	bool SaveSkeletonToBinaryFile(const char* fileName,SkeletonData* pSkelData)
	{
		if (fileName == NULL || pSkelData == NULL)
			return false;

		pSkelData->Save(fileName);

		return true;
	}

	bool SaveAnimationToBinaryFile(const char* fileName,AnimationData* pAnimaData)
	{
		if (fileName == NULL || pAnimaData == NULL)
			return false;

		pAnimaData->Save(fileName);

		return true;
	}
}

