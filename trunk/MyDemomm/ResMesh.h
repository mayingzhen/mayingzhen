#ifndef	__RES_MESHRIGID_H_
#define __RES_MESHRIGID_H_

#include "Res.h"

class CSkinMesh;

class CResMesh : public CRes
{
	friend class CResMeshMngr;

public:
	inline CSkinMesh* GetMyMesh();

private:
	CResMesh();
	CResMesh( CSkinMesh* pMesh );
	~CResMesh();

private:
	CSkinMesh* m_pSkinMesh;

};


inline CSkinMesh* CResMesh::GetMyMesh()
{
	return m_pSkinMesh;
}

#endif	//__RES_MESHRIGID_H_