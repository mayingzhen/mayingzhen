
#include "GESkeletonManager.h"


GESkeletonManager GESkeletonManager::g_SkeletonManager;


GESkeleton* GESkeletonManager::CreateSkeleton(const string& strName, const string& strOrigin)
{
	return NULL;//(GESkeleton*)(g_SkeletonManager.CreateResource(strName,strOrigin,new GESkeleton));
}

GESkeleton* GESkeletonManager::CreateSkeleton(const string& strOrigin)
{
	return NULL;//(GESkeleton*)(g_SkeletonManager.CreateResource(strOrigin,new GESkeleton));
}


