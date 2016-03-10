#ifndef  _Bone_Map__H__
#define  _Bone_Map__H__

namespace ma
{
	class Skeleton;
	class Animation;
	
	class BoneMap
	{
	public:
		void Build(const Skeleton* pSkeleton,const Animation* pAnimation); 

		BoneIndex MapNode(BoneIndex uBoneInd) const;

	private:
		std::vector<BoneIndex> m_arrMapBoneInd;
	};
}


#endif