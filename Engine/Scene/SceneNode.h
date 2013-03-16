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

		// TransformPS
		void					SetTransformPS(const maNodeTransform& tsfPS);
		const maNodeTransform&	GetTransformPS();
		void					SetPositionPS(const D3DXVECTOR3& pPosPS);
		void					TranslatePS(const D3DXVECTOR3& vDirPS);
		const D3DXVECTOR3&		GetPositionPS();

		// TransformWS 
		void					SetTransformWS(const maNodeTransform& tsfWS);
		const maNodeTransform&	GetTransformWS();
		void					SetPositionWS(const D3DXVECTOR3& vPosWS);
		void					TranslateWS(const D3DXVECTOR3& vDirWS);
		const D3DXVECTOR3&		GetPositionWS();
		const D3DXMATRIX&		GetWorldMatrix();

		// TransformLS
		void					TranslateLS(const D3DXVECTOR3& vDirLS);	
		void					RotateYAxisLS(float fDegree);
		void					RotateZAxisLS(float fDegree);
		void					RotateLS(float xDegree,float yDegree,float zDegree);
		void					RotateLS(const D3DXQUATERNION& qRot);
		EulerAngleXYZ			GetRotateLS();
		void					WorldToLocal(maNodeTransform* pOutLocal,const maNodeTransform* pWorld);

		Scene*					GetScene() {return m_pScen;}

		SceneNode*				GetSceneNode() {return m_pParentNode;}
		
		void					SetParentSceneNode(SceneNode* pParentNode) {m_pParentNode = pParentNode;}
		
		void					AddChildNode(SceneNode* pChildNode);

		virtual void			Serialize(SerializeListener& sl, const char* pszLable = "SceneNode");

	private:
		void					UpdateMatWorld();

		void					UpdateTransformWS();

		void					UpdateTransformPS();

		void					SetTransformWSDirty();

	protected:
		std::vector<SceneNode*>	m_arrChildNode;
		SceneNode*				m_pParentNode;
		Scene*					m_pScen;
		
		// Transform
		maNodeTransform			m_tsfPS;	//relate to parent node
		D3DXMATRIX				m_matWorld;	//relate to world
		maNodeTransform			m_tsfWS;
		bool					m_btsfPSDirty;
		bool					m_btsfWSDirty;
		bool					m_bmatWSDirty;

		AABB					m_aabb;	
	};

}


#endif
