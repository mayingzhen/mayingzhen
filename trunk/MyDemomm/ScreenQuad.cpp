#include "public.h"
#include "ScreenQuad.h"
#include "GameApp.h"
#include "effect_file.h"
#include "Camera.h"

CScreenQuad g_SceneQuad;

CScreenQuad::CScreenQuad()
{
	m_simpleVertexDeclaration = NULL;
}

CScreenQuad::~CScreenQuad()
{
	SAFE_RELEASE(m_simpleVertexDeclaration); 
}

void CScreenQuad::Create()
{
	D3DVERTEXELEMENT9 simpleVertexElements[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, 
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	V( g_pD3DDevice->CreateVertexDeclaration(simpleVertexElements, &m_simpleVertexDeclaration) );

	// Construct full screen quad
	const D3DXVECTOR2 offset = D3DXVECTOR2(0.5f / g_nWndWidth, 0.5f / g_nWndHeigh);

	//   1    0 
	//   +----+
	//   | \  |  
	//   |  \ |
	//   +----+
	//   3    2
	/// view space near plan  ccw index  
	m_quadVerts[0] =  SimpleVertex( D3DXVECTOR3(1, 1, 1), D3DXVECTOR2(0.0f, 1.0f) + offset );
	m_quadVerts[1] =  SimpleVertex( D3DXVECTOR3(-1, +1, 1), D3DXVECTOR2(0.0f, 0.0f) + offset );
	m_quadVerts[2] =  SimpleVertex( D3DXVECTOR3(1, -1, 1), D3DXVECTOR2(1.0f, 1.0f) + offset );
	m_quadVerts[3] =  SimpleVertex( D3DXVECTOR3(-1, -1, 1), D3DXVECTOR2(0.0f, 1.0f) + offset );
}

void CScreenQuad::Render()
{
	V( g_pD3DDevice->SetFVF(NULL) );
	V( g_pD3DDevice->SetVertexDeclaration(m_simpleVertexDeclaration) );

	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);

	assert(g_pCurCEffect);
	if (g_pCurCEffect)
	{
		D3DXMATRIX mWordViewProj,mWOrldView;
		D3DXMatrixInverse(&mWOrldView, NULL, &g_Camera.m_mProj);
		D3DXMatrixIdentity(&mWordViewProj);

		g_pCurCEffect->setMatrix(CEffectFile::k_worldViewMatrix,&mWOrldView); 
		g_pCurCEffect->setMatrix(CEffectFile::k_worldViewProjMatrix,&mWordViewProj);
		g_pCurCEffect->CommitChanges();
	}

	V( g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_quadVerts, sizeof(SimpleVertex)) );

	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
}
