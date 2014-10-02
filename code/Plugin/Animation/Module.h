#ifndef  _ANIMATION_MODULE__H__
#define  _ANIMATION_MODULE__H__

#include "Animation/Export.h"

#include "Animation/Skeleton.h"
#include "Animation/SkeletonPose.h"
#include "Animation/Animation.h"
#include "Animation/AnimationDataCover.h"
#include "Animation/AnimationSetData.h"
#include "Animation/AnimationSet.h"
#include "Animation/SkelAnimtionData.h"
#include "Animation/AnimationTree/AnimTreeNode.h"
#include "Animation/AnimationTree/AnimClipNode.h"
#include "Animation/AnimationTree/AnimLayerNode.h"
#include "Animation/AnimationTree/AnimBlendNode.h"
#include "Animation/SkelAnimtion.h"
#include "Animation/AnimationComponent.h"
#include "Animation/AnimationSystem.h"



ANIMATION_API void AnimationModuleInit();

ANIMATION_API void AnimationModuleShutdown();


#endif
