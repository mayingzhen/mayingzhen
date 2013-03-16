#include "AABB.h"

namespace ma
{
	void AABB::Transform(const D3DXMATRIX& mat)
	{
		// Do nothing if current null or infinite
		if( this->IsNull() )
			return;

		D3DXVECTOR3 oldMin, oldMax, currentCorner, curCornerTS;

		// Getting the old values so that we can use the existing merge method.
		oldMin = m_vMin;
		oldMax = m_vMax;

		// reset
		SetNull();

		// We sequentially compute the corners in the following order :
		// 0, 6, 5, 1, 2, 4 ,7 , 3
		// This sequence allows us to only change one member at a time to get at all corners.

		// For each one, we transform it using the matrix
		// Which gives the resulting point and merge the resulting point.

		// First corner 
		// min min min
		currentCorner = oldMin;
		D3DXVec3TransformCoord(&curCornerTS,&currentCorner,&mat);
		Merge( curCornerTS );

		// min,min,max
		currentCorner.z = oldMax.z;
		D3DXVec3TransformCoord(&curCornerTS,&currentCorner,&mat);
		Merge( curCornerTS );

		// min max max
		currentCorner.y = oldMax.y;
		D3DXVec3TransformCoord(&curCornerTS,&currentCorner,&mat);
		Merge( curCornerTS );

		// min max min
		currentCorner.z = oldMin.z;
		D3DXVec3TransformCoord(&curCornerTS,&currentCorner,&mat);
		Merge( curCornerTS );

		// max max min
		currentCorner.x = oldMax.x;
		D3DXVec3TransformCoord(&curCornerTS,&currentCorner,&mat);
		Merge( curCornerTS );

		// max max max
		currentCorner.z = oldMax.z;
		D3DXVec3TransformCoord(&curCornerTS,&currentCorner,&mat);
		Merge( curCornerTS );

		// max min max
		currentCorner.y = oldMin.y;
		D3DXVec3TransformCoord(&curCornerTS,&currentCorner,&mat);
		Merge( curCornerTS );

		// max min min
		currentCorner.z = oldMin.z;
		D3DXVec3TransformCoord(&curCornerTS,&currentCorner,&mat);
		Merge( curCornerTS );; 
	}
}

