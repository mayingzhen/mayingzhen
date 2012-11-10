#ifndef  _SCENE__H__
#define  _SCENE__H__


namespace ma
{
	class SceneNode;
	class IPhysicsScene;
	class Camera;

	class FRAMEWORK_API Scene
	{
	public:
		Scene(SceneNode* pRootNode);
		~Scene();
		
		void Update(float fElapsedTime);
		
		void Render(Camera* pCmera = NULL);

		void Start();

		void Stop();

	private:
		SceneNode* m_pRootNode;
		IPhysicsScene* m_pPhyScene;

		float m_fAccPhyTime;
		float m_fFixUpdateInterval;

	};
}



#endif
