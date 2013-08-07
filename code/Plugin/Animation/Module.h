#ifndef  _ANIMATION_MODULE__H__
#define  _ANIMATION_MODULE__H__

#include "Animation/Export.h"
#include "Animation/Skeleton.h"
#include "Animation/SkeletonPose.h"
#include "Animation/Animation.h"
#include "Animation/AnimationClip.h"
#include "Animation/BoneMap.h"
#include "Animation/BoneSet.h"
#include "Animation/AnimEvalContext.h"
#include "Animation/AnimationSet.h"
#include "Animation/Action.h"
#include "Animation/AnimationPlay.h"
#include "Animation/AnimationDataCover.h"
#include "Animation/AnimationUtil.h"


// AnimationTree
#include "Animation/AnimationTree/AnimTreeNode.h"
#include "Animation/AnimationTree/AnimClipNode.h"
#include "Animation/AnimationTree/AnimBlendNode.h"
#include "Animation/AnimationTree/AnimLayerNode.h"


// PoseModifier
#include "Animation/PoseModifier/PoseModifier.h"
#include "Animation/PoseModifier/LookAtModifier.h"


// Device
#include "Animation/AnimationSystem.h"



ANIMATION_API void AnimationModuleInit();

ANIMATION_API void AnimationModuleShutdown();


#endif
