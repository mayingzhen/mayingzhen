#include "GESkeleton.h"
#include "GEAnimationController.h"
#include "GEUtil.h"

GESkeleton::GESkeleton(void)
	: mCurrentMaxIndex(0)
{
}

GESkeleton::~GESkeleton(void)
{

}

bool GESkeleton::UnloadImpl(void)
{
	BoneIndexMap::iterator it=mBones.begin();
	while(it!=mBones.end())
	{
		delete it->second;
		++it;
	}
	mBones.clear();
	mBonesByName.clear();
	mRoots.clear();

	return true;
}


GEBone* GESkeleton::createBone(const string& name, int parent)
{
	// 找一个未被用过的index
	bool found=true;
	while(found)
	{
		BoneIndexMap::const_iterator it=mBones.find(++mCurrentMaxIndex);
		if(it==mBones.end()) found=false;
	}
    return this->createBone(mCurrentMaxIndex,name, parent);
}

GEBone* GESkeleton::createBone(unsigned short index, const string& name, int parent)
{
	GEBone* pbone=new GEBone(index, name, this, parent);	
	mBones.insert(BoneIndexPair(index,pbone));
	mBonesByName.insert(BoneNamePair(name, pbone));
	if(parent<0) mRoots.push_back(pbone);	// 如果是根节点
	return pbone;
}


void GESkeleton::setCurrentPose(GEAnimationController* controller)
{
	for(unsigned int i=0; i<mRoots.size(); i++)
	{
		controller->UpdateRelativeTransform(mRoots[i]);	//更新该骨头及其子节点
	}
}

void GESkeleton::computeInverseInitialTransform()
{
	// 将所有骨头的当前变换设为初始变换
	/*
	for(size_t i=0; i<mRoots.size(); ++i)
	{
		mRoots[i]->setToInitialTransform();
		//mRoots[i]->setToNoTransform();
	}
	*/

	// 计算初始变换的绝对变换
	updateWithInitialTransform();

	// 根据此时的绝对变换计算逆变换，得到绝对初始逆变换。
	for(size_t i=0; i<mRoots.size(); ++i)
	{
		mRoots[i]->computeInverseInitialTransform();
	}
}


GEBone* GESkeleton::getBone(unsigned short index)
{
	BoneIndexMap::iterator it=mBones.begin();
	it=mBones.find(index);
	if(it!=mBones.end())
		return it->second;
	else
	{
		//GEDEBUGINFO("Error in GESkeleton::getBone(unsigned short), Bone index not found.");
		return NULL;
	}
}

GEBone* GESkeleton::getBone(const string& name)
{
	BoneNameMap::iterator it=mBonesByName.begin();
	it=mBonesByName.find(name);
	if(it!=mBonesByName.end())
		return it->second;
	else
	{
		//GEDEBUGINFO("Error in GESkeleton::getBone(string), Bone name not found.");
		return NULL;
	}
}





