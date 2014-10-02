#include "NodeTransform.h"
#include "CullTree.h"

namespace ma
{
	IMPL_OBJECT(NodeTransform,Object)

	NodeTransform::NodeTransform()
	{
		m_matWorld = Matrix4::IDENTITY;
		m_bmatWSDirty = true;
	}

	NodeTransform::~NodeTransform()
	{

	}

	void NodeTransform::SetTransform(const Transform& tsf)
	{
		m_tsfWS = tsf;

		m_bmatWSDirty = true;
	}

	const Transform& NodeTransform::GetTransform()
	{
		return m_tsfWS;
	}

	Vector3	NodeTransform::GetForward()
	{
		return GetWorldMatrix().getZAxis();
	}

	Vector3	NodeTransform::GetRight()
	{
		return GetWorldMatrix().getXAxis();
	}
	
	Vector3	NodeTransform::GetUp()
	{
		return GetWorldMatrix().getYAxis();
	}

	Vector3	NodeTransform::GetPos()
	{
		return GetTransform().m_vPos;
	}

	Quaternion NodeTransform::GetRotate()
	{
		return GetTransform().m_qRot;
	}

	void  NodeTransform::Forward(float x)
	{
		Translate(GetForward() * x);
	}

	void  NodeTransform::Up(float x)
	{
		Translate(GetUp() * x);
	}

	void  NodeTransform::Right(float x)
	{
		Translate(GetRight() * x);
	}

	void  NodeTransform::Scale(float fScale)
	{
		Transform tsf = GetTransform();
		tsf.m_vScale *= Vector3(fScale,fScale,fScale);
		SetTransform(tsf);
	}

	void NodeTransform::LookAt(const Vector3& vEye, const Vector3& vAt)
	{
		Vector3 vUp = Vector3::UNIT_Z;

		Matrix4 matView = Math::MakeLookAtMatrixRH(vEye,vAt,vUp);
		
		Matrix4 matWS = matView.inverse();
		
		Transform tsfWS;
		TransformFromMatrix(&tsfWS,&matWS);

		SetTransform(tsfWS);

// 		Vector3 vDirZ = vEye - vAt;
// 		vDirZ.normalise();
// 
// 		Vector3 vUp = Vector3::UNIT_Z;
// 
// 		Vector3 vDirX = vUp.crossProduct(vDirZ);
// 		vDirX.normalise();
// 
// 		Vector3 vDirY = vDirZ.crossProduct(vDirX);
// 		vDirY.normalise();
// 
// 		Transform tsfWS = GetTransform();
// 		tsfWS.m_vPos = vEye;
// 		tsfWS.m_qRot.FromAxes(vDirX,vDirY,vDirZ);
// 
// 		//Matrix4 mat;
// 		//MatrixFromTransform(&mat,&tsfWS);
// 
// 		SetTransform(tsfWS);
	}

	void NodeTransform::LookAt(const Vector3& vTarget)
	{
		LookAt(GetPos(),vTarget);
	}


	void NodeTransform::Translate(const Vector3& vTrans)
	{
		Transform tsf = GetTransform(); 
		tsf.m_vPos += vTrans;
		SetTransform(tsf);
	}

	void NodeTransform::RotateLS(float xDegree,float yDegree,float zDegree)
	{
		float xRadian = DegreesToRadians(xDegree);
		float yRadian = DegreesToRadians(yDegree);
		float zRadian = DegreesToRadians(zDegree);

		Matrix3 matRotate;
		matRotate.FromEulerAnglesXYZ(Radian(xRadian),Radian(yRadian),Radian(zRadian));

		Quaternion qRotate(matRotate);
		Transform tsfWS = GetTransform();
		tsfWS.m_qRot = qRotate * tsfWS.m_qRot;
		SetTransform(tsfWS);
	}

	void NodeTransform::RotateWS(float xDegree,float yDegree,float zDegree)
	{
		float xRadian = DegreesToRadians(xDegree);
		float yRadian = DegreesToRadians(yDegree);
		float zRadian = DegreesToRadians(zDegree);

		Matrix3 matRotate;
		matRotate.FromEulerAnglesXYZ(Radian(xRadian),Radian(yRadian),Radian(zRadian));

		Quaternion qRotate(matRotate);
		Transform tsfWS = GetTransform();
		tsfWS.m_qRot = tsfWS.m_qRot * qRotate;
		SetTransform(tsfWS);
	}

	void NodeTransform::RotateAround(const Vector3& vPoint, Vector3 vAxis,float angle)
	{
		float fRadian = DegreesToRadians(angle);

		Transform tsf;
		tsf.m_vPos = vPoint;
		tsf.m_qRot = Quaternion( Radian(fRadian), vAxis );
		
		Transform tsfWS = GetTransform();
		TransformMul(&tsfWS,&tsf,&tsfWS);
		SetTransform(tsfWS);
	}

	const Matrix4& NodeTransform::GetWorldMatrix()
	{
		if (m_bmatWSDirty)
		{
			UpdateMatWorld();
		}
		return m_matWorld;
	}

	void NodeTransform::UpdateMatWorld()
	{
		const Transform tsfWS = GetTransform();
		m_matWorld.makeTransform(tsfWS.m_vPos,m_tsfWS.m_vScale,tsfWS.m_qRot);
		m_bmatWSDirty = false;

		OnTransformChange();
	}

	void NodeTransform::Serialize(Serializer& sl, const char* pszLable)
	{
		//sl.BeginSection(pszLable);

		sl.Serialize(m_tsfWS,"tsfWS");

		//sl.EndSection();
	}
}
