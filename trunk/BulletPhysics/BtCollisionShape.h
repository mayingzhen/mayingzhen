#ifndef __Bullet_CollisionShape_H__
#define __Bullet_CollisionShape_H__

#include "Engine/Physics/ICollisionShape.h"

namespace ma
{
	class BULLETPHYSICS_API BulletBoxCollisionShape : public IBoxCollisionShape
	{		
	public:	
		BulletBoxCollisionShape();

		~BulletBoxCollisionShape();

		virtual void			SetTransformLS(const maNodeTransform& tsfLS);

		virtual maNodeTransform GetTransformLS();

		virtual	void			SetCollisionLayer(int nCollLayer);

		virtual int				GetCollisionLayer();

		virtual void			SetSize(const D3DXVECTOR3& vSize);

		virtual	D3DXVECTOR3		GetSize(); 

	private:
		maNodeTransform m_tsfLS;

		int m_nCollLayer;

		D3DXVECTOR3 m_vSize;
	};

	
	class BULLETPHYSICS_API BulletSphereCollisionShape : public ISphereCollisionShape
	{		
	public:	
		BulletSphereCollisionShape();

		~BulletSphereCollisionShape();

		virtual void			SetTransformLS(const maNodeTransform& tsfLS);

		virtual maNodeTransform GetTransformLS();

		virtual	void			SetCollisionLayer(int eCollLayer);

		virtual int				GetCollisionLayer();

		virtual void			SetRadius(float fRadius);

		virtual	float			GetRadius(); 

	private:
		maNodeTransform m_tsfLS;
		int m_nCollLayer;

		float m_fRadius;
	};

}


#endif
