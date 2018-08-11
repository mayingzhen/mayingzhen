#include "SceneNode.h"
#include "Component.h"
#include "CullTree.h"

namespace ma
{
	SceneNode::SceneNode(Scene* pScene,const char* pName)
	{
		m_pScene = pScene;
		m_sName = pName ? pName : "";
		
		m_pParent = NULL;
		m_nLastVisibleFrame = 0;

		m_matWS = Matrix4::IDENTITY;
		

		m_nNeedChange = CT_PART;
		m_nInheritType = IT_POS | IT_SCALE | IT_ROTATE;
	}

	SceneNode::~SceneNode()
	{
		for (uint32_t i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->SetSceneNode(NULL);
		}
		m_arrComp.clear();

		this->RemoveAllChild();
		
		if (m_pParent)
		{
			m_pParent->RemoveChild(this);
		}
		m_pParent = NULL;
	}
 
	void SceneNode::RegisterAttribute()
	{
 		ACCESSOR_ATTRIBUTE(SceneNode, "Is Enabled", GetEnabled, SetEnabled, bool, true, AM_DEFAULT);
 		ACCESSOR_ATTRIBUTE(SceneNode, "Name", GetName, SetName, const char*, NULL, AM_DEFAULT);
		REF_ACCESSOR_ATTRIBUTE(SceneNode, "Position", GetPos, SetPos, Vector3, Vector3::ZERO, AM_DEFAULT);
		REF_ACCESSOR_ATTRIBUTE(SceneNode, "Rotation", GetRotation, SetRotation, Quaternion, Quaternion::IDENTITY, AM_DEFAULT);
		REF_ACCESSOR_ATTRIBUTE(SceneNode, "Scale", GetScale, SetScale, Vector3, Vector3::UNIT_SCALE, AM_DEFAULT);
	}

	void SceneNode::SetName(const char* pszName)
	{
		m_sName = pszName ? pszName : "";
	}

	const char*	SceneNode::GetName() const
	{
		return m_sName.c_str();
	}

	void SceneNode::AddComponent(Component* pComponent)
	{
		if (pComponent == NULL)
			return;

		VEC_COMP::iterator it = std::find(m_arrComp.begin(),m_arrComp.end(),pComponent);
		if (it != m_arrComp.end())
			return;

		m_arrComp.push_back(pComponent);

		pComponent->SetSceneNode(this);
	}

	void SceneNode::RemoveComponent(Component* pComponent)
	{
		if (pComponent == NULL)
			return;

		VEC_COMP::iterator it = find(m_arrComp.begin(),m_arrComp.end(),pComponent);
		if (it != m_arrComp.end())
			return;

		m_arrComp.erase(it);

		pComponent->SetSceneNode(NULL);
	}


	bool SceneNode::Import(rapidxml::xml_node<>* pXmlElem)
	{
		Serializable::Import(pXmlElem);

		rapidxml::xml_node<>* pXmlComp = pXmlElem->first_node("Component");
		while(pXmlComp)
		{
			const char* pszType = pXmlComp->findAttribute("ClassName");

			RefPtr<Component> pComponent = CreateObject<Component>(pszType);
			this->AddComponent(pComponent.get());

			pComponent->Import(pXmlComp);

			pXmlComp = pXmlComp->next_sibling("Component");
		}

		rapidxml::xml_node<>* pXmlChildNode = pXmlElem->first_node("ChildNode");
		while(pXmlChildNode)
		{
			//const char* pszType = pXmlChildNode->findAttribute("ClassName");

			RefPtr<SceneNode> pChildNode = CreateSceneNode();
			this->AddChild(pChildNode.get());

			pChildNode->Import(pXmlChildNode);

			pXmlChildNode = pXmlChildNode->next_sibling("ChildNode");
		}

		return true;
	}

	bool SceneNode::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		Serializable::Export(pXmlElem,doc);

		for (uint32_t i = 0; i < m_arrComp.size(); ++i)
		{
			rapidxml::xml_node<>* pXmlComp = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Component"));
			pXmlElem->append_node(pXmlComp);

			Component* pComp = m_arrComp[i].get();
			pComp->Export(pXmlComp,doc);
		}

		for (uint32_t i = 0; i < m_arrChild.size(); ++i)
		{
			rapidxml::xml_node<>* pXmlChildNode = doc.allocate_node(rapidxml::node_element, doc.allocate_string("ChildNode"));
			pXmlElem->append_node(pXmlChildNode);

			SceneNode* pChildNode = m_arrChild[i].get();
			pChildNode->Export(pXmlChildNode,doc);
		}

		return true;
	}

	void SceneNode::SetUserData(const char* pszKey,void* pData)
	{
		m_mapUserData[pszKey] = pData;
	}

	void* SceneNode::GetUserData(const char* pszKey)
	{
		return m_mapUserData[pszKey];
	}

	RefPtr<SceneNode> SceneNode::Clone(const char* pName)
	{
		RefPtr<SceneNode> pClone = CreateSceneNode();
		
// 		TiXmlElement xmlEle("");
// 		this->Export(&xmlEle);
// 		pClone->Import(&xmlEle);

		return pClone;
	}

	void SceneNode::SetScene(Scene* pScene)
	{
		m_pScene = pScene;

		for (uint32_t i = 0; i < m_arrChild.size(); ++i)
		{
			m_arrChild[i]->SetScene(pScene);
		}

		for (uint32_t i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->SetSceneNode(this);
		}
	}

	void SceneNode::SetParent(SceneNode* pParent)
	{
		if (m_pParent == pParent)
		{
			return;
		}

		m_pParent = pParent;
		if (m_pParent !=  NULL)
		{
			SetScene(m_pParent->GetScene());
			
			this->SetNeedChange(CT_FROMPARENT);
		}
		else
		{
			SetScene(NULL);
		}
	}

	void SceneNode::AddChild(SceneNode* pChild)
	{
		if (pChild == NULL)
			return;
	
		if (pChild->GetParent() == this)
			return;

		if (pChild->GetParent() != NULL)
		{
			pChild->GetParent()->RemoveChild(pChild);
		}

		ASSERT(pChild->GetParent() == NULL);

		m_arrChild.push_back(pChild);
		pChild->SetParent(this);
	}

	void SceneNode::RemoveAllChild()
	{
		for (VEC_CHILD::iterator iter = m_arrChild.begin();iter != m_arrChild.end();iter++)
		{
			RefPtr<SceneNode> pGameObject = *iter;
			pGameObject->SetParent(NULL);
		}
		m_arrChild.clear();
	}

	void SceneNode::RemoveChild(SceneNode* pChild)
	{
		if (pChild == NULL)	
		{
			return;
		}

		VEC_CHILD::iterator iter = find(m_arrChild.begin(), m_arrChild.end(), pChild);
		if (iter == m_arrChild.end())
		{
			return;
		}

		m_arrChild.erase(iter);
		pChild->SetParent(NULL);

	}

	SceneNode* SceneNode::FindChildNode(const char* pszName)
	{
		ASSERT(pszName);
		if (pszName == NULL)
			return NULL;

		for (uint32_t i = 0; i < m_arrChild.size(); ++i)
		{
			if (strcmp(pszName,m_arrChild[i]->GetName()) == 0)
			{
				return m_arrChild[i].get();
			}
		}

		return NULL;
	}

	void SceneNode::SetNeedChange(CHANGE_TYPE eChangeType)
	{
		m_nNeedChange |= eChangeType;

		for (uint32_t i = 0; i < m_arrChild.size(); ++i)
		{
			m_arrChild[i]->SetNeedChange(eChangeType);
		}

		for (uint32_t i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->SetNeedChange(eChangeType);
		}
	}

	bool SceneNode::BeginMatrix() const
	{
		return m_nNeedChange != CT_NONE;
	}

	const Matrix4& SceneNode::CalcMatrix()
	{
		this->UpdateWorldMatrix();
		m_nNeedChange = CT_NONE;

		return m_matWS;
	}

	void SceneNode::EndMatrix()
	{
		m_nNeedChange = CT_NONE;
		ASSERT(m_nNeedChange == CT_NONE && "EndMatrix必须在if(BeginMatrix())里CalcMatrix后调用！");
// 		if (m_pCallback)
// 		{
// 			m_pCallback->OnNodeEndMatrix(this);
// 		}
	}

	void SceneNode::UpdateWorldMatrix() const
	{
		if (m_nNeedChange == CT_NONE || m_nNeedChange == CT_NOTIFY)
		{
			return;
		}

		Matrix4 matLocal;
		matLocal.makeTransform(m_tsfPS.m_vPos, m_tsfPS.m_vScale, m_tsfPS.m_qRot);

		if (m_pParent != NULL)
		{
			if (m_nInheritType == IT_NONE)
			{
				m_matWS = matLocal;
			}
			else if (m_nInheritType == (IT_POS | IT_SCALE | IT_ROTATE))
			{
				m_matWS = m_pParent->GetMatrixWS() * matLocal;
			}
			else
			{
				Quaternion qParentRotate = Quaternion::IDENTITY;
				Vector3 vParentScale = Vector3::UNIT_SCALE;
				Vector3 vParentPos = Vector3::ZERO;
				if ((m_nInheritType&IT_ROTATE) != 0)
				{
					qParentRotate = m_pParent->GetTransformWS().m_qRot;
				}

				if ((m_nInheritType&IT_SCALE) != 0)
				{
					vParentScale = m_pParent->GetTransformWS().m_vScale;
				}

				if ((m_nInheritType&IT_POS) != 0)
				{
					vParentPos = m_pParent->GetTransformWS().m_vPos;
				}
				Matrix4 matParent;
				matParent.makeTransform(vParentPos, vParentScale, qParentRotate);
				m_matWS = matParent*matLocal;
			}

		}
		else
		{
			m_matWS = matLocal;
		}
		m_matWS.decomposition(m_tsfWS.m_vPos, m_tsfWS.m_vScale, m_tsfWS.m_qRot);

		m_nNeedChange = CT_NOTIFY;

		// change children
		for (uint32_t i = 0; i < m_arrChild.size(); ++i)
		{
			m_arrChild[i]->SetNeedChange(CT_FROMPARENT);
		}

		for (uint32_t i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->SetNeedChange(CT_FROMPARENT);
		}
	}


	void SceneNode::Update()
	{
		if (this->BeginMatrix())
		{
			this->CalcMatrix();
			this->EndMatrix();
		}

		for (uint32_t i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->Update();
		}

		for (uint32_t i = 0; i < m_arrChild.size(); ++i)
		{
			m_arrChild[i]->Update();
		}			
	}

	const Vector3& SceneNode::GetPos() const
	{
		return m_tsfPS.m_vPos;
	}

	void SceneNode::SetPos(const Vector3& vPos)
	{
		m_tsfPS.m_vPos = vPos;

		this->SetNeedChange(CT_PART);
	}

	const Vector3& SceneNode::GetScale() const
	{
		return m_tsfPS.m_vScale;
	}

	void SceneNode::SetScale(const Vector3& vScale)
	{
		m_tsfPS.m_vScale = vScale;

		this->SetNeedChange(CT_PART);
	}

	const Quaternion& SceneNode::GetRotation() const
	{
		return m_tsfPS.m_qRot;
	}

	void SceneNode::SetRotation(const Quaternion& qRot)
	{
		m_tsfPS.m_qRot = qRot;

		this->SetNeedChange(CT_PART);
	}

	const Vector3& SceneNode::GetPosWS()
	{
		return GetTransformWS().m_vPos;
	}

	void SceneNode::SetPosWS(const Vector3& vPos)
	{
		Vector3 vPosPS;
		if (m_pParent == NULL)
		{
			vPosPS = vPos;
		}
		else
		{
			vPosPS = m_pParent->GetMatrixWS().inverse() * vPos;
		}

		SetPos(vPosPS);
	}

	const Quaternion& SceneNode::GetRotationWS()
	{
		return GetTransformWS().m_qRot;
	}

	void SceneNode::SetRotationWS(const Quaternion& qRot)
	{
		Quaternion qRotPS;
		if (m_pParent == NULL)
		{
			qRotPS = qRot;
		}
		else
		{
			qRotPS = m_pParent->GetRotationWS().Inverse() * qRot;
		}
		
		SetRotation(qRotPS);
	}

	const Vector3& SceneNode::GetScaleWS()
	{
		return GetTransformWS().m_vScale;
	}

	void SceneNode::SetScaleWS(const Vector3& vScale)
	{
		Vector3 vScalePS;
		if (m_pParent == NULL)
		{
			vScalePS = vScale;
		}
		else
		{
			vScalePS = vScale / m_pParent->GetScaleWS();
		}

		SetScale(vScalePS);
	}	
	
	const Transform& SceneNode::GetTransformWS()
	{
		UpdateWorldMatrix();

		return m_tsfWS;
	}

	void SceneNode::SetTransformWS(const Transform& tsfWS)
	{
		SetPosWS(tsfWS.m_vPos);

		SetRotationWS(tsfWS.m_qRot);

		SetScaleWS(tsfWS.m_vScale);
	}

	const Matrix4& SceneNode::GetMatrixWS() const
	{
		UpdateWorldMatrix();
	
		return m_matWS;
	}

	void SceneNode::RotateAround(const Vector3& vPoint, Vector3 vAxis,float angle)
	{
		float fRadian = DegreesToRadians(angle);

		Transform tsf;
		tsf.m_vPos = vPoint;
		tsf.m_qRot = Quaternion( Radian(fRadian), vAxis );

		Transform tsfWS = GetTransformWS();
		TransformMul(&tsfWS,&tsf,&tsfWS);
		SetTransformWS(tsfWS);
	}

	Vector3	SceneNode::GetForward() const
	{
		return GetMatrixWS().GetColumn(2);
	}

	Vector3	SceneNode::GetRight() const
	{
		return GetMatrixWS().GetColumn(0);
	}

	Vector3	SceneNode::GetUp() const
	{
		return GetMatrixWS().GetColumn(1);
	}

	void  SceneNode::Forward(float x) 
	{
		Translate(GetForward() * x);
	}

	void  SceneNode::Up(float x)
	{
		Translate(GetUp() * x);
	}

	void  SceneNode::Right(float x)
	{
		Translate(GetRight() * x);
	}

	void SceneNode::Translate(const Vector3& vTrans)
	{	
		SetPos(GetPos() + vTrans);
	}

	void SceneNode::LookAt(const Vector3& vEye, const Vector3& vAt)
	{
		Matrix4 matView = Math::MakeLookAtMatrixRH(vEye, vAt, Vector3::UNIT_Y);
		Matrix4 matWorld = matView.inverse();

		Transform tsfWS;
		matWorld.decomposition(tsfWS.m_vPos, tsfWS.m_vScale, tsfWS.m_qRot);
		
		tsfWS.m_vScale = GetScaleWS();

 		SetTransformWS(tsfWS);
		
// 		Vector3 vUp = Vector3::UNIT_Y;
// 
// 		Vector3 lookDir = vAt - vEye;
// 		if (lookDir == Vector3::ZERO)
// 			return;
// 		
// 		Quaternion rot = Quaternion::IDENTITY;
// 
// 		Vector3 forward = lookDir.normalisedCopy();
// 
// 		Vector3 v = forward.crossProduct(vUp);
// 		// If direction & upDirection are parallel and crossproduct becomes zero, use FromRotationTo() fallback
// 		if (v.squaredLength() >= 1e-6f)
// 		{
// 			v.normalise();
// 			Vector3 up = v.crossProduct(forward);
// 			Vector3 right = up.crossProduct(forward);
// 			rot.FromAxes(right, up, forward);
// 		}
// 		else
// 		{
// 			ASSERT(false);
// 		}
// 
// 
// 		Transform tsfWS;
// 		tsfWS.m_vPos = vEye;
// 		tsfWS.m_qRot = rot;
// 		tsfWS.m_vScale = GetScaleWS();
// 
//  		SetTransformWS(tsfWS);
	}

	void SceneNode::LookAt(const Vector3& vTarget)
	{
		LookAt(GetPos(),vTarget);
	}

	RefPtr<SceneNode> CreateSceneNode()
	{
		SceneNode* pSceneNode = new SceneNode(NULL,NULL);
		return pSceneNode;
	}

	RefPtr<SceneNode> CreateSceneNode(const char* pszXMLFile)
	{
		SceneNode* pSceneNode = new SceneNode(NULL,NULL);
		pSceneNode->LoadFromXML(pszXMLFile);
		return pSceneNode;
	}

}


