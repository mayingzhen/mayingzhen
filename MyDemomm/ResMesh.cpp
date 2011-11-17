
#include "ResMesh.h"
#include "SinMesh.h"

const float TEXTURE_DELAY_TIME = 30.0f;

CResMesh::CResMesh(): m_pSkinMesh(NULL)
{
	SetDelayTime( TEXTURE_DELAY_TIME );
}

CResMesh::CResMesh(CSkinMesh* pMesh): m_pSkinMesh(pMesh)
{
	assert(pMesh != NULL);
	SetDelayTime( TEXTURE_DELAY_TIME );
}

CResMesh::~CResMesh()
{
	delete m_pSkinMesh;
	m_pSkinMesh = NULL;
}