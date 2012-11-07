#ifndef  _SCENE__H__
#define  _SCENE__H__

namespace ma
{
	class SceneNode;
	class IPhysicsScene;
}

namespace ma
{
	class Scene
	{
	public:
		Scene(const SceneNode* pRootNode);
		~Scene();
		
		void Update(float fElapsedTime);
		
		void Render();

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
