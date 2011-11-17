
#include "GEAnimationSetManager.h"


GEAnimationSetManager GEAnimationSetManager::g_AnimationSetManager;


GEAnimationSet* GEAnimationSetManager::CreateAnimationSet(const string& strName, const string& strOrigin)
{
	return NULL;//(GEAnimationSet*)(g_AnimationSetManager.CreateResource(strName,strOrigin,new GEAnimationSet));
}

GEAnimationSet* GEAnimationSetManager::CreateAnimationSet(const string& strOrigin)
{
	return NULL;//(GEAnimationSet*)(g_AnimationSetManager.CreateResource(strOrigin,new GEAnimationSet));
}


