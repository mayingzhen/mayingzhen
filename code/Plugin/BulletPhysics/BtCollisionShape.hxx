#include "BulletPhysics/BtCollisionShape.h"

namespace ma
{	
	BulletBoxCollisionShape::BulletBoxCollisionShape(GameObject* pGameObj)
		:IBoxCollisionShape(pGameObj)
	{
		TransformSetIdentity(&m_tsfLS);
		m_vSize = Vector3(0,0,0);
	}

	BulletBoxCollisionShape::~BulletBoxCollisionShape()
	{

	}

	void BulletBoxCollisionShape::SetSize(const Vector3& vSize)
	{
		m_vSize = vSize;
	}

	Vector3 BulletBoxCollisionShape::GetSize()
	{
		return m_vSize;
	}

	void BulletBoxCollisionShape::Serialize(Serializer& sl, const char* pszLable /* = "IBoxCollisionShape" */)
	{
		sl.BeginSection(pszLable);
		
		sl.Serialize(m_tsfLS,"tsfLS");

		sl.Serialize(m_vSize,"vSize");

		sl.EndSection();
	}

	void BulletBoxCollisionShape::DebugRender(NodeTransform tsfWS)
	{
		NodeTransform tsfLS = GetTransformLS();

		NodeTransform tsfBoxWS;
		TransformMul(&tsfBoxWS,&tsfWS,&tsfLS);
		Matrix4x4 matWS;
		MatrixFromTransform(&matWS,&tsfBoxWS);

		GetLineRender()->DrawBox(matWS,GetSize(),Color(1,0,0,0));
	}


	BulletSphereCollisionShape::BulletSphereCollisionShape(GameObject* pGameObj)
		:ISphereCollisionShape(pGameObj)
	{
		TransformSetIdentity(&m_tsfLS);
		m_fRadius = 0;
	}

	BulletSphereCollisionShape::~BulletSphereCollisionShape()
	{

	}

	void BulletSphereCollisionShape::SetRadius(float fRadius)
	{
		m_fRadius = fRadius;
	}

	float BulletSphereCollisionShape::GetRadius()
	{
		return m_fRadius;
	}

	void BulletSphereCollisionShape::Serialize(Serializer& sl, const char* pszLable /* = "IBoxCollisionShape" */)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_tsfLS,"tsfLS");

		sl.Serialize(m_fRadius,"fRadius");

		sl.EndSection();
	}


	void BulletSphereCollisionShape::DebugRender(NodeTransform tsfWS)
	{
		NodeTransform tsfLS = GetTransformLS();

		NodeTransform tsfBoxWS;
		TransformMul(&tsfBoxWS,&tsfWS,&tsfLS);
		Matrix4x4 matWS;
		MatrixFromTransform(&matWS,&tsfBoxWS);

		GetLineRender()->DrawWireSphere(matWS,GetRadius(),Color(1,0,0,0));
	}



	BulletCapsuleCollisionShape::BulletCapsuleCollisionShape(GameObject* pGameObj)
		:ICapsuleCollisionShape(pGameObj)
	{
		TransformSetIdentity(&m_tsfLS);
		m_fRadius = 0;
		m_fHeight = 0;
	}

	BulletCapsuleCollisionShape::~BulletCapsuleCollisionShape()
	{

	}

	void BulletCapsuleCollisionShape::SetHeight(float fHeight)
	{
		m_fHeight = fHeight;
	}

	float BulletCapsuleCollisionShape::GetHeight() const
	{
		return m_fHeight;
	}

	void BulletCapsuleCollisionShape::SetRadius(float fRadius)
	{
		m_fRadius = fRadius;
	}

	float BulletCapsuleCollisionShape::GetRadius() const
	{
		return m_fRadius;
	}

	void BulletCapsuleCollisionShape::Serialize(Serializer& sl, const char* pszLable /* = "IBoxCollisionShape" */)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_tsfLS,"tsfLS");

		sl.Serialize(m_fRadius,"fRadius");

		sl.Serialize(m_fHeight,"fHeight");

		sl.EndSection();
	}

	void BulletCapsuleCollisionShape::DebugRender(NodeTransform tsfWS)
	{
		NodeTransform tsfLS = GetTransformLS();

		NodeTransform tsfBoxWS;
		TransformMul(&tsfBoxWS,&tsfWS,&tsfLS);
		Matrix4x4 matWS;
		MatrixFromTransform(&matWS,&tsfBoxWS);

		float fRadius = GetRadius();
		float fHeight = GetHeight();
		Vector3 boxSize = Vector3(fRadius * 2, fHeight + 2 * fRadius, fRadius * 2);
		GetLineRender()->DrawBox(matWS,boxSize,Color(1,0,0,0));
	}
}