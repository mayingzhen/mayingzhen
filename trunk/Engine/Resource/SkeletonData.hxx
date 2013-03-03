#include "SkeletonData.h"

namespace ma
{
	void SkeletonHeader::Serialize(SerializeListener& sl,const char* pszLable)
	{
		sl.BeginSection(pszLable);
		sl.Serialize(m_nIden,"Iden");
		sl.Serialize(m_nVersion,"Version");
		sl.EndSection();
	}

	void SkeletonData::Serialize(SerializeListener& sl,const char* pszLable)
	{
		SkeletonHeader skelHeader;
		sl.Serialize(skelHeader,"Header");

		sl.BeginSection(pszLable);

		sl.Serialize(m_nGlobalSkeletonID,"GlobalSkeletonID");
		sl.Serialize(m_nBoneNum,"BoneNumber");
 		sl.Serialize(m_arrBoneName,"BoneName");
 		sl.Serialize(m_arrParentIndice,"ParentIndice");
 		sl.Serialize(m_arrScaleOS,"ScaleOS");
 		sl.Serialize(m_arrRotOS,"RotationOS");
 		sl.Serialize(m_arrPosOS,"PositionOS");

		sl.EndSection();
	}
}
