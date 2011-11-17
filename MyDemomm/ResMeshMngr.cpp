//--------------
//
// ResMeshMngr.cpp
// 描述:
// 网格资源管理器
//
//--------------

#include <assert.h>

#include "ResMeshMngr.h"
#include "SinMesh.h"



CSkinMeshMngr* CSkinMeshMngr::GetTheOne()
{
	static CSkinMeshMngr TheOne;
	return &TheOne;
}

CSkinMeshMngr::CSkinMeshMngr()
{
}

CSkinMeshMngr::~CSkinMeshMngr()
{
}

CRes* CSkinMeshMngr::CreateRes(const std::string& strName, bool bBuildNow, void* pParam)
{
 	if (!bBuildNow) 
 		return new CSkinMesh;
 	
 	CSkinMesh* pMesh = new CSkinMesh;
 	pMesh->LoadFromXFile( strName.c_str() );
 	return pMesh;
}

bool CSkinMeshMngr::BuildRes(CRes *pRes)
{
	assert( pRes != NULL );

	CSkinMesh* pResMesh = static_cast<CSkinMesh*>(pRes);
	assert(pResMesh);
	if (pResMesh == NULL)
		return false;

	HRESULT res = pResMesh->LoadFromXFile( pResMesh->GetName().c_str() );
	if (res != S_OK)
		return false;

	return true;
}

void CSkinMeshMngr::DestroyRes(CRes* pRes)
{
	assert( pRes != NULL );
	delete pRes;
}
