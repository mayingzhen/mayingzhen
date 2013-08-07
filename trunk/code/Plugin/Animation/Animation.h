#ifndef  _Animation__H__
#define  _Animation__H__


#include "Animation/Track.h"

namespace ma
{
	class ANIMATION_API Animation : public Resource
	{
		DECL_OBJECT(Animation)

	public: 
		Animation();

		~Animation();

		UINT							GetFrameNumber() {return m_nFrameNumber;}

		void							SampleSingleTrackByFrame(NodeTransform* pTSF, BoneIndex nTrackID,float fFrame) const;

		void							Serialize(Serializer& sl, const char* pszLable);

		UINT							GetTansfTrackNumber() {return m_arrTransfTrackName.size();}

		const char*						GetTransfTrackNameByIndex(UINT index) {return m_arrTransfTrackName[index].c_str();}

		UINT							GetTransfTrackIndexByName(const char* pszName);

// 		std::vector<Vector3Track*>&		GetArrScaleTrack() {return m_arrScaleTrack;}
// 
// 		std::vector<QuaternionTrack*>&	GetArrRotTrack() {return m_arrRotTrack;}
// 
// 		std::vector<Vector3Track*>&		GetArrPosTrack() {return m_arrPosTrack;}
// 		
// 		std::vector<std::string>		GetArrTransfTrackName() {return m_arrTransfTrackName;}

	public:
		std::vector<std::string>		m_arrTransfTrackName;
		UINT							m_nFrameNumber;
		std::vector<Vector3Track>		m_arrScaleTrack;
		std::vector<QuaternionTrack>	m_arrRotTrack;
		std::vector<Vector3Track>		m_arrPosTrack;
	};
}

#endif
