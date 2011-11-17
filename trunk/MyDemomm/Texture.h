#ifndef __RES_TEXTURE_H__
#define __RES_TEXTURE_H__

#include <d3d9.h>

#include "Res.h"


class CTexture : public CRes
{
	friend class CTextureMngr;

public:

	//��ȡ����ָ��
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

	D3DTEXTUREOP m_eTexBlendMode; // �����Ϸ�ʽ
	D3DXMATRIX m_TextureMatrix;   // ����������
	bool m_isUseMatrix;           // �Ƿ�ʹ�������� 	
};


#endif