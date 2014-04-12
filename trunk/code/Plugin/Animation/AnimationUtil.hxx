#include "AnimationUtil.h"
#include "AnimationTree/AnimClipNode.h"

namespace ma
{
	namespace AnimationUtil
	{
		SkelAnimtion* CreateAction(const char* pszAnimPath,Skeleton* pSkeleton,const char* pszActionName)	
		{
			ref_ptr<Animation> pAnimation = DeclareResource<Animation>(pszAnimPath);
			pAnimation->LoadSync();

			AnimationClip* pAnimationInst = new AnimationClip(pAnimation,pSkeleton);
			AnimClipNode* pClipNode = new AnimClipNode(pSkeleton);
			pClipNode->SetAnimationClip(pAnimationInst);
			SkelAnimtion* pAction = new SkelAnimtion(pszActionName,pSkeleton);
			pAction->SetTreeNode(pClipNode);

			return pAction;
		}
	}
}

