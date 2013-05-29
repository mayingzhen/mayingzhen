#include "AnimationUtil.h"

namespace ma
{
	namespace AnimationUtil
	{
		Action* CreateAction(const char* pszAnimPath,Skeleton* pSkeleton,const char* pszActionName)	
		{
			Animation* pAnimation = new Animation();
			AnimationData* pAnimData = new AnimationData();
			pAnimData->Load(pszAnimPath);
			pAnimation->InitWithData(pAnimData);

			AnimationInst* pAnimationInst = new AnimationInst(pAnimation,pSkeleton);
			AnimClipNode* pClipNode = new AnimClipNode();
			pClipNode->SetAnimationInst(pAnimationInst);
			Action* pAction = new Action();
			pAction->SetAnimName(pszActionName);
			pAction->SetTreeNode(pClipNode);

			return pAction;
		}
	}
}

