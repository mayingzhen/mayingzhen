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
	// ��һ��δ���ù���index
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
	if(parent<0) mRoots.push_back(pbone);	// ����Ǹ��ڵ�
	return pbone;
}


void GESkeleton::setCurrentPose(GEAnimationController* controller)
{
	for(unsigned int i=0; i<mRoots.size(); i++)
	{
		controller->UpdateRelativeTransform(mRoots[i]);	//���¸ù�ͷ�����ӽڵ�
	}
}

void GESkeleton::computeInverseInitialTransform()
{
	// �����й�ͷ�ĵ�ǰ�任��Ϊ��ʼ�任
	/*
	for(size_t i=0; i<mRoots.size(); ++i)
	{
		mRoots[i]->setToInitialTransform();
		//mRoots[i]->setToNoTransform();
	}
	*/

	// �����ʼ�任�ľ��Ա任
	updateWithInitialTransform();

	// ���ݴ�ʱ�ľ��Ա任������任���õ����Գ�ʼ��任��
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





