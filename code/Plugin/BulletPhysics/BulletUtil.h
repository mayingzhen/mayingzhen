#ifndef __BULLET_UTIL_H__
#define __BULLET_UTIL_H__

namespace ma
{
	inline btVector3 ToBulletUnit(const Vector3& v)
	{
		return btVector3(v.x,v.y,v.z);
	}

	inline btQuaternion ToBulletUnit(const Quaternion& q)
	{
	 	return btQuaternion(q.x,q.y,q.z,q.w);
	}

	inline btTransform ToBulletUnit(const NodeTransform& tsf)
	{
		return btTransform(ToBulletUnit(tsf.m_qRot),ToBulletUnit(tsf.m_vPos));
	}

	inline Vector3 ToMaUnit(const btVector3& v)
	{
		return *(Vector3*)(&v);
	}

// 	inline Color ToMaUnit(const btVector3& v)
// 	{
// 
// 	}

	inline Quaternion ToMaUnit(const btQuaternion& q)
	{
		return Quaternion( q.x(),q.y(),q.z(),q.w() );
	
	}

	inline NodeTransform ToMaUnit(const btTransform& btTsf)
	{
		NodeTransform tsf;
		TransformSetIdentity(&tsf);
		tsf.m_fScale = 1.0f;
		tsf.m_vPos = ToMaUnit( btTsf.getOrigin() );
		tsf.m_qRot = ToMaUnit( btTsf.getRotation() );
		return tsf;
	}
}



#endif