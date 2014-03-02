#ifndef  _CullNode__H__
#define  _CullNode__H__

namespace ma
{
	class GameObject;

	class CullNode 
	{
	public:
		virtual void AddObject(GameObject* pObject) = 0;
		
		virtual void RemoveObject(GameObject* pObject) = 0;
	};


}

#endif
