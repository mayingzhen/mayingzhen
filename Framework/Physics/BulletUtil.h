#ifndef __BULLET_UTIL_H__
#define __BULLET_UTIL_H__


namespace ma
{
	inline btVector3 ToBulletUnit(const D3DXVECTOR3& v)
	{
		return btVector3(v.x,v.y,v.z);
	}

	inline btQuaternion& ToBulletUnit(const D3DXQUATERNION& q)
	{
	 	return btQuaternion(q.x,q.y,q.z,q.w);
	}

	inline btTransform ToBulletUnit(const maNodeTransform& tsf)
	{
		return btTransform(ToBulletUnit(tsf.m_qRot),ToBulletUnit(tsf.m_vPos));
	}

	inline D3DXVECTOR3& ToMaUnit(const btVector3& v)
	{
		return *(D3DXVECTOR3*)(&v);
	}

	inline D3DXQUATERNION& ToMaUnit(const btQuaternion& q)
	{
		return D3DXQUATERNION(q.x,q.y,q.z,q.w);
	
	}

	inline maNodeTransform ToMaUnit(const btTransform& btTsf)
	{
		maNodeTransform tsf;
		tsf.m_fScale = 1.0f;
		tsf.m_vPos = btTsf.getOrigin();
		tsf.m_qRot = btTsf.getRotation();
		return tsf;
	}

	inline btDiscreteDynamicsWorld* GetBulletWorld(Component* pComp)
	{
		GameObject* pGameObj = pComp->GetGameObject();
		assert(pGameObj);
		if (pGameObj == NULL)
			return;

		Scene* pScene = pGameObj->GetScene();
		assert(pScene);
		if (pScene == NULL)
			return;

		PhysicsScene* pPhySicsScene = pScene->GetPhysicsScene();
		if (pPhySicsScene == NULL)
			return;

		return  pPhySicsScene->GetBulletWorld();
	}
}



#endif