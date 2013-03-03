#include "SkeletonData.h"

namespace S3
{



void SkeletonHeader::Serialize(SerializeListener& sl,const char* pszLable)
{
	sl.BeginSection(pszLable);
	sl.Serialize(m_nIden,"Iden");
	sl.Serialize(m_nVersion,"Version");
	sl.Serialize(m_strMaxFile,"MaxFile");
	sl.EndSection();
}


void SocketData::Serialize(SerializeListener& sl,const char* pszLable)
{
	sl.BeginSection(pszLable);
	sl.Serialize(m_strBoneName,"BoneName");
	sl.Serialize(m_nBoneID,"BoneID");
	sl.Serialize(m_matTransformBS,"TransformBS");
	sl.EndSection();
}



void SkeletonData::Serialize(SerializeListener& sl,const char* pszLable)
{
	SkeletonHeader skelHeader;
	sl.Serialize(skelHeader,"Header");

	sl.BeginSection(pszLable);

	if (skelHeader.m_nVersion == EXP_SKEL_VER_INITIAL)
	{
		sl.Serialize(m_nGlobalSkeletonID.m_a,pszLable);
		m_nGlobalSkeletonID.m_b = 0;

	}
	else
	{
		sl.Serialize(m_nGlobalSkeletonID,"GlobalSkeletonID");
	}

	sl.Serialize(m_nBoneNum,"BoneNumber");
 	sl.Serialize(m_arrBoneName,"BoneName");
 	sl.Serialize(m_arrParentIndice,"ParentIndice");
 	sl.Serialize(m_arrScaleOS,"ScaleOS");
 	sl.Serialize(m_arrRotOS,"RotationOS");
 	sl.Serialize(m_arrPosOS,"PositionOS");
 	sl.Serialize(m_arrSocket,"Socket");

	if (skelHeader.m_nVersion >= EXP_SKEL_VER_BONE_LOD_MASK)
	{
		sl.Serialize(m_arrLODBoneMask, "LODBoneMask");
	}
	else
	{
		if (m_arrLODBoneMask.empty())
		{
			std::string strBoneMask;
			strBoneMask.resize(m_nBoneNum + m_arrSocket.size(), '1');
			m_arrLODBoneMask.push_back(strBoneMask);
		}
	}

	sl.EndSection();
}
}