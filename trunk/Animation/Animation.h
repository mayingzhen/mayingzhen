#ifndef  _Animation__H__
#define  _Animation__H__

struct AnimationData;

#include "Animation/Track.h"

namespace ma
{
	struct AnimationTracks;
	class Skeleton;
	class NodeLink;

	class ANIMATION_API Animation
	{
	public: 
		Animation();

		~Animation();

		xmUint GetFrameNumber() {return m_nFrameNumber;}

		void SampleSingleTrackByFrame(maNodeTransform* pTSF, BoneIndex nTrackID,float fFrame) const;

		//bool Load(const char* pszPath);

		void InitWithData(AnimationData* pAniData);

		bool ConverteAnimDataParentToLocalSpaceAnimation(Skeleton* pSkel);

		bool ConverteAnimDataObjectToLocalSpaceAnimation(Skeleton* pSkel);

		UINT GetTansfTrackNumber() {return m_arrTransfTrackName.size();}

		const char* GetTransfTrackNameByIndex(UINT index) {return m_arrTransfTrackName[index].c_str();}

		UINT GetTransfTrackIndexByName(const char* pszName);

	private:
		std::vector<std::string>			m_arrTransfTrackName;
		xmUint								m_nBoneNum;
		xmUint								m_nSocketNum;

		std::vector<std::string>			m_arrFloatTrackName;
		xmUint								m_nFrameNumber;
		bool								m_nFrameNumberDirty;

		AnimationTracks* m_pRawTracks; 
	
	};
}

#endif