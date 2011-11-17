#include <assert.h>
#include "public.h"

#include "Texture.h"

const float TEXTURE_DELAY_TIME = 30.0f;


CTexture::CTexture(IDirect3DTexture9* pD3DTex):	m_pD3DTex( pD3DTex )
{
	assert( pD3DTex != NULL );
	SetDelayTime(TEXTURE_DELAY_TIME); 	// ��ʱ30���ͷ�
	m_isUseMatrix = false;              // �Ƿ�ʹ�������� 

	m_eTexBlendMode = D3DTOP_MODULATE;
}


CTexture::CTexture():m_pD3DTex( NULL )
{
	SetDelayTime(TEXTURE_DELAY_TIME);
	m_isUseMatrix = false;              // �Ƿ�ʹ�������� 

	m_eTexBlendMode = D3DTOP_MODULATE;
}


CTexture::~CTexture()
{
	SAFE_RELEASE( m_pD3DTex );
}