#ifndef  __SceneVisiter_H__
#define  __SceneVisiter_H__

namespace ma
{
	class ENTITYSYSTEM_API SceneVisiter
	{
	public:
		virtual bool VisiteSceneNodeBegin(SceneNode* pSceneNode)
		{
			return true;
		}

		virtual bool VisiteSceneNodeEnd(SceneNode* pSceneNode)
		{
			return true;
		}

		virtual bool VisiteGameObjectBegin(GameObject* pGameObj)
		{
			return true;
		}

		virtual bool VisiteGameObjectEnd(GameObject* pGameObj)
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

