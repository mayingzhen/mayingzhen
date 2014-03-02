#ifndef  _FbxImport_Module__H__
#define  _FbxImport_Module__H__


#include "Export.h"
#include "FbxImportParm.h"

namespace ma
{
	class Skeleton;

	FBXIMPORT_API void	FBXImporterModuleInit();

	FBXIMPORT_API void	FBXImporterModuleShutdown();

	FBXIMPORT_API bool	LoadStaticMeshData(const char* pFileName, ImportParm* pImportParm = NULL,
		const char* pOutMeshFile = NULL, const char* pOutMatFile = NULL);

	FBXIMPORT_API bool	LoadSkeletonMeshData(const char* pFileName,ImportParm* pImportParm = NULL,
		const char* pOutMeshFile = NULL, const char* pOutMatFile = NULL, 
		const char* pOutSkeFile = NULL, const char* pOutSkaFile = NULL);

	FBXIMPORT_API bool	LoadAnimationData(const char* pFileName,const Skeleton& skeData,
		const char* pOutSkaFile = NULL);
}



#endif

