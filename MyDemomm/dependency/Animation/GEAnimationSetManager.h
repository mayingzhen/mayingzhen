
#ifndef GEANIMATIONSETMANAGER_H
#define GEANIMATIONSETMANAGER_H


//#include "GEResourceManager.h"
#include "GEAnimationSet.h"

class GEAnimationSetManager /*: public GEResourceManager*/
{
public:
	~GEAnimationSetManager(void){};

	static GEAnimationSetManager& GetSingleton()
	{ return g_AnimationSetManager; }
	
	GEAnimationSet* CreateAnimationSet(const string& strName, const string& strOrigin);
	GEAnimationSet* CreateAnimationSet(const string& strOrigin);

	/** 得到一个AnimationSet */
	//inline GEAnimationSet* GetAnimationSet(ResHandle h)
	//{ return ((GEAnimationSet *)g_AnimationSetManager.GetResource(h)); }

	//inline GEAnimationSet* GetAnimationSet(const string& name)
	//{ return ((GEAnimationSet *)g_AnimationSetManager.GetResource(name)); }

	
private:
	GEAnimationSetManager() {}

	static GEAnimationSetManager g_AnimationSetManager;

};


#endif