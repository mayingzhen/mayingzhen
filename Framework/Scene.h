#ifndef  _SCENE__H__
#define  _SCENE__H__

namespace ma
{
	class SceneNode;
	class PhysicsScene;
}

namespace ma
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		
		void Tick();
		
		void Render();

	private:
		SceneNode* m_pRootNode;
		PhysicsScene* m_pPhyScene;

		float m_fAccPhyTime;
		float m_fFixUpdateInterval;

	};
}



#endif
