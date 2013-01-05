#ifndef __AnimationData_H__
#define __AnimationData_H__

namespace ma
{




enum AnimVersion
{
	EXP_ANIM_VER_INITIAL = 0,
	EXP_ANIM_VER_ROOT_MOTION, //Add root motion
	EXP_ANIM_VER_ANIM_RETARGET,	//Use case insensitive bone name,bone hierarchy for skelGUID, embed track name, float track and name track
	EXP_ANIM_VER_ANIM_REDUCE_KEYFRAME, //1.add compression param.
	//2.use local space animation tracks 
	//3.remove parent indice from animation data - it should get from binding skeleton
	//4.fix first frame no key problem
	EXP_ANIM_VER_CURRENT = EXP_ANIM_VER_ANIM_REDUCE_KEYFRAME,
};

struct COMMON_API AnimationHeader
{
	xmUint m_nIden;
	xmUint m_nVersion;
	std::string m_strMaxFile;

	void Serialize(SerializeListener& sl,const char* pszLable = "AnimationHeader");
};

struct COMMON_API Vector3TrackData
{
	std::vector<xmUint>		 m_arrFrame;
	std::vector<D3DXVECTOR3> m_arrKey;

	void Serialize(SerializeListener& sl,const char* pszLable = "Vector3Track");
};

struct COMMON_API QuaternionTrackData
{
	std::vector<xmUint>			m_arrFrame;
	std::vector<D3DXQUATERNION> m_arrKey;

	void Serialize(SerializeListener& sl,const char* pszLable = "QuaternionTrack");
};

struct COMMON_API FloatTrackData
{
	std::vector<xmUint>			m_arrFrame;
	std::vector<float>			m_arrKey;

	void Serialize(SerializeListener& sl,const char* pszLable = "FloatTrack" );
};

struct COMMON_API AnimationData 
{
	GUID								m_nGlobalSkeletonID;
	xmUint								m_nFrameNum;
	xmUint								m_nBoneNum;						//Add lhh 2010-11-15
	xmUint								m_nSocketNum;					//Add lhh 2010-11-15

	bool								m_bCompressed;
	float								m_fCompDiffThreshold;
	float								m_fCompVarianceThreshold;


	std::vector<Vector3TrackData>		m_arrScaleTrack;
	std::vector<QuaternionTrackData>	m_arrRotTrack;
	std::vector<Vector3TrackData>		m_arrPosTrack;	
	std::vector<std::string>				m_arrTransfTrackName;		//Add lhh 2010-11-15
	std::vector<FloatTrackData>		m_arrFloatLSTrack;			//Add lhh 2010-11-15
	std::vector<std::string>				m_arrFloatTrackName;		//Add lhh 2010-11-15





	//------------------------------------------------------------------------------
	//Root Motion
	//------------------------------------------------------------------------------
	Vector3TrackData					m_rootMotionScale;
	QuaternionTrackData				m_rootMotionRot;
	Vector3TrackData					m_rootMotionPos;
	AnimationData()
	{
		m_nFrameNum=0;
		m_nBoneNum=0;
		m_nSocketNum=0;
	}

	void Serialize(SerializeListener& sl,const char* pszLable = "AnimationData");
};

}

#endif