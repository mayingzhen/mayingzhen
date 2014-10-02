#ifndef  __SceneVisiter_H__
#define  __SceneVisiter_H__

namespace ma
{
	class ENGINE_API SceneVisiter
	{
	public:
		virtual bool VisiteSceneNodeBegin(NodeTransform* pSceneNode)
		{
			return true;
		}

		virtual bool VisiteSceneNodeEnd(NodeTransform* pSceneNode)
		{
			return true;
		}

		virtual bool VisiteGameObjectBegin(SceneNode* pGameObj)
		{
			return true;
		}

		virtual bool VisiteGameObjectEnd(SceneNode* pGameObj)
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

