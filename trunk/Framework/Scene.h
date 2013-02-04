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

	class FRAMEWORK_API Scene
	{
	public:
		Scene(SceneNode* pRootNode);

		~Scene();
		
		void Update(float fElapsedTime);
		
		void Render(Camera* pCmera = NULL);

		void Start();

		void Stop();

		IPhysicsScene* GetPhysicsScene() {return m_pPhyScene;}

	private:
		SceneNode* m_pRootNode;
		IPhysicsScene* m_pPhyScene;

		float m_fAccPhyTime;
		float m_fFixUpdateInterval;

	};
}



#endif