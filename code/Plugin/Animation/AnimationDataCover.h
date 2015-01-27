#ifndef  _AnimationDataCover__H__
#define  _AnimationDataCover__H__

namespace ma
{
	bool ConverteAnimDataParentToLocalSpaceAnimation(Animation* pAniData,const Skeleton* pSkel);

	bool ConverteAnimDataObjectToLocalSpaceAnimation(Animation* pAniData,const Skeleton* pSkel);
}


#endif