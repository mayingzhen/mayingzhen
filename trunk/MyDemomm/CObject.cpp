#include "public.h"
#include "CObject.h"

#include "ResMeshMngr.h"


CObject* CObject::GObjHash[OBJ_ARRAY_NUM]; // Object hash.

CObject::CObject() 
{
	//m_bIsVisible = true;	
	//m_bOnlyReceiveShadows = false;
	//m_bIsSKin = false;
	m_ObjFlag = 0;

	m_scale = 1.0f;		   // 缩放比例 
	m_angle = 0.0f;        // 方向  

	m_pOCtant = NULL;      // 所在的八叉树节点

	m_HashNext = NULL;
}

CObject::~CObject() 
{

}

void CObject::HashObject()
{
	int iHash = GetObjectHash( m_sHashName.c_str() );
	m_HashNext = GObjHash[iHash];
	GObjHash[iHash] = this;
}

void CObject::StaticInit()
{
	memset( GObjHash, 0, sizeof(GObjHash) );
}

CObject* CObject::StaticFindObject(const char* InName)
{
	int hashIndex = GetObjectHash(InName);
	for( CObject* hashObject = GObjHash[hashIndex]; hashObject != NULL;
		 hashObject = hashObject->m_HashNext )
	{
		if (hashObject->GetHashName() == InName)
		{
			return hashObject;
		}
	}
	return NULL;
}

int CObject::GetObjectHash(const char* InName)
{
	DWORD hash = 0;
	while (*InName != 0)
	{
		hash = (hash << 5) + hash + *InName;
		InName++;
	}
	return hash % OBJ_ARRAY_NUM;
}

bool CObject::IsIn(CAABB& box)
{
	if ( box.isNull() ) 
		return false;

	if (box.isInfinite())
		return true;

	D3DXVECTOR3 center = m_WorldAABB.Centroid();

	D3DXVECTOR3 bmin = box.vMin;
	D3DXVECTOR3 bmax = box.vMax;

	bool centre = ( bmax > center && bmin < center );
	if (!centre)
		return false;

	// Even if covering the centre line, need to make sure this BB is not large
	// enough to require being moved up into parent. When added, bboxes would
	// end up in parent due to cascade but when updating need to deal with
	// bbox growing too large for this child
	D3DXVECTOR3 octreeSize = bmax - bmin;
	D3DXVECTOR3 nodeSize = m_WorldAABB.vMax - m_WorldAABB.vMin;
	return nodeSize < octreeSize;
}


CMapObj::CMapObj()
{
	m_pSkinMesk = NULL;
}

CMapObj::~CMapObj()
{
	SAFE_RELEASE(m_pSkinMesk);
}

void CMapObj::SetSkinMesh(const std::string& modelName)
{	
	SAFE_DELETE(m_pSkinMesk);

	m_pSkinMesk = (CSkinMesh*)ResMeshMngr()->Query(modelName,true,NULL);
}

void CMapObj::Render()
{
	if (!m_pSkinMesk || !m_pSkinMesk->IsValid() )
		return;

	D3DXMATRIX matRy,matSc,matTrworld;
	D3DXMatrixRotationY(&matRy,m_angle);
	D3DXMatrixScaling(&matSc,m_scale,m_scale,m_scale);
	D3DXMatrixTranslation( &matTrworld, m_wordPos.x, m_wordPos.y, m_wordPos.z );
	matTrworld = matRy * matSc * matTrworld;

	m_pSkinMesk->Render(&matTrworld);

	//// AABB
	const CAABB& meshAABB = m_pSkinMesk->GetBoundingBox();
	const D3DXMATRIX& meshRootMat = m_pSkinMesk->GetRootMatrix();
	D3DXMATRIX AABBWorldmat = meshRootMat * matTrworld;
	meshAABB.Transform(m_WorldAABB,  AABBWorldmat);
 	
// 	D3DXMATRIX initMat;
// 	D3DXMatrixIdentity(&initMat);
// 	m_WorldAABB.Render(initMat);
}

void CMapObj::Update()
{
	m_pSkinMesk->Upadte();

	D3DXMATRIX matRy,matSc,matTrworld;
	D3DXMatrixRotationY(&matRy,m_angle);
	D3DXMatrixScaling(&matSc,m_scale,m_scale,m_scale);
	D3DXMatrixTranslation( &matTrworld, m_wordPos.x, m_wordPos.y, m_wordPos.z );
	matTrworld = matRy * matSc * matTrworld;

	//// AABB
	const CAABB& meshAABB = m_pSkinMesk->GetBoundingBox();
	const D3DXMATRIX& meshRootMat = m_pSkinMesk->GetRootMatrix();
	D3DXMATRIX AABBWorldmat = meshRootMat * matTrworld;
	meshAABB.Transform(m_WorldAABB,  AABBWorldmat);

}

void CMapObj::UpdateWorldAABB()
{
	assert(m_pSkinMesk);
	if (m_pSkinMesk == NULL)
		return;

	//D3DXVECTOR3 min,max;
	//m_pSkinMesk->GetBoundingBox(min,max);	

// 	D3DXMATRIX matRy,matSc,matTrworld;
// 	D3DXMatrixRotationY(&matRy,m_angle);
// 	D3DXMatrixScaling(&matSc,m_scale,m_scale,m_scale);
// 	D3DXMatrixTranslation(&matTrworld, m_wordPos.y, m_wordPos.y, m_wordPos.z);
// 	matTrworld = matRy * matSc * matTrworld;
// 
// 	D3DXVec3TransformCoord( &m_WorldAABB.vMax, &max, &matTrworld );
// 	D3DXVec3TransformCoord( &m_WorldAABB.vMin, &min, &matTrworld ); 

	//m_WorldAABB.vMax = max;
	//m_WorldAABB.vMin = min;
}





