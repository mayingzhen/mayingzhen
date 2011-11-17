#pragma once

#include <list>


#include "Bounding.h"

class CObject;


//八叉树节点（参照Ogre的Octree）
class COctreeNode      
{
public:
    COctreeNode(COctreeNode* parent);
    ~COctreeNode();

    void AddObject(CObject *object);
    void RemoveObject(CObject *object);

	bool IsTwiceSize( const CAABB& box ) const;
    void GetChildIndexes( const CAABB &, int *x, int *y, int *z ) const;

    void GetCullBounds( CAABB * ) const;

	int GetNumObject() {return m_nNumObject;}

public:
	CAABB mBox;
	D3DXVECTOR3 mHalfSize;
	COctreeNode* m_pChildren[2][2][2]; // 八个子节点
	std::list<CObject*> m_ObjectList;  // 当前节点下物体链表(不包括其子节点)

private:
	void Ref()
	{
        m_nNumObject++;
        if (m_pParent != 0) 
			m_pParent->Ref();
    };

    void Unref()
    {
        m_nNumObject--;
        if ( m_pParent != 0 ) 
			m_pParent->Unref();
    };

private:
	COctreeNode* m_pParent; // 父节点
	int m_nNumObject;		// 当前节点及其子节点包含的物体数量
};


class COctree //八叉树
{

public:
	


	COctreeNode* m_pRoot;

};


