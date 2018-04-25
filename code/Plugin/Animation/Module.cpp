#include "Animation/stdafx.h"
#include "Animation/Module.h"

#include "Animation/Skeleton.hxx"
#include "Animation/Track.hxx"
#include "Animation/Animation.hxx"
#include "Animation/SkeletonPose.hxx"
#include "Animation/BoneMap.hxx"
#include "Animation/BoneSet.hxx"
#include "Animation/AnimationSet.hxx"
#include "Animation/AnimationComponent.hxx"
#include "Animation/AnimationDataCover.hxx"
#include "Animation/AnimDataManager.hxx"

// AnimationTree
#include "Animation/AnimationTree/AnimTreeNode.hxx"
#include "Animation/AnimationTree/AnimClipNode.hxx"
#include "Animation/AnimationTree/AnimBlendNode.hxx"
#include "Animation/AnimationTree/FrameEvent.hxx"


// PoseModifier
#include "Animation/PoseModifier/PoseModifier.hxx"
#include "Animation/PoseModifier/LookAtModifier.hxx"


// AnimationNodes
#include "Animation/AnimationTree/ParameterHandler.hxx"
#include "Animation/AnimationTree/ParameterNode.hxx"
#include "Animation/AnimationTree/StateMachineNode.hxx"



using namespace ma;


void AnimationModuleInit()
{
	g_pSkeletonManager = new ResourceSystem<Skeleton>();
	g_pAnimDataManager = new AnimDataManager();

	AnimationComponent::RegisterAttribute();
	AnimTreeNode::RegisterAttribute();
	AnimClipNode::RegisterAttribute();
	StateMachineNode::RegisterAttribute();
	MachineTransition::RegisterAttribute();
}

void AnimationModuleShutdown()
{
	SAFE_DELETE(g_pSkeletonManager);
	SAFE_DELETE(g_pAnimDataManager);
}


#if defined(_WIN32)
extern "C" bool dllStartPlugin()
{
	AnimationModuleInit();

	return true;
}


extern "C" bool dllStopPlugin()
{
	AnimationModuleShutdown();

	return true;
}
#endif

