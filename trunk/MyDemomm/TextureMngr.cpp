#include "public.h"
#include "TextureMngr.h"
#include "Texture.h"
#include "GameApp.h"


///////////////////////////////////////////////////////////////////////////////
CTextureMngr* CTextureMngr::GetTheOne()
{
	static CTextureMngr TheOne;
	return &TheOne;
}

///////////////////////////////////////////////////////////////////////////////
CTextureMngr::CTextureMngr()
{
}

///////////////////////////////////////////////////////////////////////////////
CTextureMngr::~CTextureMngr()
{
}


void CTextureMngr::OnResetDevice( IDirect3DDevice9* pDevice )
{
	//��������Ŀǰ����managed�ģ�����������ʱû�и��������������
	//�����������Ҫ���ص������������������FixAll��ʵ�ֻ����FixRes�ӿ�
}


CRes* CTextureMngr::CreateRes(const string &strName, bool bBuildNow, void* pParam)
{
	if ( !bBuildNow ) //��ͬ�����أ��ȴ������̵߳���BuildRes
		return new CTexture;

	IDirect3DTexture9* pD3DTex = LoadTextureFromFile( strName.c_str() );

	if ( NULL == pD3DTex )
		return NULL;

	return new CTexture(pD3DTex);
}


bool CTextureMngr::BuildRes(CRes *pRes)
{
	CTexture *pTexture = static_cast<CTexture*>(pRes);
	assert(pTexture);
	if (pTexture == NULL)
		return false;

	IDirect3DTexture9* pD3DTex = LoadTextureFromFile( pTexture->GetName().c_str() );
	if ( NULL == pD3DTex )
	{
		assert(false);
		return false;
	}
	pTexture->m_pD3DTex = pD3DTex;
	return true;
}


void CTextureMngr::DestroyRes( CRes *pRes )
{
	assert( pRes != NULL );
	delete pRes;
}


IDirect3DTexture9* CTextureMngr::LoadTextureFromFile( LPCSTR pFilename )
{
	assert( pFilename != NULL );

	IDirect3DDevice9* pDevice = NULL/*OASIS_EFFECT::EffectSystem()->Device()*/;

	IDirect3DTexture9* pD3DTex = NULL;
	//��ʱͳһ��D3DPOOL_MANAGED��������Ҫ�Ļ��ɸ��ݲ�������ͬ
	//HRESULT hr = D3DXCreateTextureFromFileExA(	pDevice,
	//											strName.c_str(),
	//											D3DX_DEFAULT,
	//											D3DX_DEFAULT,
	//											D3DX_DEFAULT,
	//											0,
	//											D3DFMT_UNKNOWN,
	//											D3DPOOL_MANAGED,
	//											//D3DPOOL_DEFAULT,
	//											D3DX_DEFAULT,
	//											D3DX_DEFAULT,
	//											0, NULL, NULL,
	//											&pD3DTex );
	HRESULT hr = D3DXCreateTextureFromFile( g_pD3DDevice, pFilename, &pD3DTex );

	if ( FAILED( hr ) )
	{
		//assert(false);
		return NULL;
	}
	return pD3DTex;
}