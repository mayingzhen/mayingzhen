#ifndef  _SCENE_NODE__H__
#define  _SCENE_NODE__H__


namespace ma
{
	class Scene;

	class FRAMEWORK_API SceneNode : public Object
	{
		DECL_OBJECT(SceneNode)

	public:
		SceneNode(Scene* pSene = NULL, const char* pNodeName = NULL);

		~SceneNode();

		virtual void Update();

		virtual void ParalleUpdate(float fTimeElapsed);

		virtual void FixedUpdate(float fTimeElapsed);

		virtual void LateUpdate(float fTimeElapsed);

		virtual void Render();

		virtual void Start();

		virtual void Stop();

		void TranslateLS(const D3DXVECTOR3& vDeltaLS);

		void TranslatePS(const D3DXVECTOR3& vDeltaPS);

		void TranslateWS(const D3DXVECTOR3& vDeltaWS);

		void RotateLS(const D3DXQUATERNION* pRot);

		void RotateLS(float x,float y,float z);

		maNodeTransform GetTransformWS();

		void SetTransformWS(const maNodeTransform& TSFWS);
		
		void SetTransformPS(const maNodeTransform& TSFPS) {m_tsfPS = TSFPS;}
		
		const maNodeTransform& GetTransformPS() {return m_tsfPS;}

		const D3DXMATRIX& GetWorldMatrix() {return m_matWorld;}

		const D3DXVECTOR3* GetWorldPos() {return (const D3DXVECTOR3*)(&m_matWorld._41);}
	
		void SyncWorld();

		virtual void SyncToPhysics();

		virtual void SyncFromPhysics();

		Scene* GetScene() {return m_pScen;}

		SceneNode* GetSceneNode() {return m_pParentNode;}

		void SetParentSceneNode(SceneNode* pParentNode) {m_pParentNode = pParentNode;}

		void AddChildNode(SceneNode* pChildNode);

		virtual void Serialize(SerializeListener& sl, const char* pszLable = "SceneNode");

	private:
		std::vector<SceneNode*> m_vChildNodes;

		SceneNode* m_pParentNode;

		Scene* m_pScen;
		
		maNodeTransform m_tsfPS; // relate to parent node
		D3DXMATRIX m_matWorld;    // relate to world
	};

}


#endif