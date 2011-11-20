#include "sky.h"
#include "Material.h"
#include "effect_file.h"
#include "Camera.h"
#include "GameApp.h"


CSky::CSky()
{
	m_pSkyMap = NULL;
}

CSky::~CSky()
{
	SAFE_RELEASE(m_pSkyMap);
}

void CSky::Create()
{
	HRESULT hr = 0;

	m_SkyBoxModel.LoadFromXFile("res\\skybox01.x");

	hr = D3DXCreateCubeTextureFromFile(g_pD3DDevice,"res\\skybox02.dds",&m_pSkyMap);

	m_SkyTech.loadResource("shader\\simple_skybox.fx");	
}


void CSky::Render()
{	
	m_SkyTech.begin();
	m_SkyTech.BeginPass(0);

	LPD3DXEFFECT effect = m_SkyTech.effect();

	D3DXMATRIX matWorld,matScla;
	D3DXMatrixScaling(&matScla, 120,120,120); // box ด๓ะก = 100 * 2
	D3DXMatrixTranslation(&matWorld,0,0,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matScla);

	V( effect->SetTexture( "skyCubeMapTex", m_pSkyMap ) );

	m_SkyBoxModel.Render(&matWorld);

	m_SkyTech.endPass();
	m_SkyTech.end();
 }








