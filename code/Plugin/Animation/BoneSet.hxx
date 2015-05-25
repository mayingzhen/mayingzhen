#include "BoneSet.h"

namespace ma
{
	void BoneSet::RemoveBoneInd(BoneIndex uBoneID) 
	{
		for (UINT i = 0; i<m_arrBone.size(); ++i)
		{
			if (m_arrBone[i] == uBoneID)
			{
				m_arrBone.erase(m_arrBone.begin() + i);
				return;
			}
		}
	}

	void BoneSet::Serialize(Serializer& sl, const char* pszLable)
	{

	}

}