#ifndef  _SCENE_NODE__H__
#define  _SCENE_NODE__H__


namespace ma
{
	class Scene;

	class ENGINE_API SceneNode : public Object
	{
		DECL_OBJECT(SceneNode)

	public:
		SceneNode(Scene* pSene = NULL, const char* pNodeName = NULL);

		~SceneNode();

		virtual void			Update();

		virtual void			ParalleUpdate(float fTimeElapsed);

		virtual void			FixedUpdate(float fTimeElapsed);

		virtual void			LateUpdate(float fTimeElapsed);

		virtual void			Render();

		virtual void			Start();

		virtual void			Stop();

		virtual void			SyncToPhysics();

		virtual void			SyncFromPhysics();

		virtual void			TravelProperty(PropertyVisitor* pVisitor);

		// TransformPS
		void					SetTransformPS(const NodeTransform& tsfPS);
		const NodeTransform&	GetTransformPS();
		void					SetPositionPS(const Vector3& pPosPS);
		void					TranslatePS(const Vector3& vDirPS);
		const Vector3&		GetPositionPS();

		// TransformWS 
		void					SetTransformWS(const NodeTransform& tsfWS);
		const NodeTransform&	GetTransformWS();
		void					SetPositionWS(const Vector3& vPosWS);
		void					TranslateWS(const Vector3& vDirWS);
		const Vector3&			GetPositionWS();
		const Matrix4x4&		GetWorldMatrix();

		// TransformLS
		void					TranslateLS(const Vector3& vDirLS);	
		void					RotateYAxisLS(float fDegree);
		void					RotateZAxisLS(float fDegree);
		void					RotateLS(float xDegree,float yDegree,float zDegree);
		void					RotateLS(const Quaternion& qRot);
		EulerAngleXYZ			GetRotateLS();
		void					WorldToLocal(NodeTransform* pOutLocal,const NodeTransform* pWorld);

		Scene*					GetScene() {return m_pScene;}

		SceneNode*				GetSceneNode() {return m_pParentNode;}
		
		void					SetParentSceneNode(SceneNode* pParentNode) {m_pParentNode = pParentNode;}
		
		void					AddChildNode(SceneNode* pChildNode);

		const AABB&				GetAABBWS() {return m_aabbWS;}	

		const AABB&				GetAABBLS() {return m_aabbLS;}

		virtual void			Serialize(SerializeListener& sl, const char* pszLable = "SceneNode");

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
