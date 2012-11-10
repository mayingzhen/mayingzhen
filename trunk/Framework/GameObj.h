#ifndef  _GAMEOBJ__H__
#define  _GAMEOBJ__H__

#include "Framework/SceneNode.h"


namespace ma
{
	class Component;
	class Scene;

	class FRAMEWORK_API GameObject : public SceneNode
	{
	public:
		GameObject(Scene* pScene = NULL, const char* pName = NULL);

		~GameObject();

		void Render();

		void Update();

		void ParalleUpdate(float fTimeElapsed);

		void FixedUpdate(float fTimeElapsed);

		void LateUpdate(float fTimeElapsed);

		void Start();

		void Stop();

		void AddComponent(Component* pComponent);
			
	private:
		std::vector<Component*> m_vComponents;
	
	};
}



#endif
