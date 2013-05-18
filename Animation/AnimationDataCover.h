#ifndef  _AnimationDataCover__H__
#define  _AnimationDataCover__H__

namespace ma
{
	bool ANIMATION_API ConverteAnimDataParentToLocalSpaceAnimation(AnimationData* pAniData,const SkeletonData* pSkel);

	bool ANIMATION_API ConverteAnimDataObjectToLocalSpaceAnimation(AnimationData* pAniData,const SkeletonData* pSkel);
}


#endif