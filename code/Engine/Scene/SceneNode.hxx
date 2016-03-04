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
		m_bmatWSDirty = false;	

		m_bLoadOver = false;
	}

	SceneNode::~SceneNode()
	{
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


	bool SceneNode::Improt(rapidxml::xml_node<>* pXmlElem)
	{
		Serializable::Improt(pXmlElem);

		rapidxml::xml_node<>* pXmlComp = pXmlElem->first_node("Component");
		while(pXmlComp)
		{
			const char* pszType = pXmlComp->findAttribute("ClassName");

			RefPtr<Component> pComponent = CreateObject<Component>(pszType);
			this->AddComponent(pComponent.get());

			pComponent->Improt(pXmlComp);

			pXmlComp = pXmlComp->next_sibling("Component");
		}

		rapidxml::xml_node<>* pXmlChildNode = pXmlElem->first_node("ChildNode");
		while(pXmlChildNode)
		{
			const char* pszType = pXmlChildNode->findAttribute("ClassName");

			RefPtr<SceneNode> pChildNode = CreateSceneNode();
			this->AddChild(pChildNode.get());

			pChildNode->Improt(pXmlChildNode);

			pXmlChildNode = pXmlChildNode->next_sibling("ChildNode");
		}

		return true;
	}

	bool SceneNode::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
	{
		Serializable::Export(pXmlElem,doc);

		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			rapidxml::xml_node<>* pXmlComp = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Component"));
			pXmlElem->append_node(pXmlComp);

			Component* pComp = m_arrComp[i].get();
			pComp->Export(pXmlComp,doc);
		}

		for (UINT i = 0; i < m_arrChild.size(); ++i)
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
		
		//TiXmlElement xmlEle("");
		//this->Export(&xmlEle);
		//pClone->Improt(&xmlEle);

		return pClone;
	}

	void SceneNode::SetScene(Scene* pScene)
	{
		m_pScene = pScene;

		for (VEC_CHILD::iterator it = m_arrChild.begin(); it != m_arrChild.end(); ++it)
		{
			(*it)->SetScene(pScene);
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
			
			MarkDirty();
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

		for (UINT i = 0; i < m_arrChild.size(); ++i)
		{
			if (strcmp(pszName,m_arrChild[i]->GetName()) == 0)
			{
				return m_arrChild[i].get();
			}
		}

		return NULL;
	}

	void SceneNode::Update()
	{
		if (m_bmatWSDirty)
		{
			UpdateMatWorld();
	
			m_bmatWSDirty = false;
		}

		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			m_arrComp[i]->Update();
		}

		for (UINT i = 0; i < m_arrChild.size(); ++i)
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

		MarkDirty();
	}

	const Vector3& SceneNode::GetScale() const
	{
		return m_tsfPS.m_vScale;
	}

	void SceneNode::SetScale(const Vector3& vScale)
	{
		m_tsfPS.m_vScale = vScale;

		MarkDirty();
	}

	const Quaternion& SceneNode::GetRotation() const
	{
		return m_tsfPS.m_qRot;
	}

	void SceneNode::SetRotation(const Quaternion& qRot)
	{
		m_tsfPS.m_qRot = qRot;

		MarkDirty();
	}

	const Vector3& SceneNode::GetPosWS() const
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

	const Quaternion& SceneNode::GetRotationWS() const
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

	const Vector3& SceneNode::GetScaleWS() const
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
	
	const Transform& SceneNode::GetTransformWS() const
	{
		if (m_bmatWSDirty)
		{
			UpdateMatWorld();
		}

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
		if (m_bmatWSDirty)
		{
			UpdateMatWorld();
		}
	
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

	void SceneNode::Rotate(const Quaternion& delta,bool fixedAxis/* = false*/)
	{
		Quaternion qRotate = GetRotation();
	
		if (!fixedAxis)
			qRotate = (qRotate * delta);
		else
			qRotate = (delta * qRotate);

		qRotate.normalise();

		SetRotation(qRotate);
	}


	void SceneNode::Pitch(float angle, bool fixedAxis)
	{
		Rotate(Quaternion(Radian(angle), Vector3::UNIT_X), fixedAxis);
	}

	void SceneNode::Yaw(float angle, bool fixedAxis)
	{
		Rotate(Quaternion(Radian(angle), Vector3::UNIT_Z), fixedAxis);
	}

	void SceneNode::Roll(float angle, bool fixedAxis)
	{
		Rotate(Quaternion(Radian(angle), Vector3::UNIT_Y), fixedAxis);
	}

	Vector3	SceneNode::GetForward()
	{
		return GetMatrixWS().GetColumn(1);
	}

	Vector3	SceneNode::GetRight()
	{
		return GetMatrixWS().GetColumn(0);
	}

	Vector3	SceneNode::GetUp()
	{
		return GetMatrixWS().GetColumn(2);
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
		Vector3 vUp = Vector3::UNIT_Z;

		Vector3 vDirY = vAt - vEye;
		vDirY.normalise();

		Vector3 vDirX = vDirY.crossProduct(vUp);
		vDirX.normalise();

		Vector3 vDirZ = vDirX.crossProduct(vDirY);
		vDirZ.normalise();

		Transform tsfWS;
		tsfWS.m_vPos = vEye;
		tsfWS.m_qRot = Quaternion(vDirX,vDirY,vDirZ);
		tsfWS.m_vScale = Vector3::UNIT_SCALE;
 
 		SetTransformWS(tsfWS);
	}

	void SceneNode::LookAt(const Vector3& vTarget)
	{
		LookAt(GetPos(),vTarget);
	}

	void SceneNode::MarkDirty()
	{
		m_bmatWSDirty = true;

		for (VEC_COMP::iterator iter  = m_arrComp.begin(); iter != m_arrComp.end(); ++iter)
		{
			(*iter)->MarkDirty();	
		}

		for (VEC_CHILD::iterator iter = m_arrChild.begin();iter != m_arrChild.end();++iter)
		{
			(*iter)->MarkDirty();
		}
	}

	void SceneNode::UpdateMatWorld() const
	{
		if (m_pParent == NULL)
		{
			m_matWS.makeTransform(m_tsfPS.m_vPos,m_tsfPS.m_vScale,m_tsfPS.m_qRot);
			m_tsfWS = m_tsfPS;
		}
		else
		{
			Matrix4 matPS;
			matPS.makeTransform(m_tsfPS.m_vPos,m_tsfPS.m_vScale,m_tsfPS.m_qRot);
			m_matWS = m_pParent->GetMatrixWS() * matPS;
			TransformFromMatrix(&m_tsfWS,&m_matWS);
		}

		m_bmatWSDirty = false;
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


