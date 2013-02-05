#ifndef  _SCENE__H__
#define  _SCENE__H__


namespace ma
{
	class SceneNode;
	class IPhysicsScene;
	class Camera;

	enum DbgRenderFlag{
		eDbgRenderCollShape		= 1 << 0,
		eDbgRenderSkeleton		= 1 << 1,
	};

	class FRAMEWORK_API Scene : public Object
	{
	public:
		Scene(SceneNode* pRootNode);

		~Scene();
		
		void Update(float fElapsedTime);

		void ParalleUpdate(float fTimeElapsed);

		void FixedUpdate(float fTimeElapsed);

		void LateUpdate(float fTimeElapsed);
		
		void Render(Camera* pCmera = NULL);

		void Start();

		void Stop();

		IPhysicsScene* GetPhysicsScene() {return m_pPhyScene;}

		virtual void Serialize(SerializeListener& sl, const char* pszLable = "Scene");

	private:
		SceneNode* m_pRootNode;
		IPhysicsScene* m_pPhyScene;

		float m_fAccPhyTime;
		float m_fFixUpdateInterval;

	};
}



#endif
