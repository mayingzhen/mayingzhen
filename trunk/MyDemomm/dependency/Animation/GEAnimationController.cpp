#include "GEAnimationcontroller.h"
#include "GEAnimationSet.h"
#include "GESkeleton.h"
#include "GEUtil.h"

GEAnimationController::GEAnimationController(void)
{
	mLoop=true;
	mBeingBlending=false;
	mBeingTranslating=false;

	mActiveTimeScale=1.0f;
	mBlendedTimeScale=1.0f;
	mTranslatedTimeScale = 1.0f;

	mTranslatingTime = 0.f;

	mActiveAnimationLength=0.0f;
	mBlendedAnimationLength=0.0f;
	mTranslatedAnimationLength=0.0f;

	mActiveAnimation=NULL;
	mBlendedAnimation=NULL;
	mTranslatedAnimation=NULL;

	mSkeleton = NULL;
	mAnimationSet = NULL;

	mActiveAnimationName = "";
}

GEAnimationController::~GEAnimationController(void)
{
	//if (mSkeleton != NULL)
	//	GESkeletonManager::GetSingleton().RemoveResource(mSkeleton->GetHandle());
	//if (mAnimationSet != NULL)
	//	GEAnimationSetManager::GetSingleton().RemoveResource(mAnimationSet->GetHandle());

	mActiveAnimation		= NULL;
	mBlendedAnimation		= NULL;
	mTranslatedAnimation	= NULL;
	mSkeleton				= NULL;
	mAnimationSet			= NULL;
}

bool GEAnimationController::SetActiveAnimation(const string& animationName, float beginTime)
{
	mBeingBlending = false;
	mBeingTranslating = false;

	mActiveAnimation=mAnimationSet->getAnimation(animationName);
	if(mActiveAnimation==NULL) return false;
	mActiveAnimationTime=beginTime;
	mActiveAnimationName = animationName;
	mActiveAnimationLength=mActiveAnimation->getLength();
	return true;
}

bool GEAnimationController::AddTime(float delta)
{	
	mActiveAnimationTime += delta * mActiveTimeScale;

	if(mBeingBlending)  // 如果正在blending
	{
		if(mActiveAnimationTime>mActiveAnimationLength)
		{
			if(mLoop==true)
				mActiveAnimationTime=0;
			else
				mActiveAnimationTime=mActiveAnimationLength;
		}

		mBlendedAnimationTime += delta * mBlendedTimeScale;
		mBlendingTime+=delta;
		if(mBlendedAnimationTime>mBlendedAnimationLength)
		{
			if(mLoop==true)
				mBlendedAnimationTime=0;
			else
				mBlendedAnimationTime=mBlendedAnimationLength;
		}

		if(mBlendingTime>=mBlendInterval)	// 完成blend了
		{
			mActiveAnimation=mBlendedAnimation;
			mBlendedAnimation=NULL;
			mActiveAnimationTime=mBlendedAnimationTime;
			mBeingBlending=false;
		}
		return false;
	}

	if(mBeingTranslating)
	{
		mTranslatingTime+=delta*mTranslatedTimeScale;

		if(mTranslatingTime >= mTranslateInterval)
		{
			mActiveAnimation=mTranslatedAnimation;
			mTranslatedAnimation=NULL;
			mActiveAnimationTime=mTranslatingTime;
			mActiveTimeScale = mTranslatedTimeScale;
			mBeingTranslating=false;
			mActiveAnimationLength = mActiveAnimation->getLength();
		}
		return false;
	}

	if(mActiveAnimationTime>mActiveAnimationLength)
	{
		if(mLoop==true)
		{
			mActiveAnimationTime=0;
			return false;
		}
		else
		{
			mActiveAnimationTime=mActiveAnimationLength;
			return true;
		}
	}

	return false;
}

bool GEAnimationController::BlendTo(const string& targetAnimationName,
									float blendTime, float timeScale)
{
	if(mBeingBlending) return false;	// 已经在blending了
	
	mBlendInterval=blendTime;
	mBlendingTime=0.f;
	mBlendedAnimationTime=0.f;
	mBlendedTimeScale=timeScale;	
	mBlendedAnimation=mAnimationSet->getAnimation(targetAnimationName);
	if(mBlendedAnimation==NULL) return false;
	mBlendedAnimationLength=mBlendedAnimation->getLength();
	mBeingBlending=true;
	return true;
}

bool GEAnimationController::BlendToEnd(const string& targetAnimationName, float timeScale)	// 现在到当前动画结束这段时间内作blending，timeScale为1.0f
{
	float blendTime=mActiveAnimationLength-mActiveAnimationTime;
	BlendTo(targetAnimationName, blendTime, timeScale);
	return true;
}


bool GEAnimationController::TranslateTo(const string& targetAnimationName, float translateTime ,float timeScale)
{
	if(mBeingTranslating) return false;

	mActiveAnimationName = targetAnimationName;
	mTranslateInterval = translateTime;
	mTranslatingTime = 0.f;
	mTranslatedTimeScale = timeScale;
	mTranslatedAnimation = mAnimationSet->getAnimation(targetAnimationName);
	if(mTranslatedAnimation == NULL) return false;
	mTranslatedAnimationLength = mTranslatedAnimation->getLength();
	if(mTranslateInterval > mTranslatedAnimationLength)
		mTranslateInterval = mTranslatedAnimationLength;
	mBeingTranslating = true;

	return true;
}



void GEAnimationController::UpdateSkeleton()
{	
	vector<GEBone*> roots = mSkeleton->getRoot();
	for(unsigned int i=0;i<roots.size(); i++)
	{
		GEBone* root=roots[i];
		UpdateRelativeTransform(root);
	}
	mSkeleton->update();	// 更新absolute transform
}
 
void GEAnimationController::UpdateRelativeTransform(GEBone* pbone)
{	
	if(!mBeingBlending && !mBeingTranslating)
	{
		GEKeyframe kf=mActiveAnimation->getInterpolatedKeyframe(pbone->getIndex(),mActiveAnimationTime);
		pbone->setRelativeTransform(kf);
	}
	else if(mBeingBlending)
	{
		GEKeyframe kf1, kf2, kf3;
		kf1=mActiveAnimation->getInterpolatedKeyframe(pbone->getIndex(),mActiveAnimationTime);
		kf2=mBlendedAnimation->getInterpolatedKeyframe(pbone->getIndex(),mBlendedAnimationTime);
		kf3=kf1.interpolate(kf2, 1.0f-mBlendingTime/mBlendInterval);
		pbone->setRelativeTransform(kf3);
	}
	else if(mBeingTranslating)
	{
		GEKeyframe kf1, kf2, kf3;
		kf1=mActiveAnimation->getInterpolatedKeyframe(pbone->getIndex(),mActiveAnimationTime);
		kf2=mTranslatedAnimation->getInterpolatedKeyframe(pbone->getIndex(),mTranslatingTime);
		kf3=kf1.interpolate(kf2, mTranslatingTime/mTranslateInterval);
		pbone->setRelativeTransform(kf3);
	}

	
	// 更新子节点
	vector<unsigned short> child=pbone->getChildren();
	for(unsigned int i=0;i<child.size();i++)
	{
        UpdateRelativeTransform(child[i]);		
	}
}

void GEAnimationController::UpdateRelativeTransform(unsigned short boneIndex)
{
	GEBone* p=mSkeleton->getBone(boneIndex);
	UpdateRelativeTransform(p);
}

bool GEAnimationController::Update(float deltaTime)
{		
	bool endNext = AddTime(deltaTime);
	mSkeleton->setCurrentPose(this);
	mSkeleton->update();

	return endNext;
}

