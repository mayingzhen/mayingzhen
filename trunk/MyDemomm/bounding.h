#pragma once

#include <d3dx9math.h>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <list>


//--------------------------------------------------------------------------------------
class CAABB  // ������Χ����
{
public:
	CAABB();

	CAABB(const CAABB &other);

	// ����һ�ѵ㹹��һ��AABB
	CAABB(const D3DXVECTOR3 *points, unsigned int n);

	// ����һ�ѵ㹹��һ��AABB
	explicit CAABB(const std::vector<D3DXVECTOR3> &points); 

	// ����һ��AABB����һ��AABB
	explicit CAABB(const std::vector<CAABB> &v);

	virtual ~CAABB();

	// ��һ����ϲ���AABB�й���һ���µ�AABB
	void Merge(const D3DXVECTOR3 &vPoint);

	// ��ԭAABB����AABB�ϲ���һ���������ߵ�AABB
	void Merge(const CAABB &box);

	// ���AABB�����ĵ�
	D3DXVECTOR3 Centroid( void ) const;

	D3DXVECTOR3 Point( int i ) const;

	D3DXVECTOR3 operator []( int i ) const;

	// ��AABB���б任
	void Transform( CAABB &result, const D3DXMATRIX &matrix ) const;

	// �ж���AABB���ཻ���ں�����ϵ
	bool Intersect( const CAABB& box ) const;

	// �ж�AABB��һ������ཻ���ں�����ϵ
	bool Intersect( const D3DXVECTOR3 &point ) const;

	// �ж�AABB��һ�����ߵ��ཻ���ں�����ϵ����������Ľ��㵽�������ľ���
	bool Intersect( const D3DXVECTOR3 &vOrigo, const D3DXVECTOR3 &vDir, float &fDist, bool solid = false ) const;

	// �ж�AABB��һ�����ߵ��ཻ���ں�����ϵ
	bool Intersect( const D3DXVECTOR3 &vOrigo, const D3DXVECTOR3 &vDir ) const;

	// �ж�AABB��һ��ƽ����ཻ��ϵ�������ཻ���򷵻�AABB��ƽ���λ�ù�ϵ��1���࣬-1���ࣩ
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

//Balder note:����һ����Χ��A��ȫ������Χ��B,��B�е���A������ʱ��������ȫ�������᷵��false
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

//Balder ����һ����Χ��B�Ƿ������Χ��A
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
class CFrustum  // ��׶��
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
	CFrustum(const D3DXMATRIX &mViewProj); // ͨ����ͶӰ��������׶
	~CFrustum() {}

	// ��AABB���п��Ӳ��ԣ�0-���ɼ���1-�ɼ���������2-�ɼ����ཻ��
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



































