#include "Animation/stdafx.h"
#include "Animation/Module.h"

#include "Animation/Skeleton.hxx"
#include "Animation/Animation.hxx"
#include "Animation/AnimationClip.hxx"
#include "Animation/SkeletonPose.hxx"
#include "Animation/BoneMap.hxx"
#include "Animation/BoneSet.hxx"
#include "Animation/AnimationSet.hxx"
#include "Animation/SkelAnimtion.hxx"
#include "Animation/AnimationComponent.hxx"
#include "Animation/AnimationDataCover.hxx"
#include "Animation/AnimationUtil.hxx"
#include "Animation/AnimationSystem.hxx"

// AnimationTree
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


Object* Create_AnimationComponent(void* arg) { return GetAnimationSystem()->CreateAnimationObject((SceneNode*)arg);} 
void Delete_AnimationComponent(Object* pObj) {}


Resource* AnimationData_Creator() {return new Animation();}
Resource* SkeletonData_Creator() {return new Skeleton();}

void AnimationModuleInit()
{
	GetResourceSystem()->RegisterResourceFactory("ske",SkeletonData_Creator);
	GetResourceSystem()->RegisterResourceFactory("ska",AnimationData_Creator);

	/// RTTI
	#define RTTI_DECL(ClassType) \
		ClassType::StaticInitClass(); \
		GetObjectFactoryManager()->RegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Animation/RTTIDecl.h>
	#undef RTTI_DECL

	AnimationComponent::StaticInitClass();
	GetObjectFactoryManager()->RegisterObjectFactory("AnimationComponent",Create_AnimationComponent);

	AnimationComponent::RegisterObject( GetContext() );

	AnimationSystem* pAniSystem = new AnimationSystem();
	SetAnimationSystem(pAniSystem);
}

void AnimationModuleShutdown()
{
	AnimationSystem* pAniSystem = GetAnimationSystem();
	SAFE_DELETE(pAniSystem);
	SetAnimationSystem(NULL);

	// RTTI
	#define RTTI_DECL(ClassType) \
		GetObjectFactoryManager()->UnRegisterObjectFactory(#ClassType,Create_##ClassType); 
	#include <Animation/RTTIDecl.h>
	#undef RTTI_DECL

	AnimationComponent::StaticShutdownClass(); 
	GetObjectFactoryManager()->UnRegisterObjectFactory("AnimationComponent",Create_AnimationComponent); 

	GetResourceSystem()->UnregisterResourceFactory("ske",SkeletonData_Creator);
	GetResourceSystem()->UnregisterResourceFactory("ska",AnimationData_Creator);
}

#if PLATFORM_WIN == 1
extern "C" ANIMATION_API bool dllStartPlugin()
{
	AnimationModuleInit();

	return true;
}


extern "C" ANIMATION_API bool dllStopPlugin()
{
	AnimationModuleShutdown();

	return true;
}
#endif

