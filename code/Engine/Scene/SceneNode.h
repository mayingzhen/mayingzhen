#ifndef  _SceneNode__H__
#define  _SceneNode__H__

#include "Component.h"

namespace ma
{
	class Component;
	class Scene;
	class IPhysicsObject;
	class SceneVisiter;
	class CullNode;
	class CullTree;
	class XmlFile;

	class SceneNode : public Serializable
	{
	
	public:
		SceneNode(Scene* pScene,const char* pName = NULL);

		virtual ~SceneNode();

		DECL_OBJECT(SceneNode)

		static void			RegisterAttribute();

		virtual void		Update();
		
		void				SetName(const char* pszName);
		const char*			GetName() const;

		bool				GetEnabled() const {return m_bEnable;}
		void				SetEnabled(bool b) {m_bEnable = b;}

		// Transform
		const Vector3&		GetPos() const; 
		void				SetPos(const Vector3& vPos);

		const Vector3&		GetScale() const;
		void				SetScale(const Vector3& vScale);

		const Quaternion&	GetRotation() const;
		void				SetRotation(const Quaternion& qRot);

		const Vector3&		GetPosWS() const;
		void				SetPosWS(const Vector3& vPos);

		const Quaternion&	GetRotationWS() const;
		void				SetRotationWS(const Quaternion& qRot);

		const Vector3&		GetScaleWS() const;
		void				SetScaleWS(const Vector3& vScale);

		Vector3				GetForward();
		Vector3				GetRight();		
		Vector3				GetUp();

		void                Forward(float fValue);
		void                Up(float fValue);
		void				Right(float fValue);

		void				Translate(const Vector3& vDir);

		void				LookAt(const Vector3& vPos, const Vector3& vTarget);
		void				LookAt(const Vector3& vTarget);

		const Transform&    GetTransformWS() const;
		void				SetTransformWS(const Transform& tsfWS);

		const Matrix4&		GetMatrixWS() const;

		void				RotateAround(const Vector3& vPoint, Vector3 vAxis,float angle); 
		void				Rotate(const Quaternion& delta,bool fixedAxis = false);
		void				Pitch(float angle, bool fixedAxis = false);
		void				Yaw(float angle, bool fixedAxis = false);
		void				Roll(float angle, bool fixedAxis = false);

		// Component
		template<class T>
		T*					CreateComponent();
		UINT				GetComponentNumber() {return m_arrComp.size();}
		Component*			GetComponentByIndex(UINT index) {return m_arrComp[index].get();}
		template<class T>
		void				GetTypeComponent(std::vector<T*>& arrTyeComp);
		template<class T>
		T*					GetTypeComponent();
		void				AddComponent(Component* pComponent);
		void				RemoveComponent(Component* pComponent);
						

		// Child Parent
		SceneNode*			GetParent() {return m_pParent;}
		void				AddChild(SceneNode* pChild);
		void				RemoveChild(SceneNode* pChild);
		void				RemoveAllChild();	
		SceneNode*			FindChildNode(const char* pszName);	

		Scene*				GetScene() {return m_pScene;}
		
		virtual void		Improt(TiXmlElement* pXmlElem);
		virtual void		Export(TiXmlElement* pXmlElem);		

		RefPtr<SceneNode>	Clone(const char* pName);

		void				SetUserData(const char* pszKey,void* pData);
		void*				GetUserData(const char* pszKey);

		void				SetLastVisibleFrame(UINT nFrame) {m_nLastVisibleFrame = nFrame;}
		UINT				GetLastVisibleFrame() {return m_nLastVisibleFrame;}

	private:
		void				SetParent(SceneNode* pParent);
		void				SetScene(Scene* pScene);

		void				UpdateMatWorld() const;

		void				MarkDirty();

	protected:
		typedef std::vector< RefPtr<Component> > VEC_COMP;
		typedef std::vector< RefPtr<SceneNode> > VEC_CHILD;
		typedef std::map<std::string, void*> MAP_USER_DATA;

		VEC_COMP					m_arrComp;
		SceneNode*					m_pParent;
		Scene*						m_pScene;
		VEC_CHILD					m_arrChild;

		UINT						m_nLastVisibleFrame;

		mutable Transform			m_tsfPS;
		mutable Transform			m_tsfWS;
		mutable Matrix4				m_matWS;	
		mutable bool				m_bmatWSDirty;

		bool						m_bEnable;

		MAP_USER_DATA				m_mapUserData;

		std::string					m_sName;
	};

	RefPtr<SceneNode> CreateSceneNode();
	RefPtr<SceneNode> CreateSceneNode(const char* pszXMLFile);


	template<class T>
	inline T* SceneNode::CreateComponent()
	{
		RefPtr<T> pComponent = CreateObject<T>();
		ASSERT(pComponent);
		if (pComponent == NULL)
			return NULL;

		AddComponent(pComponent.get());
		return pComponent.get();
	}

	template<class T>
	inline void SceneNode::GetTypeComponent(std::vector<T*>& arrTyeComp)
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			T* pComponent = dynamic_cast<T*>( m_arrComp[i].get() );
			if (pComponent != NULL)
			{
				arrTyeComp.push_back(pComponent);
			}
		}
	}

	template<class T>
	T*	SceneNode::GetTypeComponent()
	{
		for (UINT i = 0; i < m_arrComp.size(); ++i)
		{
			T* pComponent = dynamic_cast<T*>( m_arrComp[i].get() );
			if (pComponent != NULL)
			{
				return pComponent;
			}
		}
		return NULL;
	}
}



#endif
