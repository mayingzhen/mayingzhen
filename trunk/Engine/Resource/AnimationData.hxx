#include "AnimationData.h"

namespace ma
{
	enum AnimVersion
	{
		EXP_ANIM_VER_INITIAL = 1,

		EXP_ANIM_VER_CURRENT = EXP_ANIM_VER_INITIAL,
	};

	AnimationHeader::AnimationHeader()
	{
		m_nIden = 'MAAD';
		m_nVersion = EXP_ANIM_VER_CURRENT;
	}

	void SerializeData(SerializeListener& sl,AnimationHeader& aniHeader, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(aniHeader.m_nIden,"Iden");
		sl.Serialize(aniHeader.m_nVersion,"Version");

		sl.EndSection();
	}

	void SerializeData(SerializeListener& sl, Vector3TrackData& data, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(data.m_arrFrame,"Frame");
		sl.Serialize(data.m_arrKey,"Data");

		sl.EndSection();
	}

	void SerializeData(SerializeListener& sl, QuaternionTrackData& data, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(data.m_arrFrame,"Frame");
		sl.Serialize(data.m_arrKey,"Data");

		sl.EndSection();
	}


	void SerializeDataV1(SerializeListener& sl, AnimationData& data, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(data.m_nGlobalSkeletonID,"GlobalSkeletonID");
		sl.Serialize(data.m_nFrameNum,"FrameNumber");
		sl.Serialize(data.m_nBoneNum,"BoneNum");
		sl.Serialize(data.m_arrScaleTrack,"ScalePSTrack");
		sl.Serialize(data.m_arrRotTrack,"RotationPSTrack");
		sl.Serialize(data.m_arrPosTrack,"PositionPSTrack");
		sl.Serialize(data.m_arrTransfTrackName,"TransfTrackName");

		sl.EndSection();
	}

	void AnimationData::Serialize(SerializeListener& sl, const char* pszLable)
	{
		AnimationHeader header;
		sl.Serialize(header);
		if (header.m_nIden != 'MAAD')
			return;

		if (header.m_nVersion == EXP_ANIM_VER_INITIAL)
		{
			SerializeDataV1(sl,*this,pszLable);
		}
	}
}
