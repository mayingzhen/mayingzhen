#include "Animation/BoneMap.h"

namespace ma
{
	void BoneMap::Build(const Skeleton* pSkeleton,const Animation* pAnimation)
	{
		if (pSkeleton == NULL || pAnimation == NULL)
			return;

		UINT uBoneNumber = pSkeleton->GetBoneNumer();
		m_arrMapBoneInd.resize(uBoneNumber,Math::InvalidID<BoneIndex>());
		for (UINT i = 0; i < uBoneNumber; ++i)
		{
			const char* pszBoneName = pSkeleton->GetBoneNameByIndex(i); 
			UINT uTransfTrackIndex = pAnimation->GetTrackIndexByName(pszBoneName);
			m_arrMapBoneInd[i] = uTransfTrackIndex;
		}
	}

	BoneIndex BoneMap::MapNode(BoneIndex uBoneInd) const
	{
		if (uBoneInd >= m_arrMapBoneInd.size() || uBoneInd < 0)
			return Math::InvalidID<BoneIndex>();

		return m_arrMapBoneInd[uBoneInd];
	}
}