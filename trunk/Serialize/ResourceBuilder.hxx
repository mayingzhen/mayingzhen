#include "Serialize/ResourceBuilder.h"
//#include "Serialize/Serialize/SerializeListener.h"
//#include "Serialize/Serialize/BinaryInputArchive.h"
//#include "Serialize/MeshData.h"


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
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
		pMeshData->Serialize(&ar,"Mesh");

		return pMeshData;
	}

	SkeletonData* LoadSkeletonFromBinaryFile(const char* filename)
	{
		BinaryInputArchive ar;
		bool bLoadOK = ar.Open(filename);
		if (!bLoadOK)
		{
			LogError(_ERR_INVALID_CALL,"Fail to open mesh from file %s:",filename);
			return NULL;
		}

		SkeletonData* pSkelData = new SkeletonData();
		pSkelData->Serialize(ar, "Skeleton");

		return pSkelData;
	}

	AnimationData* LoadAnimationFromBinaryFile(const char* fileName)
	{
		BinaryInputArchive ar;
		bool bLoadOK = ar.Open(fileName);
		if (!bLoadOK)
		{
			LogError(_ERR_INVALID_CALL,"Fail to open mesh from file %s:",fileName);
			return NULL;
		}

		AnimationData* pAnimationData = new AnimationData();
		pAnimationData->Serialize(ar, "Animation");

		return pAnimationData;
	}
}