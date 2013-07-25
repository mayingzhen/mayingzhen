#ifndef __AnimationData_H__
#define __AnimationData_H__

namespace ma
{

	struct ENGINE_API AnimationHeader
	{
		UINT m_nIden;
		UINT m_nVersion;

		AnimationHeader();
	};

	struct ENGINE_API Vector3TrackData
	{
		std::vector<UINT>		m_arrFrame;
		std::vector<Vector3>	m_arrKey;

	};

	struct ENGINE_API QuaternionTrackData
	{
		std::vector<UINT>			m_arrFrame;
		std::vector<Quaternion>		m_arrKey;
	};

	struct ENGINE_API AnimationData : public Resource
	{
		DECL_OBJECT(AnimationData)
	
	public:

		maGUID								m_nGlobalSkeletonID;
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

		virtual void CreateFromMemeory();

		virtual void Serialize(Serializer& sl, const char* pszLable = "AnimationData");

		virtual void SaveToFile(const char* pPath);
	};
}

#endif
