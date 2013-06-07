#include "SceneNode.h"

namespace ma
{
	IMPL_OBJECT(SceneNode,Object)

	SceneNode::SceneNode(Scene* pSene, const char* pNodeName):
	Object(pNodeName)
	{
		m_pScene = pSene;	
		m_pParentNode = NULL;
		m_btsfPSDirty = false;
		m_btsfWSDirty = true;
		m_bmatWSDirty = true;

		MatrixIdentity(&m_matWorld);
		TransformSetIdentity(&m_tsfPS);
		TransformSetIdentity(&m_tsfWS);

		//m_aabb.SetNull();
	}

	SceneNode::~SceneNode()
	{

	}

	void SceneNode::Update()
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->Update();
		}
	}

	void SceneNode::ParalleUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->ParalleUpdate(fTimeElapsed);
		}
	}

	void SceneNode::FixedUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->FixedUpdate(fTimeElapsed);
		}
	}

	void SceneNode::LateUpdate(float fTimeElapsed)
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->LateUpdate(fTimeElapsed);
		}
	}

	void SceneNode::Render()
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->Render();
		}
	}

	void SceneNode::Start()
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->Start();
		}
	}

	void SceneNode::Stop()
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			if (m_arrChildNode[i] == NULL)
				continue;

			m_arrChildNode[i]->Stop();
		}
	}

	void SceneNode::SetTransform(const NodeTransform& tsf, TRANSFORM_TYPE ts)
	{
		if (ts == TS_WORLD)
		{
			SetTransformWSDirty();

			m_tsfWS = tsf;
			m_btsfWSDirty = false;
		}
		else if (ts == TS_PARENT)
		{
			SetTransformWSDirty();

			m_tsfPS = tsf;
			m_btsfPSDirty = false;
		}
		else 
		{
			ASSERT(false);
		}
	}

	const NodeTransform& SceneNode::GetTransform(TRANSFORM_TYPE ts)
	{
		if (ts == TS_PARENT)
		{
			if (m_btsfPSDirty)
			{
				UpdateTransformPS();
			}
			return m_tsfPS;
		}
		else if (ts == TS_WORLD)
		{
			if (m_btsfWSDirty)
			{
				UpdateTransformWS();
			}
			return m_tsfWS;
		}
		else
		{
			ASSERT(false);
			if (m_btsfPSDirty)
			{
				UpdateTransformPS();
			}
			return m_tsfPS;	
		}
	}


	void SceneNode::SetPosition(const Vector3& vPos, TRANSFORM_TYPE ts)
	{		
		NodeTransform tsf = GetTransform(ts);
		tsf.m_vPos = vPos;
		SetTransform(tsf,ts);
	}

	const Vector3&	SceneNode::GetPosition(TRANSFORM_TYPE ts)
	{
		return GetTransform(ts).m_vPos;
	}

	const Quaternion& SceneNode::GetRotate(TRANSFORM_TYPE ts)
	{
		return GetTransform(ts).m_qRot;
	}

// 	EulerAngleXYZ SceneNode::GetRotate(TRANSFORM_TYPE ts)
// 	{
// 		EulerAngleXYZ euler;
// 		EulerAngleFromQuaternion(&euler,&GetTransform(ts).m_qRot);
// 		return euler;	
// 	}

	Vector3	SceneNode::GetDirection(TRANSFORM_TYPE ts)
	{
		Quaternion qRotate = GetTransform(ts).m_qRot;
		Matrix4x4 matRotate;
		MatrixFromQuaternion(&matRotate,&qRotate);
		return matRotate.GetRow(3);

	}

	Vector3	SceneNode::GetRight(TRANSFORM_TYPE ts)
	{
		Quaternion qRotate = GetTransform(ts).m_qRot;
		Matrix4x4 matRotate;
		MatrixFromQuaternion(&matRotate,&qRotate);
		return matRotate.GetRow(0);
	}
	
	Vector3	SceneNode::GetUp(TRANSFORM_TYPE ts)
	{
		Quaternion qRotate = GetTransform(ts).m_qRot;
		Matrix4x4 matRotate;
		MatrixFromQuaternion(&matRotate,&qRotate);
		return matRotate.GetRow(1);
	}

	void  SceneNode::Move(float x)
	{
		Vector3 vPos = GetPosition(TS_PARENT);
		vPos += GetDirection(TS_PARENT) * x;
		SetPosition(vPos,TS_PARENT);
	}

	void  SceneNode::Fly(float x)
	{
		Vector3 vPos = GetPosition(TS_PARENT);
		vPos += GetUp(TS_PARENT) * x;
		SetPosition(vPos,TS_PARENT);
		
	}

	void  SceneNode::Right(float x)
	{
		Vector3 vPos = GetPosition(TS_PARENT);
		vPos += GetRight(TS_PARENT) * x;
		SetPosition(vPos,TS_PARENT);
	}


	void SceneNode::Translate(const Vector3& vDir, TRANSFORM_TYPE ts)
	{
		if (ts == TS_WORLD || ts == TS_PARENT)
		{
			Vector3 vPos = GetPosition(ts);
			vPos += vDir;
			SetPosition(vPos,ts);
		}
		else if (ts == TS_LOCAL)
		{
			NodeTransform tsfPS = GetTransform(TS_PARENT);
			Vector3 vDirPS;
			QuaternionTransformVector(&vDirPS,&vDir,&tsfPS.m_qRot);
			tsfPS.m_vPos += vDirPS;
			SetTransform(tsfPS,TS_PARENT);
		}
	}

	void SceneNode::Rotate(const Quaternion& qRot, TRANSFORM_TYPE ts)
	{
		if (ts == TS_WORLD || ts == TS_PARENT)
		{
			NodeTransform tsf = GetTransform(ts);
			tsf.m_qRot = qRot * tsf.m_qRot;
			SetTransform(tsf,ts);
		}
		else if (ts == TS_LOCAL)
		{
			NodeTransform tsfPS = GetTransform(TS_PARENT);
			tsfPS.m_qRot = tsfPS.m_qRot * qRot;
			SetTransform(tsfPS,TS_PARENT);
		}
	}	

	void SceneNode::Rotate(float xDegree,float yDegree,float zDegree, TRANSFORM_TYPE ts)
	{
		Quaternion qRot;
		EulerAngleXYZ qEuler(ToRadian(xDegree), ToRadian(yDegree), ToRadian(xDegree));
		QuaternionFromEulerAngleXYZ(&qRot,&qEuler);
		Rotate(qRot,ts);
	}

	const Matrix4x4& SceneNode::GetWorldMatrix()
	{
		if (m_bmatWSDirty)
		{
			UpdateMatWorld();
		}
		return m_matWorld;
	}

	void SceneNode::SetTransformWSDirty()
	{
		m_btsfWSDirty = true;
		m_bmatWSDirty = true;
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			m_arrChildNode[i]->SetTransformWSDirty();
		}
	}

	void					SceneNode::UpdateMatWorld()
	{
		MatrixFromTransform(&m_matWorld,&GetTransform(TS_WORLD));
		m_bmatWSDirty = false;
	}

	void					SceneNode::UpdateTransformWS()
	{
		ASSERT(!m_btsfPSDirty);
		if (m_pParentNode == NULL)
		{
			m_tsfWS = m_tsfPS;
		}
		else
		{
			TransformMul(&m_tsfWS, &m_tsfPS, &m_pParentNode->GetTransform(TS_WORLD));
		}
		m_btsfWSDirty = false;
	}

	void					SceneNode::UpdateTransformPS()
	{
		ASSERT(!m_btsfWSDirty);
		if (m_pParentNode == NULL)
		{
			m_tsfPS = m_tsfWS;
		}
		else
		{
			TransformInvMul(&m_tsfPS,&m_tsfWS,&m_pParentNode->GetTransform(TS_PARENT));
		}
		m_btsfPSDirty = false;
	}


	void					SceneNode::WorldToLocal(NodeTransform* pOutLocal,const NodeTransform* pWorld)
	{
		NodeTransform tsfWorld = GetTransform(TS_WORLD);
		NodeTransform tsfWorldInv;
		TransformInverse(&tsfWorldInv,&tsfWorld);
		TransformMul(pOutLocal,pWorld,&tsfWorldInv);
	}


	void					SceneNode::RotateYAxisLS(float fDegree)
	{
		Quaternion qRot;
		const Vector3 vY(0.0f,1.0f,0.0f);
		QuaternionRotationAxis(&qRot,&vY,ToRadian(fDegree));
		Rotate(qRot,TS_LOCAL);

	}

	void					SceneNode::RotateZAxisLS(float fDegree)
	{
		Quaternion qRot;
		const Vector3 vZ(0.0f,0.0f,1.0f);
		QuaternionRotationAxis(&qRot,&vZ,ToRadian(fDegree));
		Rotate(qRot,TS_LOCAL);
	}

	void					SceneNode::RotateXAxisLS(float fDegree)
	{
		Quaternion qRot;
		const Vector3 vZ(1.0f,0.0f,0.0f);
		QuaternionRotationAxis(&qRot,&vZ,ToRadian(fDegree));
		Rotate(qRot,TS_LOCAL);
	}

// 	void					SceneNode::RotateLS(float xDegree,float yDegree,float zDegree)
// 	{
// 		Quaternion qRot;
// 		EulerAngleXYZ qEuler(ToRadian(xDegree), ToRadian(yDegree), ToRadian(xDegree));
// 		QuaternionFromEulerAngleXYZ(&qRot,&qEuler);
// 		RotateLS(qRot);
// 	}

// 	void					SceneNode::RotateLS(const Quaternion& qRot)
// 	{
// 		NodeTransform tsfPS = GetTransformPS();
// 		QuaternionMultiply(&tsfPS.m_qRot,&qRot,&tsfPS.m_qRot);
// 		SetTransformPS(tsfPS);
// 	}

// 	EulerAngleXYZ			SceneNode::GetRotateLS()
// 	{
// 		EulerAngleXYZ euler;
// 		EulerAngleFromQuaternion(&euler,&GetTransformPS().m_qRot);
// 		return euler;
// 	}

// 	void  SceneNode::Pitch(float x, TRANSFORM_TYPE ts)
// 	{
// 
// 	}
// 
// 	void  SceneNode::Yaw(float x, TRANSFORM_TYPE ts)
// 	{
// 
// 	}
// 	
// 	void  SceneNode::Roll(float x, TRANSFORM_TYPE ts)
// 	{
// 
// 	}
// 
// 	void SceneNode::Move(float x)
// 	{
// 		Vector3 dir;
// 		
// 	}
// 
// 	void SceneNode::Fly(float x)
// 	{
// 
// 	}
// 
// 	void SceneNode::Right(float x)
// 	{
// 
// 	}


	void SceneNode::SyncToPhysics()
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			m_arrChildNode[i]->SyncToPhysics();
		}	
	}

	void SceneNode::SyncFromPhysics()
	{
		for (UINT i = 0; i < m_arrChildNode.size(); ++i)
		{
			m_arrChildNode[i]->SyncFromPhysics();
		}
	}

	void SceneNode::AddChildNode(SceneNode* pChildNode)
	{
		if (pChildNode == NULL)
			return;

		std::vector<SceneNode*>::iterator it = find(m_arrChildNode.begin(),m_arrChildNode.end(),pChildNode);
		if (it != m_arrChildNode.end())
			return;

		m_arrChildNode.push_back(pChildNode);

		pChildNode->SetParentSceneNode(this);
	}

	void SceneNode::UpdateAABB()
	{
		const Matrix4x4& matWorld = GetWorldMatrix();
		m_aabbWS = m_aabbLS;
		m_aabbWS.Transform(matWorld);


		for (UINT nCnt = 0; nCnt < m_arrChildNode.size(); ++nCnt)
		{
			m_arrChildNode[nCnt]->UpdateAABB();
			m_aabbWS.Merge(m_arrChildNode[nCnt]->GetAABBWS());
		}

	}

	void SceneNode::Serialize(SerializeListener& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);
		
		sl.Serialize(m_tsfPS);

		sl.SerializeObjectArray(m_arrChildNode);

		sl.EndSection();

		if ( sl.IsReading() )
		{
			for(UINT i = 0; i < m_arrChildNode.size(); ++i)
			{
				m_arrChildNode[i]->SetParentSceneNode(this);
			}
		}
	}
}
