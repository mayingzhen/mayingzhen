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

		// Transform
		void					SetTransform(const NodeTransform& tsf, TRANSFORM_TYPE ts = TS_PARENT);	
		const NodeTransform&	GetTransform(TRANSFORM_TYPE ts = TS_PARENT);
		const Matrix4x4&		GetWorldMatrix();
		
		void					SetPosition(const Vector3& vPos, TRANSFORM_TYPE ts = TS_PARENT);
		const Vector3&			GetPosition(TRANSFORM_TYPE ts = TS_PARENT);
		void					Translate(const Vector3& vDir, TRANSFORM_TYPE ts = TS_PARENT);

		const Quaternion&		GetRotate(TRANSFORM_TYPE ts = TS_PARENT);
		void					Rotate(const Quaternion& qRot, TRANSFORM_TYPE ts = TS_PARENT);
		void					Rotate(float xDegree,float yDegree,float zDegree, TRANSFORM_TYPE ts = TS_PARENT);
		void					RotateYAxisLS(float fDegree);
		void					RotateZAxisLS(float fDegree);
		void					RotateXAxisLS(float fDegree);

		Vector3					GetDirection(TRANSFORM_TYPE ts = TS_PARENT);
		Vector3					GetRight(TRANSFORM_TYPE ts = TS_PARENT);
		Vector3					GetUp(TRANSFORM_TYPE ts = TS_PARENT);

		void                    Move(float x);
		void                    Fly(float x);
		void                    Right(float x);

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
