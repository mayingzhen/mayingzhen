#include "BulletPhysics/BtCollisionShape.h"

namespace ma
{	
	BulletBoxCollisionShape::BulletBoxCollisionShape()
	{
		//m_pBoxShape = NULL;
		TransformSetIdentity(&m_tsfLS);
		m_nCollLayer = 0;
	}

	BulletBoxCollisionShape::~BulletBoxCollisionShape()
	{

	}

	void BulletBoxCollisionShape::SetTransformLS(const NodeTransform& tsfLS) 
	{
		m_tsfLS = tsfLS;
	}

	NodeTransform BulletBoxCollisionShape::GetTransformLS() 
	{
		return m_tsfLS;
	}

	void BulletBoxCollisionShape::SetCollisionLayer(int nCollLayer)
	{
		m_nCollLayer = nCollLayer;
	}

	int BulletBoxCollisionShape::GetCollisionLayer()
	{
		return m_nCollLayer;
	}

	void BulletBoxCollisionShape::SetSize(const Vector3& vSize)
	{
		m_vSize = vSize;
	}

	Vector3 BulletBoxCollisionShape::GetSize()
	{
		return m_vSize;
	}


	BulletSphereCollisionShape::BulletSphereCollisionShape()
	{
		TransformSetIdentity(&m_tsfLS);
	}

	BulletSphereCollisionShape::~BulletSphereCollisionShape()
	{

	}

	void BulletSphereCollisionShape::SetTransformLS(const NodeTransform& tsfLS) 
	{
		m_tsfLS = tsfLS;
	}

	NodeTransform BulletSphereCollisionShape::GetTransformLS() 
	{
		return m_tsfLS;
	}

	void BulletSphereCollisionShape::SetCollisionLayer(int nCollLayer)
	{
		m_nCollLayer = nCollLayer;
	}

	int BulletSphereCollisionShape::GetCollisionLayer()
	{
		return m_nCollLayer;
	}

	void BulletSphereCollisionShape::SetRadius(float fRadius)
	{
		m_fRadius = fRadius;
	}

	float BulletSphereCollisionShape::GetRadius()
	{
		return m_fRadius;
	}



}