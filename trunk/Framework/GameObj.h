#ifndef  _GAMEOBJ__H__
#define  _GAMEOBJ__H__

#include "SceneNode.h"

namespace ma
{
	class Component;
}

namespace ma
{
	class GameObject : public SceneNode
	{
	public:
		GameObject();
		~GameObject();

		void Render();

		void Update();

		void AddComponent(Component* pComponent);

	private:
		std::vector<Component*> m_vComponents;
	};
}



#endif