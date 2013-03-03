#include "ResourceBuilder.h"


namespace ma
{

	MeshData* LoadMeshFromBinaryFile(const char* filename)
	{
		BinaryInputArchive ar;
		bool bLoadOK = ar.Open(filename);
		if (!bLoadOK)
		{
			LogError(_ERR_INVALID_CALL,"Fail to open mesh from file %s:",filename);
			return NULL;
		}

		MeshData* pMeshData = new MeshData;
		pMeshData->Serialize(ar,"Mesh");

		return pMeshData;
	}

	SkeletonData* LoadSkeletonFromBinaryFile(const char* filename)
	{
		BinaryInputArchive ar;
		bool bLoadOK = ar.Open(filename);
		if (!bLoadOK)
		{
			assert(false);
			LogError(_ERR_INVALID_CALL,"Fail to open mesh from file %s:",filename);
			return NULL;
		}

		SkeletonData* pSkelData = new SkeletonData();
		pSkelData->Serialize(ar, "Skeleton");

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
		BinaryOutputArchive ar;
		bool bLoadOK = ar.Open(filename);
		if (!bLoadOK)
		{
			LogError(_ERR_INVALID_CALL,"Fail to open mesh from file %s:",filename);
			return false;
		}

		pMeshData->Serialize(ar,"Mesh");

		return true;
	}

	bool SaveSkeletonToBinaryFile(const char* fileName,SkeletonData* pSkelData)
	{
		BinaryOutputArchive ar;
		bool bLoadOK = ar.Open(fileName);
		if (!bLoadOK)
		{
			assert(false);
			LogError(_ERR_INVALID_CALL,"Fail to open mesh from file %s:",fileName);
			return false;
		}

		pSkelData->Serialize(ar, "Skeleton");

		return true;
	}

	bool SaveAnimationToBinaryFile(const char* fileName,AnimationData* pAnimaData)
	{
		pAnimaData->Save(fileName);

		return true;
	}

}
