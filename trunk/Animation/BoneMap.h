#ifndef  _Bone_Map__H__
#define  _Bone_Map__H__

namespace ma
{
	class Skeleton;
	class Animation;
	
	class ANIMATION_API BoneMap
	{
	public:
		void Build(Skeleton* pSkeleton,Animation* pAnimation); 

		BoneIndex MapNode(BoneIndex uBoneInd);

	private:
		std::vector<BoneIndex> m_arrMapBoneInd;
	};
}


#endif