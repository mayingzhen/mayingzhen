#ifndef  __SceneVisiter_H__
#define  __SceneVisiter_H__

namespace ma
{
	class ENGINE_API SceneVisiter
	{
	public:
		virtual bool VisiteSceneNode(SceneNode* pGameObj)
		{
			return true;
		}

		virtual bool VisiteComponent(Component* pComp)
		{
			return true;
		}
	};

}

#endif // __SceneVisiter_H__

