#ifndef  _SCENE__H__
#define  _SCENE__H__

namespace ma
{
	class SceneNode;
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
		std::vector<SceneNode*> m_vSceneNodes;
		

	};
}



#endif
