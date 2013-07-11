#include "AnimationUtil.h"

namespace ma
{
	namespace AnimationUtil
	{
		Action* CreateAction(const char* pszAnimPath,Skeleton* pSkeleton,const char* pszActionName)	
		{
			Animation* pAnimation = new Animation();
			AnimationData* pAnimData = static_cast<AnimationData*>(ResourceManager::DeclareResource(pszAnimPath));
			pAnimData->Load();
			pAnimData->LoadImp(pAnimData->GetDataStream());
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

