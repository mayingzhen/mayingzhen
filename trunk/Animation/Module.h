#ifndef  _ANIMATION_MODULE__H__
#define  _ANIMATION_MODULE__H__

#include "Animation/Export.h"
#include "Animation/Skeleton.h"
#include "Animation/NodePose.h"
#include "Animation/Animation.h"
#include "Animation/AnimationInst.h"
#include "Animation/NodeLink.h"
#include "Animation/AnimEvalContext.h"
#include "Animation/AnimationSet.h"
#include "Animation/AnimationAction.h"
#include "Animation/AnimationPlay.h"

#include "Animation/AnimationTree/AnimTreeNode.h"
#include "Animation/AnimationTree/AnimClipNode.h"
#include "Animation/AnimationTree/AnimBlendNode.h"
#include "Animation/AnimationTree/AnimLayerNode.h"

#include "Animation/PoseModifier/PoseModifier.h"
#include "Animation/PoseModifier/LookAtModifier.h"

#include "Animation/AnimationDataCover.h"

ANIMATION_API void AnimationModuleInit();

ANIMATION_API void AnimationModuleShutdown();


#endif