#ifndef __IAnimationSystemRTTI_H__
#define __IAnimationSystemRTTI_H__

namespace ma
{
	// RTTI
	Object*	Create_AnimationObject(void* arg) {return GetAnimationSystem()->CreateAnimationObject( (GameObject*)arg );}
	void	Delete_AnimationObject(Object* pObj) {GetAnimationSystem()->DeleteAnimationObject((IAnimationObject*)pObj);}



	void IAnimationSystemRTTIInit()
	{

#define RTTI_DECL(ClassType,showType) \
	ClassType::StaticInitClass(); \
	ObjectFactoryManager::GetInstance().RegisterObjectFactory(#ClassType,Create_##showType);  \
	ObjectFactoryManager::GetInstance().RegisterObjectDeleteFactory(#ClassType,Delete_##showType);

		RTTI_DECL(IAnimationObject,AnimationObject);
#undef RTTI_DECL

	}

	void IAnimationSystemRTTIShoutdown()
	{

	}
}



#endif