#ifndef  _AnimationDataCover__H__
#define  _AnimationDataCover__H__

namespace ma
{
	bool ANIMATION_API ConverteAnimDataParentToLocalSpaceAnimation(Animation* pAniData,const Skeleton* pSkel);

	bool ANIMATION_API ConverteAnimDataObjectToLocalSpaceAnimation(Animation* pAniData,const Skeleton* pSkel);
}


#endif