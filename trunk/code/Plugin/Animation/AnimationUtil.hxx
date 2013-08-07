#include "AnimationUtil.h"

namespace ma
{
	namespace AnimationUtil
	{
		Action* CreateAction(const char* pszAnimPath,Skeleton* pSkeleton,const char* pszActionName)	
		{
			Animation* pAnimation = DeclareResource<Animation>(pszAnimPath);
			pAnimation->LoadSync();

			AnimationClip* pAnimationInst = new AnimationClip(pAnimation,pSkeleton);
			AnimClipNode* pClipNode = new AnimClipNode();
			pClipNode->SetAnimationClip(pAnimationInst);
			Action* pAction = new Action(pszActionName);
			pAction->SetTreeNode(pClipNode);

			return pAction;
		}
	}
}

