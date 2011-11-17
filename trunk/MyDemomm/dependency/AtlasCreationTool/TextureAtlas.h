#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include "TextureObject.h"

class AtlasContainer;

class CTextureAtlas
{
public:
 	CTextureAtlas();
 	~CTextureAtlas(); 

	bool Create(LPDIRECT3DDEVICE9 pd3dDevice, const std::vector<std::string>& fileNames);

	bool GetInTextureAtlasUV(std::string fileName,
		float& texU, float& texV,
		float& texW, float& texH);

	LPDIRECT3DTEXTURE9 GetAtlasTexture();
	
private:
	bool CreateTextureAtlases(CmdLineOptionCollection const &options); 

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;

	std::vector<TextureInAlta> m_atlas; 

	AtlasContainer*   m_pAtlas;

};



#endif TEXTUREATLAS_H