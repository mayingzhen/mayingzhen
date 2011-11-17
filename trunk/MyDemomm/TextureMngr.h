
#ifndef __TEXTURE_MNGR_H__
#define __TEXTURE_MNGR_H__

#include <d3d9.h>
#include <string>
using namespace std;

#include "ResMngr.h"


class CTextureMngr : public CResMngr
{
public:
	//获取单件指针
	static CTextureMngr* GetTheOne();

public:
	void OnResetDevice( IDirect3DDevice9* pDevice );

protected:
	///////////////////////////////////////////////////////////////////////////
	//实现基类要求的接口
	///////////////////////////////////////////////////////////////////////////

	//创建资源
	virtual CRes* CreateRes( const string& strName, bool bBuildNow, void* pParam );
	//加载资源
	virtual bool BuildRes( CRes *pTexture );
	//销毁资源
	virtual void DestroyRes( CRes* pTexture );

private:
	//保证单件，不允许外界创建该类对象
	CTextureMngr();
	~CTextureMngr();

private:
	IDirect3DTexture9* LoadTextureFromFile( LPCSTR pFilename );
};

///////////////////////////////////////////////////////////////////////////////
inline CTextureMngr* TextureMngr()
{
	return CTextureMngr::GetTheOne();
}

#endif