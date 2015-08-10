#include "Animation/stdafx.h"
#include "Animation/Module.h"

#include "Animation/Skeleton.hxx"
#include "Animation/Animation.hxx"
#include "Animation/SkeletonPose.hxx"
#include "Animation/BoneMap.hxx"
#include "Animation/BoneSet.hxx"
#include "Animation/AnimationSet.hxx"
#include "Animation/AnimationComponent.hxx"
#include "Animation/AnimationDataCover.hxx"
#include "Animation/AnimationSystem.hxx"

// AnimationTree
#include "Animation/AnimationTree/AnimTreeNode.hxx"
#include "Animation/AnimationTree/AnimClipNode.hxx"
#include "Animation/AnimationTree/AnimBlendNode.hxx"
#include "Animation/AnimationTree/AnimLayerNode.hxx"


// PoseModifier
#include "Animation/PoseModifier/PoseModifier.hxx"
#include "Animation/PoseModifier/LookAtModifier.hxx"




using namespace ma;

#define RTTI_DECL(ClassType) Object* Create_##ClassType() { return new ClassType();}
#include <Animation/RTTIDecl.h>
#undef RTTI_DECL

Resource* AnimationData_Creator() {return new Animation();}
Resource* SkeletonData_Creator() {return new Skeleton();}
Resource* AnimSetData_Creator() {return new XmlFile();}

void AnimationModuleInit()
{
	AnimationSystem* pAniSystem = new AnimationSystem();
	SetAnimationSystem(pAniSystem);

	GetResourceSystem()->RegisterResourceFactory("ske",SkeletonData_Creator);
	GetResourceSystem()->RegisterResourceFactory("ska",AnimationData_Creator);
	GetResourceSystem()->RegisterResourceFactory("aniset",AnimSetData_Creator);

	/// RTTI
	#define RTTI_DECL(ClassType) \
		GetObjectFactoryManager()->RegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Animation/RTTIDecl.h>
	#undef RTTI_DECL

	AnimationComponent::RegisterAttribute();
	AnimTreeNode::RegisterAttribute();
	AnimClipNode::RegisterAttribute();
}

void AnimationModuleShutdown()
{
	// RTTI
	#define RTTI_DECL(ClassType) \
		GetObjectFactoryManager()->UnRegisterObjectFactory(#ClassType,Create_##ClassType);  
	#include <Animation/RTTIDecl.h>
	#undef RTTI_DECL

	GetResourceSystem()->UnregisterResourceFactory("ske",SkeletonData_Creator);
	GetResourceSystem()->UnregisterResourceFactory("ska",AnimationData_Creator);
	GetResourceSystem()->UnregisterResourceFactory("aniset",AnimSetData_Creator);

	AnimationSystem* pAniSystem = GetAnimationSystem();
	SAFE_DELETE(pAniSystem);
	SetAnimationSystem(NULL);
}


#if PLATFORM_WIN == 1
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

