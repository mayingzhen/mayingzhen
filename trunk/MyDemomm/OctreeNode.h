#pragma once

#include <list>


#include "Bounding.h"

class CObject;


//�˲����ڵ㣨����Ogre��Octree��
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
	COctreeNode* m_pChildren[2][2][2]; // �˸��ӽڵ�
	std::list<CObject*> m_ObjectList;  // ��ǰ�ڵ�����������(���������ӽڵ�)

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
	COctreeNode* m_pParent; // ���ڵ�
	int m_nNumObject;		// ��ǰ�ڵ㼰���ӽڵ��������������
};


class COctree //�˲���
{

public:
	


	COctreeNode* m_pRoot;

};


