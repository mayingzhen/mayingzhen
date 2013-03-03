#include "AnimationData.h"

namespace S3
{

	void AnimationHeader::Serialize(SerializeListener& sl,const char* pszLable)
	{
		sl.BeginSection(pszLable);
		sl.Serialize(m_nIden,"Iden");
		sl.Serialize(m_nVersion,"Version");
		sl.Serialize(m_strMaxFile,"MaxFile");
		sl.EndSection();
	}

	void Vector3TrackData::Serialize(SerializeListener& sl,const char* pszLable)
	{
		sl.BeginSection(pszLable);
		if (sl.GetVersion() >= EXP_ANIM_VER_ROOT_MOTION)
		{
			sl.Serialize(m_arrFrame,"Frame");
		}
		sl.Serialize(m_arrKey,"Data");
		sl.EndSection();
	}

	void QuaternionTrackData::Serialize(SerializeListener& sl,const char* pszLable)
	{
		sl.BeginSection(pszLable);
		if (sl.GetVersion() >= EXP_ANIM_VER_ROOT_MOTION)
		{
			sl.Serialize(m_arrFrame,"Frame");
		}
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

		if (header.m_nVersion <= EXP_ANIM_VER_ROOT_MOTION)
		{
			sl.Serialize(m_nGlobalSkeletonID.m_a,"GlobalSkeletonID");
			m_nGlobalSkeletonID.m_b = 0;

			sl.Serialize(m_nFrameNum,"FrameNumber");
			sl.Serialize(m_arrScaleTrack,"ScalePSTrack");
			sl.Serialize(m_arrRotTrack,"RotationPSTrack");
			sl.Serialize(m_arrPosTrack,"PositionPSTrack");
			if(sl.GetVersion() == EXP_ANIM_VER_ROOT_MOTION)
			{
				sl.Serialize(m_rootMotionScale,"RootMotionScale");
				sl.Serialize(m_rootMotionRot,"RootMotionRot");
				sl.Serialize(m_rootMotionPos,"RootMotionPos");
			}
		} 
		else if(header.m_nVersion >= EXP_ANIM_VER_ANIM_RETARGET)
		{


			sl.Serialize(m_nGlobalSkeletonID,"GlobalSkeletonID");
			sl.Serialize(m_nFrameNum,"FrameNumber");
			sl.Serialize(m_nBoneNum,"BoneNum");
			sl.Serialize(m_nSocketNum,"SocketNum");

			if (sl.GetVersion() == EXP_ANIM_VER_ANIM_RETARGET)
			{
				std::vector<xmUint16> arrParentIndice;
				sl.Serialize(arrParentIndice,"ParentIndice");
			}


			if (sl.GetVersion() >= EXP_ANIM_VER_ANIM_REDUCE_KEYFRAME)
			{
				sl.Serialize(m_bCompressed,"Compressed");
				sl.Serialize(m_fCompDiffThreshold,"DifferenceThreshold");
				sl.Serialize(m_fCompVarianceThreshold,"VarianceThreshold");
			}

			sl.Serialize(m_arrScaleTrack,"ScalePSTrack");
			sl.Serialize(m_arrRotTrack,"RotationPSTrack");
			sl.Serialize(m_arrPosTrack,"PositionPSTrack");
			sl.Serialize(m_arrTransfTrackName,"TransfTrackName");
			sl.Serialize(m_rootMotionScale,"RootMotionScale");
			sl.Serialize(m_rootMotionRot,"RootMotionRot");
			sl.Serialize(m_rootMotionPos,"RootMotionPos");
			sl.Serialize(m_arrFloatLSTrack,"FloatPSTrack");
			sl.Serialize(m_arrFloatTrackName,"FloatTrackName");

		}

		sl.PopVersion();

		sl.EndSection();
	}


}