#ifndef  _NODE_LINK__H__
#define  _NODE_LINK__H__

namespace ma
{
	class Skeleton;
	class Animation;
	
	class ANIMATION_API NodeLink
	{
	public:
		void Build(Skeleton* pSkeleton,Animation* pAnimation); 

		BoneIndex MapNode(BoneIndex uBoneInd);

	private:
		std::vector<BoneIndex> m_arrMapBoneInd;
	};
}


#endif