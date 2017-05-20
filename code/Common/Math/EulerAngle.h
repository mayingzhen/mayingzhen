#ifndef __EulerAngle_H__
#define __EulerAngle_H__

#include "Quaternion.h"

namespace ma
{
	//Rotate on x axis first, then y,z axis
	//x-roll
	//y-pitch
	//z-yaw
	class EulerAngleXYZ
	{
	public:
		float x;
		float y;
		float z;

		EulerAngleXYZ();

		EulerAngleXYZ(float fX,float fY,float fZ);

		EulerAngleXYZ operator+(const EulerAngleXYZ& rhs);

		EulerAngleXYZ& operator+=(const EulerAngleXYZ& rhs);

		EulerAngleXYZ& operator*=(float);

		void Normalize();
	};

	void QuaternionFromEulerAngleXYZ(Quaternion* pQuat,const EulerAngleXYZ* pEuler);

	inline EulerAngleXYZ::EulerAngleXYZ()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	inline EulerAngleXYZ::EulerAngleXYZ(float fX,float fY,float fZ)
	:x(fX)
	,y(fY)
	,z(fZ)
	{

	}


	inline EulerAngleXYZ EulerAngleXYZ::operator +(const EulerAngleXYZ& rhs)
	{
		return EulerAngleXYZ(x + rhs.x,y + rhs.y, z + rhs.z);
	}

	inline EulerAngleXYZ& EulerAngleXYZ::operator +=(const EulerAngleXYZ& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	inline EulerAngleXYZ& EulerAngleXYZ::operator *=(float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}


	inline void EulerAngleXYZ::Normalize()
	{
		x = fmod(x,Math::TWO_PI);
		y = fmod(y,Math::TWO_PI);
		z = fmod(z,Math::TWO_PI);
	}

	inline void QuaternionFromEulerAngleXYZ(Quaternion* pQuat,const EulerAngleXYZ* pEuler)
	{	
		float fHalfX = 0.5f*pEuler->x;
		float fHalfY = 0.5f*pEuler->y;
		float fHalfZ = 0.5f*pEuler->z;
		Quaternion qRotX(cos(fHalfX),sin(fHalfX),0.0f,0.0f);
		Quaternion qRotY(cos(fHalfY),0.0f,sin(fHalfY),0.0f);
		Quaternion qRotZ(cos(fHalfZ),0.0f,0.0f,sin(fHalfZ));
		*pQuat = qRotY * qRotX;
		*pQuat = qRotZ * *pQuat;
	}

	inline bool EulerAngleXYZFromMatrix(EulerAngleXYZ* pEuler,const Matrix3* pMat)
	{
		// rot  = cy*cz           cy*sz           -sy
		//        cz*sx*sy-cx*sz  cx*cz+sx*sy*sz  cy*sx
		//        cx*cz*sy+sx*sz  -cz*sx+cx*sy*sz  cx*cy

		pEuler->y = -asin(pMat->m[2][0]);

		if ( pEuler->y < Math::HALF_PI )
		{
			if ( pEuler->y > -Math::HALF_PI )
			{
				pEuler->x = atan2(pMat->m[2][1],pMat->m[2][2]);
				pEuler->z = atan2(pMat->m[1][0],pMat->m[0][0]);
				return true;
			}
			else
			{
				// WARNING.  Not a unique solution.
				float fRmY = atan2(pMat->m[0][1],pMat->m[1][1]);
				pEuler->z = (0.0f);  // any angle works
				pEuler->x = pEuler->z - fRmY;
				return false;
			}
		}
		else
		{
			// WARNING.  Not a unique solution.
			float fRpY = atan2(pMat->m[0][1],pMat->m[1][1]);
			pEuler->z = (0.0f);  // any angle works
			pEuler->x = fRpY - pEuler->z;
			return false;
		}
	}

	inline bool EulerAngleFromQuaternion(EulerAngleXYZ* pEuler,const Quaternion* pQua)
	{
		Matrix3 mat;
		pQua->ToRotationMatrix(mat);

		return EulerAngleXYZFromMatrix(pEuler,&mat);
	}

	//axis is non zero
	//x-roll = 0, y-pitch, z-yaw
	inline void EulerAngleFromXToAxis(EulerAngleXYZ* pEuler,const Vector3* pAxis)
	{
		float r = sqrt(pAxis->x*pAxis->x+pAxis->y*pAxis->y);
		pEuler->x = 0.0f;
		pEuler->z = atan2(pAxis->y,pAxis->x);	//yaw
		pEuler->y = - atan2(pAxis->z,r);		//pitch
	}

}

#endif// __EulerAngle_H__