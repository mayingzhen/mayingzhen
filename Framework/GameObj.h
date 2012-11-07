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
		~GameObject()

	
	};
}



#endif