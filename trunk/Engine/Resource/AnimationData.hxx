#include "AnimationData.h"

namespace ma
{

	void AnimationHeader::Serialize(SerializeListener& sl,const char* pszLable)
	{
		sl.BeginSection(pszLable);
		sl.Serialize(m_nIden,"Iden");
		sl.Serialize(m_nVersion,"Version");
		sl.EndSection();
	}

	void Vector3TrackData::Serialize(SerializeListener& sl,const char* pszLable)
	{
		sl.BeginSection(pszLable);
		sl.Serialize(m_arrFrame,"Frame");
		sl.Serialize(m_arrKey,"Data");
		sl.EndSection();
	}

	void QuaternionTrackData::Serialize(SerializeListener& sl,const char* pszLable)
	{
		sl.BeginSection(pszLable);
		sl.Serialize(m_arrFrame,"Frame");
		sl.Serialize(m_arrKey,"Data");
		sl.EndSection();
	}

	void FloatTrackData::Serialize(SerializeListener& sl,const char* pszLable)
	{
		sl.BeginSection(pszLable);
		sl.Serialize(m_arrKey,"Data");
		sl.EndSection();
	}

	void AnimationData::Serialize(SerializeListener& sl,const char* pszLable)
	{
		AnimationHeader header;
		sl.Serialize(header,"AnimationHeader");
		sl.PushVersion(header.m_nVersion);
		sl.BeginSection(pszLable);
		sl.Serialize(m_nGlobalSkeletonID,"GlobalSkeletonID");
		sl.Serialize(m_nFrameNum,"FrameNumber");
		sl.Serialize(m_nBoneNum,"BoneNum");
		sl.Serialize(m_arrScaleTrack,"ScalePSTrack");
		sl.Serialize(m_arrRotTrack,"RotationPSTrack");
		sl.Serialize(m_arrPosTrack,"PositionPSTrack");
		sl.Serialize(m_arrTransfTrackName,"TransfTrackName");
		sl.PopVersion();
		sl.EndSection();
	}
}
