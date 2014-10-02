#include "Animation/stdafx.h"
#include "Animation/Module.h"

#include "Animation/Skeleton.hxx"
#include "Animation/Animation.hxx"
#include "Animation/AnimationClip.hxx"
#include "Animation/SkeletonPose.hxx"
#include "Animation/BoneMap.hxx"
#include "Animation/BoneSet.hxx"
#include "Animation/AnimationSetData.hxx"
#include "Animation/AnimationSet.hxx"
#include "Animation/SkelAnimtionData.hxx"
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
Resource* AnimationSetData_Creator() {return new AnimationSetData();}

void AnimationModuleInit()
{
	AnimationComponent::StaticInitClass(); 
	ObjectFactoryManager::GetInstance().RegisterObjectFactory("AnimationComponent",Create_AnimationComponent); 
	ObjectFactoryManager::GetInstance().RegisterObjectDeleteFactory("AnimationComponent",Delete_AnimationComponent);


	/// RTTI
	#define RTTI_DECL(ClassType) ClassType::StaticInitClass();
	#include <Animation/RTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Animation/RTTIDecl.h>
	#undef RTTI_DECL

	ResourceSystem::RegisterResourceFactory("ska",AnimationData_Creator);
	ResourceSystem::RegisterResourceFactory("ske",SkeletonData_Creator);
	ResourceSystem::RegisterResourceFactory("aniset",AnimationSetData_Creator);

	AnimationSystem* pAniSystem = new AnimationSystem();
	SetAnimationSystem(pAniSystem);
}

void AnimationModuleShutdown()
{
	AnimationSystem* pAniSystem = GetAnimationSystem();
	SAFE_DELETE(pAniSystem);
	SetAnimationSystem(NULL);

	ResourceSystem::UnregisterResourceFactory("aniset",AnimationSetData_Creator);
	ResourceSystem::UnregisterResourceFactory("skn",AnimationData_Creator);
	ResourceSystem::UnregisterResourceFactory("ske",SkeletonData_Creator);

	// RTTI
	#define RTTI_DECL(ClassType) ObjectFactoryManager::GetInstance().UnRegisterObjectFactory(#ClassType,Create_##ClassType);
	#include <Animation/RTTIDecl.h>
	#undef RTTI_DECL

	#define RTTI_DECL(ClassType) ClassType::StaticShutdownClass();
	#include <Animation/RTTIDecl.h>
	#undef RTTI_DECL
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

