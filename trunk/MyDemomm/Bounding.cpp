#include "Bounding.h"

#include "effect_file.h"
#include "GameApp.h"

#include <d3dx9math.h>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <list>





CAABB::CAABB(): vMax( -FLT_MAX, -FLT_MAX, -FLT_MAX ),
vMin( FLT_MAX, FLT_MAX, FLT_MAX ) 
{

}

CAABB::CAABB(const CAABB &other): vMax(other.vMax),
vMin(other.vMin) 
{

}

// 根据一堆点构造一个AABB
CAABB::CAABB(const D3DXVECTOR3 *points,unsigned int n):
vMax( -FLT_MAX, -FLT_MAX, -FLT_MAX ), vMin( FLT_MAX, FLT_MAX, FLT_MAX )
{
	for ( unsigned int i = 0; i < n; ++ i )
	{
		Merge( points[i] );
	}
}

	  // 根据一堆点构造一个AABB
CAABB::CAABB( const std::vector< D3DXVECTOR3 > &points ): 
vMax( -FLT_MAX, -FLT_MAX, -FLT_MAX ), vMin( FLT_MAX, FLT_MAX, FLT_MAX )
{
	unsigned int n = points.size();
	for ( unsigned int i = 0; i < n; ++ i )
	{
		Merge( points[i] );
	}
}

	  // 根据一堆AABB构造一个AABB
CAABB::CAABB( const std::vector< CAABB > &v ): 
vMax( -FLT_MAX, -FLT_MAX, -FLT_MAX ), vMin( FLT_MAX, FLT_MAX, FLT_MAX )
{
	unsigned int n = v.size();
	for ( unsigned int i = 0; i < n; ++ i )
	{
		Merge( v[i].vMax );
		Merge( v[i].vMin );
	}
}

CAABB::~CAABB() 
{

}

// 将一个点合并到AABB中构成一个新的AABB
void CAABB::Merge( const D3DXVECTOR3 &vPoint )
{
	vMin.x = min( vMin.x, vPoint.x );
	vMin.y = min( vMin.y, vPoint.y );
	vMin.z = min( vMin.z, vPoint.z );
	vMax.x = max( vMax.x, vPoint.x );
	vMax.y = max( vMax.y, vPoint.y );
	vMax.z = max( vMax.z, vPoint.z );
}


// 获得AABB的中心点
D3DXVECTOR3 CAABB::Centroid( void ) const
{
	D3DXVECTOR3 vCentroid( vMax + vMin );
	return 0.5f * vCentroid;
}

// 获得AABB上的8个点
//	   1-----0
//	  /|    /|
//	 / |   / |
//	5-----4  |
//	|  3--|--2
//	| /   | /
//	|/    |/
//	7-----6
D3DXVECTOR3 CAABB::Point( int i ) const
{
	return D3DXVECTOR3( (i&1)?vMin.x:vMax.x, (i&2)?vMin.y:vMax.y, (i&4)?vMin.z:vMax.z );
}

D3DXVECTOR3 CAABB::operator []( int i ) const
{
	return Point( i );
}

// 对AABB进行变换 Fix 可能导致AABB变大
void CAABB::Transform( CAABB &result, const D3DXMATRIX &matrix ) const
{
	D3DXVECTOR3 pts[8];
	for ( int i = 0; i < 8; ++ i )
	{
		pts[i] = Point( i );
		D3DXVec3TransformCoord( &pts[i], &pts[i], &matrix );
	}

	//result.vMax = D3DXVECTOR3( -FLT_MAX, -FLT_MAX, -FLT_MAX );
	//result.vMin = D3DXVECTOR3(  FLT_MAX,  FLT_MAX,  FLT_MAX );

	//for ( int i = 0; i < 8; ++ i )
	//{
	//	D3DXVECTOR3 tmp;
	//	D3DXVec3TransformCoord( &tmp, &pts[i], &matrix );
	//	result.Merge( tmp );
	//}
	result = CAABB(pts,8);
}

// 判断两AABB的相交（内含）关系
bool CAABB::Intersect(const CAABB& box) const
{
	if ( vMax.x < box.vMin.x ) return false;
	if ( vMax.y < box.vMin.y ) return false;
	if ( vMax.z < box.vMin.z ) return false;
	if ( vMin.x > box.vMax.x ) return false;
	if ( vMin.y > box.vMax.y ) return false;
	if ( vMin.z > box.vMax.z ) return false;
	return true;
}

// 判断AABB和一个点的相交（内含）关系
bool CAABB::Intersect( const D3DXVECTOR3 &point ) const
{
	return( point.x >= vMin.x && point.x <= vMax.x  && 
		point.y >= vMin.y && point.y <= vMax.y  && 
		point.z >= vMin.z && point.z <= vMax.z );
}

// 判断AABB和一条射线的相交（内含）关系，返回最近的交点到射线起点的距离
bool CAABB::Intersect( const D3DXVECTOR3 &vOrigo, const D3DXVECTOR3 &vDir, float &fDist, bool solid) const
{
	D3DXPLANE sides[6] = {	D3DXPLANE( 1, 0, 0, -vMin.x ), D3DXPLANE(-1, 0, 0, vMax.x ),
		D3DXPLANE( 0, 1, 0, -vMin.y ), D3DXPLANE( 0,-1, 0, vMax.y ),
		D3DXPLANE( 0, 0, 1, -vMin.z ), D3DXPLANE( 0, 0,-1, vMax.z ) };
	bool intersect = false;
	fDist = FLT_MAX;

	float dist[6];
	bool bInside = true;
	for ( int i = 0; i < 6; ++ i )
	{
		dist[i] = D3DXPlaneDotCoord( &sides[i], &vOrigo );
		if ( dist[i] < 0.f ) bInside = false;
	}
	if ( solid && bInside )
	{
		fDist = 0.f;
		return true;
	}

	for ( int i = 0; i < 6; ++ i )
	{
		if ( fabs( dist[i] ) < 0.00015f )
		{
			fDist = 0.f;
			return true;
		}

		float cosTheta = D3DXPlaneDotNormal( &sides[i], &vDir );
		if ( fabs( cosTheta ) < 0.00015f ) continue;

		float fDistTmp = - dist[i] / cosTheta;
		if ( fDistTmp < 0.0f ) continue;

		D3DXVECTOR3 vHit = fDistTmp * vDir + vOrigo;

		bool insideTmp = true;

		for ( int j = 0; ( j < 6 ) && insideTmp; ++ j )
		{
			if ( j == i ) continue;
			float d = D3DXPlaneDotCoord( &sides[j], &vHit );

			insideTmp = ( ( d + 0.00015f ) >= 0.0f );
		}

		if ( insideTmp )
		{
			if ( fDist > fDistTmp ) fDist = fDistTmp;
			intersect = true;
		}
	}

	return intersect;     
}

// 判断AABB和一条射线的相交（内含）关系
bool CAABB::Intersect( const D3DXVECTOR3 &vOrigo, const D3DXVECTOR3 &vDir ) const
{
	D3DXPLANE sides[6] = {	D3DXPLANE( 1, 0, 0, -vMin.x ), D3DXPLANE(-1, 0, 0, vMax.x ),
		D3DXPLANE( 0, 1, 0, -vMin.y ), D3DXPLANE( 0,-1, 0, vMax.y ),
		D3DXPLANE( 0, 0, 1, -vMin.z ), D3DXPLANE( 0, 0,-1, vMax.z ) };

	bool intersect = false;

	for ( int i = 0; ( i < 6 ) && !intersect; ++ i )
	{
		float dist = D3DXPlaneDotCoord ( &sides[i], &vOrigo );
		if ( fabs( dist ) < 0.00015f )
		{
			return true;
		}

		float cosTheta = D3DXPlaneDotNormal( &sides[i], &vDir );
		if ( fabs( cosTheta ) < 0.00015f ) continue;

		float fDist = - dist / cosTheta;
		if ( fDist < 0.0f ) continue;

		D3DXVECTOR3 vHit = fDist * vDir + vOrigo;

		intersect = true;

		for ( int j = 0; ( j < 6 ) && intersect; ++ j )
		{
			if ( j == i ) continue;
			float d = D3DXPlaneDotCoord( &sides[j], &vHit );

			intersect = ( ( d + 0.00015f ) >= 0.0f );
		}
	}

	return intersect;     
}

// 判断AABB和一个平面的相交关系，若不相交，则返回AABB和平面的位置关系（1正侧，-1负侧）
bool CAABB::Intersect( const D3DXPLANE &plane, int &nSide ) const
{
	int side;

	float fDot = D3DXPlaneDotCoord( &plane, &Point( 0 ) );
	if ( fDot < 0.0f ) side = -1;
	else if ( fDot > 0.0f ) side = 1;
	else side = 0;

	for ( int i = 1; i < 8; ++ i )
	{
		int newSide;
		fDot = D3DXPlaneDotCoord( &plane, &Point( i ) );
		if ( fDot < 0.0f ) newSide = -1;
		else if ( fDot > 0.0f ) newSide = 1;
		else newSide = 0;

		if ( newSide != side ) return true;
	}
	nSide = side;
	return false;
}

bool CAABB::isNull() const
{
	return false;
}


bool CAABB::isInfinite() const
{
	return false;
}

D3DXVECTOR3 CAABB::getSize(void) const
{
	return vMax - vMin;
}

D3DXVECTOR3 CAABB::getHalfSize(void) const
{
	return (vMax - vMin) * 0.5;
}

void CAABB::setExtents(const D3DXVECTOR3& min,const D3DXVECTOR3& max)
{
	assert( (min.x <= max.x && min.y <= max.y && min.z <= max.z) &&
		"The minimum corner of the box must be less than or equal to maximum corner" );

	vMin = min;
	vMax = max;
}

struct SimpleVertex
{
	D3DXVECTOR3 position;
};

void CAABB::Render(D3DXMATRIX& matWorld) const
{
	D3DXVECTOR3 boxPos[24];

	/// top
	boxPos[0] = Point(0);
	boxPos[1] = Point(1);
	boxPos[2] = Point(1);
	boxPos[3] = Point(5);
	boxPos[4] = Point(5);
	boxPos[5] = Point(4);
	boxPos[6] = Point(4);
	boxPos[7] = Point(0);

	/// buttom
	boxPos[8] = Point(7);
	boxPos[9] = Point(3);
	boxPos[10] = Point(3);
	boxPos[11] = Point(2);
	boxPos[12] = Point(2);
	boxPos[13] = Point(6);
	boxPos[14] = Point(6);
	boxPos[15] = Point(7);

	/// middle
	boxPos[16] = Point(1);
	boxPos[17] = Point(3);
	boxPos[18] = Point(0);
	boxPos[19] = Point(2);
	boxPos[20] = Point(4);
	boxPos[21] = Point(6);
	boxPos[22] = Point(5);
	boxPos[23] = Point(7);

	//	   1-----0
	//	  /|    /|
	//	 / |   / |
	//	5-----4  |
	//	|  3--|--2
	//	| /   | /
	//	|/    |/
	//	7-----6

	IDirect3DVertexDeclaration9* simpleVertexDeclaration;
	D3DVERTEXELEMENT9 simpleVertexElements[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, 
		D3DDECL_END()
	};
	V( g_pD3DDevice->CreateVertexDeclaration(simpleVertexElements, &simpleVertexDeclaration) );

	if (g_pCurCEffect)
	{
		D3DXMATRIX matView,matProj;
		g_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION,&matProj);
		g_pD3DDevice->SetVertexDeclaration(simpleVertexDeclaration);
		D3DXMATRIX matWorldView = matWorld * matView;
		D3DXMATRIX matWorldViewProj = matWorldView * matProj;

		g_pCurCEffect->setMatrix(CEffectFile::k_worldViewMatrix,&matWorldView);
		g_pCurCEffect->setMatrix(CEffectFile::k_worldViewProjMatrix,&matWorldViewProj);
		g_pCurCEffect->setMatrix(CEffectFile::k_worldMatrix,&matWorld);

		g_pCurCEffect->CommitChanges();
	}
	else
	{	
		g_pD3DDevice->SetTransform(D3DTS_WORLD,&matWorld);
	}

	//pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );	
	HRESULT hr = g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 12, boxPos, sizeof(SimpleVertex) );
	assert( SUCCEEDED(hr) );

	SAFE_RELEASE(simpleVertexDeclaration);
}

/*
void CAABB::Render_Point(D3DXMATRIX& matWorld) const
{
	D3DXVECTOR3 boxPos[24];

	/// top
	boxPos[0] = vPoint[0];
	boxPos[1] = vPoint[1];
	boxPos[2] = vPoint[1];
	boxPos[3] = vPoint[5];
	boxPos[4] = vPoint[5];
	boxPos[5] = vPoint[4];
	boxPos[6] = vPoint[4];
	boxPos[7] = vPoint[0];

	/// buttom
	boxPos[8] = vPoint[7];
	boxPos[9] = vPoint[3];
	boxPos[10] = vPoint[3];
	boxPos[11] = vPoint[2];
	boxPos[12] = vPoint[2];
	boxPos[13] = vPoint[6];
	boxPos[14] = vPoint[6];
	boxPos[15] = vPoint[7];

	/// middle
	boxPos[16] = vPoint[1];
	boxPos[17] = vPoint[3];
	boxPos[18] = vPoint[0];
	boxPos[19] = vPoint[2];
	boxPos[20] = vPoint[4];
	boxPos[21] = vPoint[6];
	boxPos[22] = vPoint[5];
	boxPos[23] = vPoint[7];

	//	   1-----0
	//	  /|    /|
	//	 / |   / |
	//	5-----4  |
	//	|  3--|--2
	//	| /   | /
	//	|/    |/
	//	7-----6

	IDirect3DVertexDeclaration9* simpleVertexDeclaration;
	D3DVERTEXELEMENT9 simpleVertexElements[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, 
		D3DDECL_END()
	};
	V( g_pD3DDevice->CreateVertexDeclaration(simpleVertexElements, &simpleVertexDeclaration) );

	if (g_pCurCEffect)
	{
		D3DXMATRIX matView,matProj;
		g_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION,&matProj);
		g_pD3DDevice->SetVertexDeclaration(simpleVertexDeclaration);
		D3DXMATRIX matWorldView = matWorld * matView;
		D3DXMATRIX matWorldViewProj = matWorldView * matProj;

		g_pCurCEffect->setMatrix(CEffectFile::k_worldViewMatrix,&matWorldView);
		g_pCurCEffect->setMatrix(CEffectFile::k_worldViewProjMatrix,&matWorldViewProj);
		g_pCurCEffect->setMatrix(CEffectFile::k_worldMatrix,&matWorld);

		g_pCurCEffect->CommitChanges();
	}
	else
	{	
		g_pD3DDevice->SetTransform(D3DTS_WORLD,&matWorld);
	}

	//pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );	
	HRESULT hr = g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 12, boxPos, sizeof(SimpleVertex) );
	assert( SUCCEEDED(hr) );

	SAFE_RELEASE(simpleVertexDeclaration);
}
*/


CFrustum::CFrustum(const D3DXMATRIX &mViewProj)
{
	D3DXVECTOR4 column1( mViewProj._11, mViewProj._21, mViewProj._31, mViewProj._41 );
	D3DXVECTOR4 column2( mViewProj._12, mViewProj._22, mViewProj._32, mViewProj._42 );
	D3DXVECTOR4 column3( mViewProj._13, mViewProj._23, mViewProj._33, mViewProj._43 );
	D3DXVECTOR4 column4( mViewProj._14, mViewProj._24, mViewProj._34, mViewProj._44 );


	vPlanes[0] = ( float * )( column4 - column1 );		// right
	vPlanes[1] = ( float * )( column4 + column1 );		// left
	vPlanes[2] = ( float * )( column4 - column2 );		// bottom
	vPlanes[3] = ( float * )( column4 + column2 );		// top
	vPlanes[4] = ( float * )( column4 - column3 );		// near
	vPlanes[5] = ( float * )( column4 + column3 );		// far

	for ( int i = 0; i < 6; ++ i )
	{
		D3DXPlaneNormalize( &vPlanes[i], &vPlanes[i] );
		nVertexLUT[i] = ( ( vPlanes[i].a < 0.f ) ? 1 : 0 ) | ( ( vPlanes[i].b < 0.f ) ? 2 : 0 ) | ( ( vPlanes[i].c < 0.f ) ? 4 : 0 );
	}

	//	   4-----5
	//	  /|    /|
	//	 / |   / |
	//	0-----1  |
	//	|  6--|--7  far
	//	| /   | /
	//	|/    |/
	//	2-----3  near
	for ( int i = 0; i < 8; ++ i )
	{
		D3DXPLANE &p0 = ( i&1 ) ? vPlanes[1] : vPlanes[0];
		D3DXPLANE &p1 = ( i&2 ) ? vPlanes[2] : vPlanes[3];
		D3DXPLANE &p2 = ( i&4 ) ? vPlanes[5] : vPlanes[4];

		D3DXVECTOR3 n0( ( const float* )p0 );
		D3DXVECTOR3 n1( ( const float* )p1 );
		D3DXVECTOR3 n2( ( const float* )p2 );

		D3DXVECTOR3 n1_n2, n2_n0, n0_n1;

		D3DXVec3Cross( &n1_n2, &n1, &n2 );
		D3DXVec3Cross( &n2_n0, &n2, &n0 );
		D3DXVec3Cross( &n0_n1, &n0, &n1 );

		float cosTheta = D3DXVec3Dot( &n0, &n1_n2 );

		float secTheta = 1.f / cosTheta;

		n1_n2 = n1_n2 * p0.d;
		n2_n0 = n2_n0 * p1.d;
		n0_n1 = n0_n1 * p2.d;

		vPts[i] = -( n1_n2 + n2_n0 + n0_n1 ) * secTheta;
	}
}


CFrustum::Visble CFrustum::VisibleTest( const CAABB &box ) const
{
	/* //也可以使用这种方法测试，更好理解
 	for (int i = 0; i < 6; ++i)
 	{
		int side = 0;
		if ( box.Intersect(vPlanes[i],side) )
			return PARTIAL;

		if (side == -1)
			return NONE;
	}

	return FULL;
	*/

	
	bool intersect = false;
	for ( int i = 0; i < 6; ++ i )
	{
		int nV = nVertexLUT[i];

		D3DXVECTOR3 nVertex( ( nV&1 ) ? box.vMin.x : box.vMax.x, 
							 ( nV&2 ) ? box.vMin.y : box.vMax.y, 
							 ( nV&4 ) ? box.vMin.z : box.vMax.z );

		D3DXVECTOR3 pVertex( ( nV&1 ) ? box.vMax.x : box.vMin.x, 
							 ( nV&2 ) ? box.vMax.y : box.vMin.y, 
							 ( nV&4 ) ? box.vMax.z : box.vMin.z );
		
		if ( D3DXPlaneDotCoord( &vPlanes[i], &nVertex ) < 0.0f ) 
			return NONE;

		if ( D3DXPlaneDotCoord( &vPlanes[i], &pVertex ) < 0.0f ) 
			intersect = true;
	}

	return intersect ? PARTIAL : FULL;
	
}

bool CFrustum::IsPointIn(const D3DXVECTOR3 &point) const
{
	for (int i = 0; i < 6; ++i)
	{
		if ( D3DXPlaneDotCoord(&vPlanes[i], &point) < 0.0f ) 
			return false;
	}

	return true;
}

void CFrustum::Render()
{
// 	D3DXVECTOR3 boxPos[24];
// 
// 	/// top
// 	boxPos[0] = vPts(0);
// 	boxPos[1] = vPts(1);
// 	boxPos[2] = vPts(1);
// 	boxPos[3] = vPts(5);
// 	boxPos[4] = vPts(5);
// 	boxPos[5] = vPts(4);
// 	boxPos[6] = vPts(4);
// 	boxPos[7] = vPts(0);
// 
// 	/// buttom
// 	boxPos[8] = vPts(7);
// 	boxPos[9] = vPts(3);
// 	boxPos[10] = vPts(3);
// 	boxPos[11] = vPts(2);
// 	boxPos[12] = vPts(2);
// 	boxPos[13] = vPts(6);
// 	boxPos[14] = vPts(6);
// 	boxPos[15] = vPts(7);
// 
// 	/// middle
// 	boxPos[16] = vPts(1);
// 	boxPos[17] = vPts(3);
// 	boxPos[18] = vPts(0);
// 	boxPos[19] = vPts(2);
// 	boxPos[20] = vPts(4);
// 	boxPos[21] = vPts(6);
// 	boxPos[22] = vPts(5);
// 	boxPos[23] = vPts(7);
// 
// 	//	   1-----0
// 	//	  /|    /|
// 	//	 / |   / |
// 	//	5-----4  |
// 	//	|  3--|--2
// 	//	| /   | /
// 	//	|/    |/
// 	//	7-----6
// 
// 	IDirect3DVertexDeclaration9* simpleVertexDeclaration;
// 	D3DVERTEXELEMENT9 simpleVertexElements[] =
// 	{
// 		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, 
// 		D3DDECL_END()
// 	};
// 	V( g_pD3DDevice->CreateVertexDeclaration(simpleVertexElements, &simpleVertexDeclaration) );
// 
// 	if (g_pCurCEffect)
// 	{
// 		D3DXMATRIX matView,matProj;
// 		g_pD3DDevice->GetTransform(D3DTS_VIEW,&matView);
// 		g_pD3DDevice->GetTransform(D3DTS_PROJECTION,&matProj);
// 		g_pD3DDevice->SetVertexDeclaration(simpleVertexDeclaration);
// 		D3DXMATRIX matWorldView = matWorld * matView;
// 		D3DXMATRIX matWorldViewProj = matWorldView * matProj;
// 
// 		g_pCurCEffect->setMatrix(CEffectFile::k_worldViewMatrix,&matWorldView);
// 		g_pCurCEffect->setMatrix(CEffectFile::k_worldViewProjMatrix,&matWorldViewProj);
// 		g_pCurCEffect->setMatrix(CEffectFile::k_worldMatrix,&matWorld);
// 
// 		g_pCurCEffect->CommitChanges();
// 	}
// 	else
// 	{	
// 		g_pD3DDevice->SetTransform(D3DTS_WORLD,&matWorld);
// 	}
// 
// 	//pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );	
// 	HRESULT hr = g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 12, boxPos, sizeof(SimpleVertex) );
// 	assert( SUCCEEDED(hr) );
// 
// 	SAFE_RELEASE(simpleVertexDeclaration);	
}




