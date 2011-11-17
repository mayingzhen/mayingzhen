//--------------
//
// ResMeshMngr.h
// 描述:
// 网格资源管理器
//
//--------------

#ifndef __RES_MESHMNGR_H_
#define __RES_MESHMNGR_H_

#include <string>

#include "ResMngr.h"

class CSkinMesh;

class CSkinMeshMngr : public CResMngr
{

public:
	static CSkinMeshMngr* GetTheOne();

protected:
	virtual CRes* CreateRes(const std::string& strName, bool bBuildNow, void* pParam);
	virtual bool BuildRes(CRes *pRes);
	virtual void DestroyRes(CRes* pRes);

private:
	CSkinMeshMngr();
	~CSkinMeshMngr();
};

inline CSkinMeshMngr* ResMeshMngr()
{
	return CSkinMeshMngr::GetTheOne();
}

#endif	//__RES_MESHMNGR_H_
