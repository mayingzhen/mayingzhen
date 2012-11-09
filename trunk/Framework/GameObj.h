#ifndef  _GAMEOBJ__H__
#define  _GAMEOBJ__H__

#include "Framework/SceneNode.h"


namespace ma
{
	class Component;

	class GameObject : public SceneNode
	{
	public:
		GameObject(const char* pName = NULL);
		~GameObject();

	
	};
}



#endif
