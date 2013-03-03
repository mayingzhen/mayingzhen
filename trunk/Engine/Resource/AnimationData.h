#ifndef __AnimationData_H__
#define __AnimationData_H__

namespace ma
{

	enum AnimVersion
	{
		EXP_ANIM_VER_INITIAL = 1,

		EXP_ANIM_VER_CURRENT = EXP_ANIM_VER_INITIAL,
	};

	struct ENGINE_API AnimationHeader
	{
		UINT m_nIden;
		UINT m_nVersion;

		void Serialize(SerializeListener& sl,const char* pszLable = "AnimationHeader");
	};

	struct ENGINE_API Vector3TrackData
	{
		std::vector<UINT>		 m_arrFrame;
		std::vector<D3DXVECTOR3> m_arrKey;

		void Serialize(SerializeListener& sl,const char* pszLable = "Vector3Track");
	};

	struct ENGINE_API QuaternionTrackData
	{
		std::vector<UINT>			m_arrFrame;
		std::vector<D3DXQUATERNION> m_arrKey;

		void Serialize(SerializeListener& sl,const char* pszLable = "QuaternionTrack");
	};

	struct ENGINE_API FloatTrackData
	{
		std::vector<UINT>			m_arrFrame;
		std::vector<float>			m_arrKey;

		void Serialize(SerializeListener& sl,const char* pszLable = "FloatTrack" );
	};

	struct ENGINE_API AnimationData : public Resource
	{
		GUID								m_nGlobalSkeletonID;
		UINT								m_nFrameNum;
		UINT								m_nBoneNum;						
		std::vector<Vector3TrackData>		m_arrScaleTrack;
		std::vector<QuaternionTrackData>	m_arrRotTrack;
		std::vector<Vector3TrackData>		m_arrPosTrack;	
		std::vector<std::string>			m_arrTransfTrackName;		

		AnimationData()
		{
			m_nFrameNum = 0;
			m_nBoneNum = 0;
		}

		void Serialize(SerializeListener& sl,const char* pszLable = "AnimationData");
	};
}

#endif
