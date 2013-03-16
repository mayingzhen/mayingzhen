#include "Animation/stdafx.h"
#include "Animation/Module.h"

#include "Animation/Skeleton.hxx"
#include "Animation/Animation.hxx"
#include "Animation/AnimationInst.hxx"
#include "Animation/SkeletonPose.hxx"
#include "Animation/BoneMap.hxx"
#include "Animation/BoneSet.hxx"
#include "Animation/AnimationSet.hxx"
#include "Animation/Action.hxx"
#include "Animation/AnimationPlay.hxx"
#include "Animation/AnimationDataCover.hxx"
#include "Animation/AnimationUtil.hxx"


// AnimationTree
#include "Animation/AnimationTree/AnimTreeNode.hxx"
#include "Animation/AnimationTree/AnimClipNode.hxx"
#include "Animation/AnimationTree/AnimBlendNode.hxx"
#include "Animation/AnimationTree/AnimLayerNode.hxx"


// PoseModifier
#include "Animation/PoseModifier/PoseModifier.hxx"
#include "Animation/PoseModifier/LookAtModifier.hxx"


// Device
#include "Animation/AnimationDevice.hxx"


using namespace ma;

#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();}
#include <Animation/RTTIDecl.h>
#undef RTTI_DECL


void AnimationModuleInit()
{
	/// RTTI
	#define RTTI_DECL(ClassType) ClassType##::StaticInitClass();
	#include <Animation/RTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Animation/RTTIDecl.h>
	#undef RTTI_DECL


	// Device
	AnimationDevice* pAnimationDevice = new AnimationDevice();
	SetAnimationDevice(pAnimationDevice); 
}

void AnimationModuleShutdown()
{
	// Device
	AnimationDevice* pAnimationDevice = (AnimationDevice*)GetAnimationDevice();
	SAFE_DELETE(pAnimationDevice);


	// RTTI
	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().UnRegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Animation/RTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ClassType##::StaticShutdownClass();
	#include <Animation/RTTIDecl.h>
	#undef RTTI_DECL
}
