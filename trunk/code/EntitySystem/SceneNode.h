#ifndef  _SCENE_NODE__H__
#define  _SCENE_NODE__H__


namespace ma
{
	class Scene;
	class SceneVisiter;

	class ENTITYSYSTEM_API SceneNode : public Object
	{
		DECL_OBJECT(SceneNode)

	public:
		SceneNode(GameObject* pGameObj,Scene* pSene = NULL);

		~SceneNode();

		bool					TravelScene(SceneVisiter* pVisiter);

		// Transform
		void					SetTransform(const NodeTransform& tsf);	
		const NodeTransform&	GetTransform();
		const Matrix4x4&		GetWorldMatrix();

		void					Translate(const Vector3& vDir);
		void					Rotate(float xDegree,float yDegree,float zDegree);
		void					Rotate(const Quaternion& qRot);

		void					RotateYAxisLS(float fDegree);
		void					RotateZAxisLS(float fDegree);
		void					RotateXAxisLS(float fDegree);

		Vector3					GetForward();
		Vector3					GetRight();
		Vector3					GetUp();

		void                    Forward(float fValue);
		void                    Up(float fValue);
		void                    Right(float fValue);

		void					Scale(float fScale);

		void					WorldToLocal(NodeTransform* pOutLocal,const NodeTransform* pWorld);

		SceneNode*				GetParent() {return m_pParentNode;}

		Scene*					GetSene() {return m_pScene;}

		GameObject*				GetGameObject() {return m_pGameObject;}
		
		void					NotifySetParent(SceneNode* pParentNode);
		
		void					AddChildNode(SceneNode* pChildNode);

		void					RemoveChildNode(SceneNode* pChildNode);

		UINT					GetChildNumber() {return m_arrChildNode.size();}
			
		SceneNode*				GetChildByIndex(UINT index) {return m_arrChildNode[index];}
			
		const AABB&				GetAABBWS() {return m_aabbWS;}	

		const AABB&				GetAABBLS() {return m_aabbLS;}

		virtual void			Serialize(Serializer& sl, const char* pszLable = "SceneNode");

		virtual void			UpdateAABB();

	private:
		void					UpdateMatWorld();

	protected:
		std::vector<SceneNode*>	m_arrChildNode;
		SceneNode*				m_pParentNode;
		Scene*					m_pScene;

		GameObject*				m_pGameObject;
		
		Matrix4x4				m_matWorld;	//relate to world
		NodeTransform			m_tsfWS;
		bool					m_bmatWSDirty;

		AABB					m_aabbWS;	
		AABB					m_aabbLS;
	};

}


#endif
