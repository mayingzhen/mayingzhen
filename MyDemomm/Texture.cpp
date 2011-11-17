#include <assert.h>
#include "public.h"

#include "Texture.h"

const float TEXTURE_DELAY_TIME = 30.0f;


CTexture::CTexture(IDirect3DTexture9* pD3DTex):	m_pD3DTex( pD3DTex )
{
	assert( pD3DTex != NULL );
	SetDelayTime(TEXTURE_DELAY_TIME); 	// 延时30秒释放
	m_isUseMatrix = false;              // 是否使用纹理动画 

	m_eTexBlendMode = D3DTOP_MODULATE;
}


CTexture::CTexture():m_pD3DTex( NULL )
{
	SetDelayTime(TEXTURE_DELAY_TIME);
	m_isUseMatrix = false;              // 是否使用纹理动画 

	m_eTexBlendMode = D3DTOP_MODULATE;
}


CTexture::~CTexture()
{
	SAFE_RELEASE( m_pD3DTex );
}