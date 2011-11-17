
#ifndef GESKELETONMANAGER_H
#define GESKELETONMANAGER_H


//#include "GEResourceManager.h"
#include "GESkeleton.h"

class GESkeletonManager /*: public GEResourceManager*/
{
public:
	~GESkeletonManager(void){};

	static GESkeletonManager& GetSingleton()
	{ return g_SkeletonManager; }
	
	GESkeleton* CreateSkeleton(const string& strName, const string& strOrigin);
	GESkeleton* CreateSkeleton(const string& strOrigin);

	/** 得到一个Mesh */
	//inline GESkeleton* GetSkeleton(ResHandle h)
	//{ return ((GESkeleton *)g_SkeletonManager.GetResource(h)); }

	//inline GESkeleton* GetSkeleton(const string& name)
	//{ return ((GESkeleton *)g_SkeletonManager.GetResource(name)); }

	
private:
	GESkeletonManager() {}

	static GESkeletonManager g_SkeletonManager;

};


#endif