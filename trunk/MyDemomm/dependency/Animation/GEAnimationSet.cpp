#include ".\geanimationset.h"

GEAnimationSet::GEAnimationSet(void)
{
}

GEAnimationSet::~GEAnimationSet(void)
{
}

bool GEAnimationSet::UnloadImpl(void)
{
	mSetByIndex.clear();
	mSetByName.clear();
	return true;
}

void GEAnimationSet::addAnimation(GEAnimation* animation)
{
	int index;
	string name;
	
	index=animation->getIndex();
	name=animation->getName();

	mSetByIndex.insert(AniIndexMapPair(index, animation));
	mSetByName.insert(AniNameMapPair(name, animation));
}

GEAnimation* GEAnimationSet::getAnimation(string name)
{
	AniNameMap::const_iterator it;
	it=mSetByName.find(name);
	if(it!=mSetByName.end())
		return it->second;
	else
		return	NULL;
}

GEAnimation* GEAnimationSet::getAnimation(int index)
{
	AniIndexMap::const_iterator it;
	it=mSetByIndex.find(index);
	if(it!=mSetByIndex.end())
		return it->second;
	else
		return NULL;
}

unsigned int GEAnimationSet::getNumberAnimation()
{
	return static_cast<unsigned int>(mSetByIndex.size());
}

bool GEAnimationSet::removeAnimation(string name)
{
	AniNameMap::iterator it;
	it=mSetByName.find(name);

	if(it==mSetByName.end()) return false;

	AniIndexMap::iterator it2;
	it2=mSetByIndex.find(it->second->getIndex());

	mSetByName.erase(it);
	mSetByIndex.erase(it2);
	return true;
}

bool GEAnimationSet::removeAnimation(int index)
{
	AniIndexMap::iterator it;
	it=mSetByIndex.find(index);

	if(it==mSetByIndex.end()) return false;

	AniNameMap::iterator it2;
	it2=mSetByName.find(it->second->getName());

	mSetByIndex.erase(it);
	mSetByName.erase(it2);	
	return true;
}





