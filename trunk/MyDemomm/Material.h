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
		SOLID,		  //��͸��
		GLASS_BLEND,  //�������
		FIRE_BLEND,   //������
		DARK_BLEND,	  //�ڰ����
		DARK2_BLEND,  //���ڻ��
		NONAME_BLEND, //�������
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
	bool m_bAlphaTest;			// �Ƿ���Alpha����		
	bool m_bFog;				// �Ƿ������	
	bool m_bCullNone;			// �Ƿ�˫����Ⱦ


	std::string m_strEffectName; // ��Ϊ�յĻ�����Ϊ�̶�����?!
	CEffectFile* m_pEffectFile;
	std::vector<CTexture*> m_vecTexLayer; // ������ 

	D3DMATERIAL9 m_lpMaterial; // d3d����
};



