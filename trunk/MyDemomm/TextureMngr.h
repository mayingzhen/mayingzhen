
#ifndef __TEXTURE_MNGR_H__
#define __TEXTURE_MNGR_H__

#include <d3d9.h>
#include <string>
using namespace std;

#include "ResMngr.h"


class CTextureMngr : public CResMngr
{
public:
	//��ȡ����ָ��
	static CTextureMngr* GetTheOne();

public:
	void OnResetDevice( IDirect3DDevice9* pDevice );

protected:
	///////////////////////////////////////////////////////////////////////////
	//ʵ�ֻ���Ҫ��Ľӿ�
	///////////////////////////////////////////////////////////////////////////

	//������Դ
	virtual CRes* CreateRes( const string& strName, bool bBuildNow, void* pParam );
	//������Դ
	virtual bool BuildRes( CRes *pTexture );
	//������Դ
	virtual void DestroyRes( CRes* pTexture );

private:
	//��֤��������������紴���������
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