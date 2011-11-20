#ifndef C_SKY_HH__
#define C_SKY_HH__

#include "public.h"

#include <d3d9.h>
#include <d3dx9.h>

#include "SinMesh.h"
#include "effect_file.h"


class CSky : public CSingleton<CSky>
{

public:
	CSky();
	~CSky();

	void Render();
	void Create();

private:
	CSkinMesh m_SkyBoxModel;
	LPDIRECT3DCUBETEXTURE9	m_pSkyMap;

	CEffectFile m_SkyTech;
};

#endif