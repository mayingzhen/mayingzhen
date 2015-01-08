#include "AnimationUtil.h"
#include "AnimationTree/AnimClipNode.h"

namespace ma
{
	namespace AnimationUtil
	{
		SkelAnimtion* CreateAction(const char* pszAnimPath,Skeleton* pSkeleton,const char* pszActionName)	
		{
			RefPtr<Animation> pAnimation = DeclareResource<Animation>(pszAnimPath);
			pAnimation->LoadSync();

			AnimationClip* pAnimationInst = new AnimationClip();
			AnimClipNode* pClipNode = new AnimClipNode();
			pClipNode->SetAnimationClip(pAnimationInst);
			SkelAnimtion* pAction = new SkelAnimtion();
			pAction->SetAnimName(pszAnimPath);
			pAction->SetSkeletion(pSkeleton);
			pAction->SetTreeNode(pClipNode);

			return pAction;
		}
	}
}

