#ifndef  _SCENE_NODE__H__
#define  _SCENE_NODE__H__


namespace ma
{
	class Scene;
	class SceneVisiter;

	class FRAMWORK_API SceneNode : public Object
	{
		DECL_OBJECT(SceneNode)

	public:
		SceneNode(Scene* pSene = NULL, const char* pNodeName = NULL);

		~SceneNode();

		bool					TravelScene(SceneVisiter* pVisiter);

		// Transform
		void					SetTransform(const NodeTransform& tsf, TRANSFORM_TYPE ts = TS_WORLD);	
		const NodeTransform&	GetTransform(TRANSFORM_TYPE ts = TS_WORLD);
		const Matrix4x4&		GetWorldMatrix();
		
		void					SetPosition(const Vector3& vPos, TRANSFORM_TYPE ts = TS_WORLD);
		const Vector3&			GetPosition(TRANSFORM_TYPE ts = TS_WORLD);
		void					Translate(const Vector3& vDir, TRANSFORM_TYPE ts = TS_WORLD);

		const Quaternion&		GetRotate(TRANSFORM_TYPE ts = TS_WORLD);
		void					Rotate(const Quaternion& qRot, TRANSFORM_TYPE ts = TS_WORLD);
		void					Rotate(float xDegree,float yDegree,float zDegree, TRANSFORM_TYPE ts = TS_WORLD);
		void					RotateYAxisLS(float fDegree);
		void					RotateZAxisLS(float fDegree);
		void					RotateXAxisLS(float fDegree);

		Vector3					GetDirection(TRANSFORM_TYPE ts = TS_WORLD);
		Vector3					GetRight(TRANSFORM_TYPE ts = TS_WORLD);
		Vector3					GetUp(TRANSFORM_TYPE ts = TS_WORLD);

		void                    Move(float x);
		void                    Fly(float x);
		void                    Right(float x);

		void					WorldToLocal(NodeTransform* pOutLocal,const NodeTransform* pWorld);

		SceneNode*				Clone(const char* pName);

		SceneNode*				GetParent() {return m_pParentNode;}

		Scene*					GetSene() {return m_pScene;}

		GameObject*				CreateGameObject();

		GameObject*				GetGameObject() {return m_pGameObject;}
		
		void					NotifySetParent(SceneNode* pParentNode);
		
		SceneNode*				AddChildNode(const char* pName);
		
		void					AddChildNode(SceneNode* pChildNode);

		UINT					GetChildNumber() {return m_arrChildNode.size();}
			
		SceneNode*				GetChildByIndex(UINT index) {return m_arrChildNode[index];}
			
		const AABB&				GetAABBWS() {return m_aabbWS;}	

		const AABB&				GetAABBLS() {return m_aabbLS;}

		virtual void			Serialize(Serializer& sl, const char* pszLable = "SceneNode");

		virtual void			UpdateAABB();

	private:
		void					UpdateMatWorld();

		void					UpdateTransformWS();

		void					UpdateTransformPS();

		void					SetTransformWSDirty();

	protected:
		std::vector<SceneNode*>	m_arrChildNode;
		SceneNode*				m_pParentNode;
		Scene*					m_pScene;

		GameObject*				m_pGameObject;
		
		// Transform
		NodeTransform			m_tsfPS;	//relate to parent node
		Matrix4x4				m_matWorld;	//relate to world
		NodeTransform			m_tsfWS;
		bool					m_btsfPSDirty;
		bool					m_btsfWSDirty;
		bool					m_bmatWSDirty;

		AABB					m_aabbWS;	
		AABB					m_aabbLS;
	};

}


#endif
