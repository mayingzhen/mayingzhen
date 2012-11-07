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
		GameObject(const char* pName = NULL);
		~GameObject();

		void Render();

		void Update();

		virtual void Render();

		virtual void Update(float fTimeElapsed);

		virtual void ParalleUpdate(float fTimeElapsed);

		virtual void FixedUpdate(float fTimeElapsed);

		virtual void LateUpdate(float fTimeElapsed);

		virtual void Start();

		virtual void Stop();

		void AddComponent(Component* pComponent);

	private:
		std::vector<Component*> m_vComponents;
	};
}



#endif