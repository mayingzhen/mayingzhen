#ifndef  __RenderQueueBuilder_H__
#define  __RenderQueueBuilder_H__

namespace ma
{

	class RenderQueueBuilder : public SceneVisiter
	{
		virtual bool VisiteSceneNodeBegin(SceneNode* pSceneNode)
		{
			return true;
		}

		virtual bool VisiteComponent(Component* pComp)
		{
			pComp->BuildRenderItem();
			return true;
		}
	};
}

#endif // __SceneVisiter_H__
