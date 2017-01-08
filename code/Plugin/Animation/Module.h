#ifndef  _ANIMATION_MODULE__H__
#define  _ANIMATION_MODULE__H__

#include "Animation/Skeleton.h"
#include "Animation/SkeletonPose.h"
#include "Animation/Animation.h"
#include "Animation/AnimationDataCover.h"
#include "Animation/AnimationSet.h"
#include "Animation/AnimationTree/AnimTreeNode.h"
#include "Animation/AnimationTree/AnimClipNode.h"
#include "Animation/AnimationTree/AnimBlendNode.h"
#include "Animation/AnimationTree/FrameEvent.h"
#include "Animation/BoneSet.h"
#include "Animation/AnimationComponent.h"
#include "Animation/PoseModifier/LookAtModifier.h"



void AnimationModuleInit();

void AnimationModuleShutdown();


#endif
