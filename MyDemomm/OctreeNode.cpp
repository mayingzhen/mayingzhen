#include "OctreeNode.h"
#include "CObject.h"
#include "public.h"



COctreeNode::COctreeNode(COctreeNode* parent) 
:mHalfSize( 0, 0, 0 )
{
	for ( int i = 0; i < 2; i++ )
	{
		for ( int j = 0; j < 2; j++ )
		{
			for ( int k = 0; k < 2; k++ )
			{
				m_pChildren[i][j][k] = 0;
			}
		}
	}
	m_pParent = parent;
	m_nNumObject = 0;
}

COctreeNode::~COctreeNode()
{
	for ( int i = 0; i < 2; i++ )
	{
		for ( int j = 0; j < 2; j++ )
		{
			for ( int k = 0; k < 2; k++ )
			{
				if ( m_pChildren[ i ][ j ][ k ] != 0 )
					SAFE_DELETE(m_pChildren[i][j][k]) ;
			}
		}
	}

	m_pParent = NULL;
}


bool COctreeNode::IsTwiceSize(const CAABB& box) const
{
	if ( box.isInfinite() )
		return false;

    D3DXVECTOR3 halfMBoxSize = mBox.getHalfSize();
    D3DXVECTOR3 boxSize = box.getSize();
    return ( (boxSize.x <= halfMBoxSize.x) && 
		     (boxSize.y <= halfMBoxSize.y) && 
			 (boxSize.z <= halfMBoxSize.z) );

}

void COctreeNode::GetChildIndexes(const CAABB &box, int *x, int *y, int *z) const
{
    D3DXVECTOR3 max = mBox.vMax;
    D3DXVECTOR3 min = box.vMin;

    D3DXVECTOR3 center = mBox.Centroid();

    D3DXVECTOR3 ncenter = box.Centroid();

    if ( ncenter.x > center.x )
        * x = 1;
    else
        *x = 0;

    if ( ncenter.y > center.y )
        * y = 1;
    else
        *y = 0;

    if ( ncenter.z > center.z )
        * z = 1;
    else
        *z = 0;
}

void COctreeNode::AddObject(CObject *object)
{
    m_ObjectList.push_back(object);
    Ref();
}

void COctreeNode::RemoveObject(CObject *object)
{
    m_ObjectList.erase( std::find( m_ObjectList.begin(), m_ObjectList.end(),object) );
    Unref();
}

void COctreeNode::GetCullBounds(CAABB *b) const
{
	b->vMax = mBox.vMax + mHalfSize;
	b->vMin = mBox.vMin - mHalfSize;
}

