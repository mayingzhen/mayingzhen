#pragma once

#include <d3dx9.h>

#include <vector>
#include <string>

#include "Res.h"
#include "public.h"

class CTexture;
class CEffectFile;


class CMaterial : public CRes
{
public:

	enum SCENE_BlEND
	{
		SOLID,		  //不透明
		GLASS_BLEND,  //玻璃混合
		FIRE_BLEND,   //火焰混合
		DARK_BLEND,	  //黑暗混合
		DARK2_BLEND,  //超黑混合
		NONAME_BLEND, //无名混合
	};

public:
	CMaterial();
	~CMaterial();

	void BeginRender(IDirect3DDevice9 *lpDevice);
	void EndRender(IDirect3DDevice9 *lpDevice);

	const D3DMATERIAL9& GetD3dMaterial() const  {return m_lpMaterial;}	
	const std::vector<CTexture*>& GetTexLayer() const {return m_vecTexLayer;} 
	const CEffectFile* GetEffectFile() const {return m_pEffectFile;}
	void SetEffectFile(CEffectFile* effectFile) {/*SAFE_RELEASE(m_pEffectFile);*/ m_pEffectFile = effectFile;}
	void SetTexture(CTexture* texture) {m_vecTexLayer.push_back(texture);}
	void SetScenBlend(SCENE_BlEND eBlendType) {m_eBlendMode = eBlendType;}
	void SetFileMode(D3DFILLMODE eFileMode) {m_eFileMode = eFileMode;}

	bool operator < (const CMaterial &data) const;

private:
	SCENE_BlEND m_eBlendMode;
	D3DFILLMODE	m_eFileMode;
	bool m_bAlphaTest;			// 是否开启Alpha测试		
	bool m_bFog;				// 是否进行雾化	
	bool m_bCullNone;			// 是否双面渲染


	std::string m_strEffectName; // 若为空的话，则为固定管线?!
	CEffectFile* m_pEffectFile;
	std::vector<CTexture*> m_vecTexLayer; // 纹理组 

	D3DMATERIAL9 m_lpMaterial; // d3d材质
};



