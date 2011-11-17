#ifndef C_SKY_HH__
#define C_SKY_HH__

#include "public.h"

#include <d3d9.h>
#include <d3dx9.h>

#include "SinMesh.h"
#include "effect_file.h"

class CTexture;

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z, 
		float nx, float ny, float nz, float u, float v)
	{
		_x = x;   _y = y;   _z = z;
		
		_nx = nx; _ny = ny; _nz = nz;

		_u = u;   _v = v;

	}

	float _x, _y, _z, _nx, _ny, _nz, _u, _v;

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
};


class CSky : public CSingleton<CSky>
{

public:
	CSky();
	~CSky();

	void Render();
	void Create();

private:
	LPD3DXMESH m_pMesh;
	//CTexture* m_texture[6];
	//DWORD NumSubsets;

	CSkinMesh m_SkyBoxModel;
	//CTexture* m_pSkyMap;
	LPDIRECT3DCUBETEXTURE9	m_pSkyMap;

	CEffectFile m_SkyTech;
};

#endif