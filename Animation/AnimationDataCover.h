#ifndef  _AnimationDataCover__H__
#define  _AnimationDataCover__H__

namespace ma
{
	bool ANIMATION_API ConverteAnimDataParentToLocalSpaceAnimation(AnimationData* pAniData,Skeleton* pSkel);

	bool ANIMATION_API ConverteAnimDataObjectToLocalSpaceAnimation(AnimationData* pAniData,Skeleton* pSkel);
}


#endif