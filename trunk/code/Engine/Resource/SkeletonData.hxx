#include "SkeletonData.h"

namespace ma
{
	enum SkelVersion
	{
		EXP_SKEL_VER_INITIAL = 1,
		EXP_SKEL_VER_CURRENT = EXP_SKEL_VER_INITIAL,
	};

	SkeletonHeader::SkeletonHeader()
	{
		m_nIden = 'MAED';
		m_nVersion = EXP_SKEL_VER_CURRENT;
	}


	void SerializeData(SerializeListener& sl, SkeletonHeader& data, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(data.m_nIden,"Iden");
		sl.Serialize(data.m_nVersion,"Version");
		
		sl.EndSection();
	}

	void SerializeDataV1(SerializeListener& sl, SkeletonData& data, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(data.m_nGlobalSkeletonID,"GlobalSkeletonID");
		sl.Serialize(data.m_nBoneNum,"BoneNumber");
 		sl.Serialize(data.m_arrBoneName,"BoneName");
 		sl.Serialize(data.m_arrParentIndice,"ParentIndice");
 		sl.Serialize(data.m_arrScaleOS,"ScaleOS");
 		sl.Serialize(data.m_arrRotOS,"RotationOS");
 		sl.Serialize(data.m_arrPosOS,"PositionOS");

		sl.EndSection();
	}

	void SkeletonData::Serialize(SerializeListener& sl, const char* pszLable)
	{
		SkeletonHeader header;
		sl.Serialize(header);
		if (header.m_nIden != 'MAED')
			return;

		if (header.m_nVersion == EXP_SKEL_VER_CURRENT)
		{
			SerializeDataV1(sl,*this,pszLable);
		}

	}
}
