#pragma once

#include <d3dx9math.h>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <list>


//--------------------------------------------------------------------------------------
class CAABB  // 轴对齐包围盒类
{
public:
	CAABB();

	CAABB(const CAABB &other);

	// 根据一堆点构造一个AABB
	CAABB(const D3DXVECTOR3 *points, unsigned int n);

	// 根据一堆点构造一个AABB
	explicit CAABB(const std::vector<D3DXVECTOR3> &points); 

	// 根据一堆AABB构造一个AABB
	explicit CAABB(const std::vector<CAABB> &v);

	virtual ~CAABB();

	// 将一个点合并到AABB中构成一个新的AABB
	void Merge(const D3DXVECTOR3 &vPoint);

	// 将原AABB和新AABB合并成一个包含两者的AABB
	void Merge(const CAABB &box);

	// 获得AABB的中心点
	D3DXVECTOR3 Centroid( void ) const;

	D3DXVECTOR3 Point( int i ) const;

	D3DXVECTOR3 operator []( int i ) const;

	// 对AABB进行变换
	void Transform( CAABB &result, const D3DXMATRIX &matrix ) const;

	// 判断两AABB的相交（内含）关系
	bool Intersect( const CAABB& box ) const;

	// 判断AABB和一个点的相交（内含）关系
	bool Intersect( const D3DXVECTOR3 &point ) const;

	// 判断AABB和一条射线的相交（内含）关系，返回最近的交点到射线起点的距离
	bool Intersect( const D3DXVECTOR3 &vOrigo, const D3DXVECTOR3 &vDir, float &fDist, bool solid = false ) const;

	// 判断AABB和一条射线的相交（内含）关系
	bool Intersect( const D3DXVECTOR3 &vOrigo, const D3DXVECTOR3 &vDir ) const;

	// 判断AABB和一个平面的相交关系，若不相交，则返回AABB和平面的位置关系（1正侧，-1负侧）
	bool Intersect( const D3DXPLANE &plane, int &nSide ) const;

	bool isNull() const;


	bool isInfinite() const;

	D3DXVECTOR3 getSize(void) const;

	D3DXVECTOR3 getHalfSize(void) const;

	void setExtents(const D3DXVECTOR3& min,const D3DXVECTOR3& max);

	void Render(D3DXMATRIX& matWorld) const;
	void Render_Point(D3DXMATRIX& matWorld) const;

public:
	D3DXVECTOR3		vMax;
	D3DXVECTOR3		vMin;
};

//Balder note:返回一个包围盒A完全包含包围盒B,当B有点在A的面上时，不是完全包含，会返回false
inline bool operator > (const CAABB &boundBoxA,const CAABB &boundBoxB)
{
	if (boundBoxA.vMax.x <= boundBoxB.vMax.x) return false;
	if (boundBoxA.vMax.y <= boundBoxB.vMax.y) return false;
	if (boundBoxA.vMax.z <= boundBoxB.vMax.z) return false;

	if (boundBoxA.vMin.x >= boundBoxB.vMax.x) return false;
	if (boundBoxA.vMin.y >= boundBoxB.vMax.y) return false;
	if (boundBoxA.vMin.z >= boundBoxB.vMax.z) return false;
	return true;
}

//Balder 返回一个包围盒B是否包含包围盒A
inline bool operator < (const CAABB &boundBoxA,const CAABB &boundBoxB)
{
	if (boundBoxA.vMax.x >= boundBoxB.vMax.x) return false;
	if (boundBoxA.vMax.y >= boundBoxB.vMax.y) return false;
	if (boundBoxA.vMax.z >= boundBoxB.vMax.z) return false;

	if (boundBoxA.vMin.x <= boundBoxB.vMax.x) return false;
	if (boundBoxA.vMin.y <= boundBoxB.vMax.y) return false;
	if (boundBoxA.vMin.z <= boundBoxB.vMax.z) return false;
	return true;	
}


//--------------------------------------------------------------------------------------
class CFrustum  // 视锥类
{
public:
	enum Visble
	{
		NONE,
		FULL,
		PARTIAL,
	};

public:
	CFrustum() {}
	CFrustum(const D3DXMATRIX &mViewProj); // 通过视投影矩阵构造视锥
	~CFrustum() {}

	// 对AABB进行可视测试（0-不可见，1-可见并包含，2-可见并相交）
	Visble VisibleTest( const CAABB &box ) const;
	bool IsPointIn(const D3DXVECTOR3 &point) const;

	void Render();

	void CalculateAABB()
	{
		m_AABB = CAABB(vPts,8);
	}

public:
	D3DXVECTOR3	vPts[8];
	D3DXPLANE vPlanes[6];
	int nVertexLUT[6];

	CAABB m_AABB;
};



































