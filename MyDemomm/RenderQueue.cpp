
#include "RenderQueue.h"
#include "CObject.h"


void CRenderQueue::clear()
{
	m_NormalRQ.clear();
	m_TransRQ.clear();
	//m_ShadowSrcRQ.clear();
	//m_ShadowDestRQ.clear();
}



// void CRenderQueue::Render()
// {
// 	// 普通物体
// 	for (int i = 0; i < m_NormalRQ.size(); ++i)
// 	{
// 		m_NormalRQ[i]->Render();
// 	}
// 
// 	// 半透明物体
// 	for (int i = 0; i < m_TransRQ.size(); ++i)
// 	{
// 		m_TransRQ[i]->Render();
// 	}
// }


// Helper function for AABB vs frustum test
//
//
inline bool ProjectedIntersection(const D3DXVECTOR3 &vHalfSize, const D3DXVECTOR3 &vCenter,
								  const D3DXVECTOR3 *pFrustumPoints,
								  const D3DXVECTOR3 &vDir)
{
	// project AABB center point to vector
	float fCenter = D3DXVec3Dot(&vCenter, &vDir);
	// project AABB half-size to vector
	float fHalfSize = vHalfSize.x * fabs(vDir.x) +
		vHalfSize.y * fabs(vDir.y) +
		vHalfSize.z * fabs(vDir.z);

	float fMin1 = fCenter - fHalfSize;
	float fMax1 = fCenter + fHalfSize;

	// project frustum points
	float fProj2 = D3DXVec3Dot(&pFrustumPoints[0], &vDir);
	float fMin2 = fProj2;
	float fMax2 = fProj2;
	for(int i=1;i<8;i++)
	{
		fProj2 = D3DXVec3Dot(&pFrustumPoints[i], &vDir);
		fMin2 = Min(fProj2, fMin2);
		fMax2 = Max(fProj2, fMax2);
	}

	// test for overlap
	if(fMin1 > fMax2 || fMin2 > fMax1) return false;

	return true;
}

// AABB vs Frustum test, returns true if objects intersect
//
//
inline bool IntersectionTest(const CAABB& objectBB, const CFrustum &frustum)
{
	// Note that this code is very unoptimal
	//
	//
	D3DXVECTOR3 vHalfSize = (objectBB.vMax - objectBB.vMin) * 0.5f;
	D3DXVECTOR3 vCenter = (objectBB.vMin + objectBB.vMax) * 0.5f;

	// AABB face normals
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, D3DXVECTOR3(1,0,0))) return false;
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, D3DXVECTOR3(0,1,0))) return false;
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, D3DXVECTOR3(0,0,1))) return false;

	// frustum face normals
	//

	// front and back faces:
	D3DXVECTOR3 vNorm1;
	//= Normalize(Cross(frustum.vPts[1] - frustum.vPts[0],
	//	frustum.vPts[3] - frustum.vPts[0]));
	D3DXVec3Cross( &vNorm1, &(frustum.vPts[1] - frustum.vPts[0]), &(frustum.vPts[3] - frustum.vPts[0]) );
	D3DXVec3Normalize( &vNorm1, &vNorm1 );

	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm1)) return false;

	// left face:
	D3DXVECTOR3 vNorm2;
	//= Normalize(Cross(frustum.vPts[1] - frustum.vPts[0],
	//	frustum.vPts[4] - frustum.vPts[0]));
	D3DXVec3Cross( &vNorm2, &(frustum.vPts[1] - frustum.vPts[0]), &(frustum.vPts[4] - frustum.vPts[0]) );
	D3DXVec3Normalize( &vNorm2, &vNorm2 );
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm2)) return false;

	// right face:
	D3DXVECTOR3 vNorm3;
	//= Normalize(Cross(frustum.vPts[2] - frustum.vPts[3],
	//	frustum.vPts[7] - frustum.vPts[3]));
	D3DXVec3Cross( &vNorm3, &(frustum.vPts[2] - frustum.vPts[3]), &(frustum.vPts[7] - frustum.vPts[3]) );
	D3DXVec3Normalize( &vNorm3, &vNorm3 );
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm3)) return false;

	// top face:
	D3DXVECTOR3 vNorm4;
	//= Normalize(Cross(frustum.vPts[2] - frustum.vPts[1],
	//	frustum.vPts[5] - frustum.vPts[1]));
	D3DXVec3Cross( &vNorm4, &(frustum.vPts[2] - frustum.vPts[1]), &(frustum.vPts[5] - frustum.vPts[1]) );
	D3DXVec3Normalize( &vNorm4, &vNorm4 );
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm4)) return false;

	// bottom face:
	D3DXVECTOR3 vNorm5;
	//= Normalize(Cross(frustum.vPts[3] - frustum.vPts[0],
	//	frustum.vPts[4] - frustum.vPts[0]));
	D3DXVec3Cross( &vNorm5, &(frustum.vPts[3] - frustum.vPts[0]), &(frustum.vPts[4] - frustum.vPts[0]) );
	D3DXVec3Normalize( &vNorm5, &vNorm5 );
	if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm5)) return false;


	// edge cross edge cases
	//
	D3DXVECTOR3 pBoxEdges[3] = {D3DXVECTOR3(1,0,0), D3DXVECTOR3(0,1,0), D3DXVECTOR3(0,0,1)};
	for(int i=0;i<3;i++)
	{
		// edge up-down
		D3DXVECTOR3 vNorm1;
		//= Normalize(Cross(frustum.vPts[1] - frustum.vPts[0], pBoxEdges[i]));
		D3DXVec3Cross( &vNorm1, &(frustum.vPts[1] - frustum.vPts[0]), &pBoxEdges[i] );
		D3DXVec3Normalize( &vNorm1, &vNorm1 );
		if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm1)) return false;

		// edge left-right
		D3DXVECTOR3 vNorm2;
		//= Normalize(Cross(frustum.vPts[3] - frustum.vPts[0], pBoxEdges[i]));
		D3DXVec3Cross( &vNorm2, &(frustum.vPts[3] - frustum.vPts[0]), &pBoxEdges[i] );
		D3DXVec3Normalize( &vNorm2, &vNorm2 );
		if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm2)) return false;

		// edge bottom left
		D3DXVECTOR3 vNorm3;
		//= Normalize(Cross(frustum.vPts[4] - frustum.vPts[0], pBoxEdges[i]));
		D3DXVec3Cross( &vNorm3, &(frustum.vPts[4] - frustum.vPts[0]), &pBoxEdges[i] );
		D3DXVec3Normalize( &vNorm3, &vNorm3 );
		if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm3)) return false;

		// edge top left
		D3DXVECTOR3 vNorm4;
		//= Normalize(Cross(frustum.vPts[5] - frustum.vPts[1], pBoxEdges[i]));
		D3DXVec3Cross( &vNorm4, &(frustum.vPts[5] - frustum.vPts[1]), &pBoxEdges[i] );
		D3DXVec3Normalize( &vNorm4, &vNorm4 );
		if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm4)) return false;

		// edge top right
		D3DXVECTOR3 vNorm5;
		//= Normalize(Cross(frustum.vPts[6] - frustum.vPts[2], pBoxEdges[i]));
		D3DXVec3Cross( &vNorm5, &(frustum.vPts[6] - frustum.vPts[2]), &pBoxEdges[i] );
		D3DXVec3Normalize( &vNorm5, &vNorm5 );
		if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm5)) return false;

		// edge bottom right
		D3DXVECTOR3 vNorm6;
		//= Normalize(Cross(frustum.vPts[7] - frustum.vPts[3], pBoxEdges[i]));
		D3DXVec3Cross( &vNorm6, &(frustum.vPts[7] - frustum.vPts[3]), &pBoxEdges[i] );
		D3DXVec3Normalize( &vNorm6, &vNorm6 );
		if(!ProjectedIntersection(vHalfSize, vCenter, frustum.vPts, vNorm6)) return false;
	}

	// all tests passed - intersection occurs
	return true;
}


inline bool IntersectionTest(const CAABB &objectBB, const CAABB &frustumBB)
{
	// min and max vectors
	const D3DXVECTOR3 &vFrustumMin = frustumBB.vMin;
	const D3DXVECTOR3 &vFrustumMax = frustumBB.vMax;
	const D3DXVECTOR3 &vObjectMin = objectBB.vMin;
	const D3DXVECTOR3 &vObjectMax = objectBB.vMax;

	// test all axes
	//
	if(vObjectMin.x > vFrustumMax.x || vFrustumMin.x > vObjectMax.x) return false;
	if(vObjectMin.y > vFrustumMax.y || vFrustumMin.y > vObjectMax.y) return false;
	if(vObjectMin.z > vFrustumMax.z || vFrustumMin.z > vObjectMax.z) return false;

	// all tests passed - intersection occurs
	return true;
}

void CRenderQueue::update()
{
// 	std::list<CObject*>::iterator it = g_SceneMng.m_allObject.begin();	
// 	while ( it != g_SceneMng.m_allObject.end() )
// 	{
// 		CObject* sn = *it;
// 		if ( sn && IntersectionTest(sn->m_WorldAABB, frustum.m_AABB) )
// 		{
// 			PushToNormalRQ(sn);
// 		}
// 		++it;
// 	}
// 
// 	float g_fSplitPos[10];
// 	g_Camera.CalculateSplitPositions(g_fSplitPos, CD3DRender::NUM_PSSM);
// 
// 	for(int i = 0; i < CD3DRender::NUM_PSSM; i++)
// 	{
// 		CFrustum splitFrustum;
// 		splitFrustum = g_Camera.CalculateFrustum(g_fSplitPos[i], g_fSplitPos[i+1]);
// 
// 
// 	}

}