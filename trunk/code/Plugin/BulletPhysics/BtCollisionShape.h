#ifndef __Bullet_CollisionShape_H__
#define __Bullet_CollisionShape_H__

#include "Engine/Physics/ICollisionShape.h"

namespace ma
{

#define  DECL_CollisionShape \
	public : \
		virtual NodeTransform	GetTransformLS() {return m_tsfLS;} \
		virtual void			SetTransformLS(const NodeTransform& tsfLS)\
		{\
			m_tsfLS = tsfLS;\
		}\
	private: \
		NodeTransform	m_tsfLS;\
    private: 

	class BulletBoxCollisionShape : public IBoxCollisionShape
	{	
		DECL_CollisionShape

	public:	
		BulletBoxCollisionShape();

		~BulletBoxCollisionShape();

		virtual void			SetSize(const Vector3& vSize);

		virtual	Vector3			GetSize(); 

		virtual void			DebugRender(NodeTransform tsfWS);

	private:

		Vector3					m_vSize;
	};

	
	class BulletSphereCollisionShape : public ISphereCollisionShape
	{
		DECL_CollisionShape

	public:	
		BulletSphereCollisionShape();

		~BulletSphereCollisionShape();

		virtual void			SetRadius(float fRadius);

		virtual	float			GetRadius(); 

		virtual void			DebugRender(NodeTransform tsfWS);

	private:

		float					m_fRadius;
	};

	class BulletCapsuleCollisionShape : public ICapsuleCollisionShape
	{
		DECL_CollisionShape

	public:
		BulletCapsuleCollisionShape();	

		~BulletCapsuleCollisionShape();

		virtual void			SetHeight(float fHeight);

		virtual float			GetHeight() const;

		virtual void			SetRadius(float fRadius);

		virtual float			GetRadius() const;

		virtual void			DebugRender(NodeTransform tsfWS);

	private:
		float					m_fHeight;
		float					m_fRadius;
	};		

}


#endif
