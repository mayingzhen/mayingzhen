#ifndef __RES_TEXTURE_H__
#define __RES_TEXTURE_H__

#include <d3d9.h>

#include "Res.h"


class CTexture : public CRes
{
	friend class CTextureMngr;

public:

	//获取纹理指针
	IDirect3DTexture9* GetD3DTexture() {return m_pD3DTex;}

	D3DTEXTUREOP& GetTexOp() {return m_eTexBlendMode;}
	bool IsUseMatrix() {return m_isUseMatrix;}
	D3DXMATRIX* GetTexMatrix() {return &m_TextureMatrix;}

private:
	CTexture();
	CTexture( IDirect3DTexture9* pD3DTex );
	~CTexture();

private:
	IDirect3DTexture9* m_pD3DTex;

	D3DTEXTUREOP m_eTexBlendMode; // 纹理混合方式
	D3DXMATRIX m_TextureMatrix;   // 纹理动画矩阵
	bool m_isUseMatrix;           // 是否使用纹理动画 	
};


#endif