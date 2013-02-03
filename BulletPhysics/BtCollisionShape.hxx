#include "BulletPhysics/BtCollisionShape.h"

namespace ma
{	
	BulletBoxCollisionShape::BulletBoxCollisionShape()
	{
		//m_pBoxShape = NULL;
		maTransformSetIdentity(&m_tsfLS);
		m_nCollLayer = 0;
	}

	BulletBoxCollisionShape::~BulletBoxCollisionShape()
	{

	}

	void BulletBoxCollisionShape::SetTransformLS(const maNodeTransform& tsfLS) 
	{
		m_tsfLS = tsfLS;
	}

	maNodeTransform BulletBoxCollisionShape::GetTransformLS() 
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

	void BulletBoxCollisionShape::SetSize(const D3DXVECTOR3& vSize)
	{
		m_vSize = vSize;
	}

	D3DXVECTOR3 BulletBoxCollisionShape::GetSize()
	{
		return m_vSize;
	}


	BulletSphereCollisionShape::BulletSphereCollisionShape()
	{
		maTransformSetIdentity(&m_tsfLS);
	}

	BulletSphereCollisionShape::~BulletSphereCollisionShape()
	{

	}

	void BulletSphereCollisionShape::SetTransformLS(const maNodeTransform& tsfLS) 
	{
		m_tsfLS = tsfLS;
	}

	maNodeTransform BulletSphereCollisionShape::GetTransformLS() 
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