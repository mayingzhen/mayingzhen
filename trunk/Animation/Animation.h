#ifndef  _Animation__H__
#define  _Animation__H__

class AnimationData;

#include "Animation/Track.h"

namespace ma
{
	class AnimationTracks;
	class Skeleton;
	class NodeLink;

	class ANIMATION_API Animation
	{
	public: 
		Animation();

		~Animation();

		xmUint GetFrameNumber() {return m_nFrameNumber;}

		void SampleAndAddSingleTrackByFrame(xmNodeTransform* pTSF,BoneIndex nTrackID,float fWeight,float fFrame) const;

		void SampleSingleTrackByFrame(xmNodeTransform* pTSF, BoneIndex nTrackID,float fFrame) const;

		bool Load(const char* pszPath);

		bool ConverteAnimDataParentToLocalSpaceAnimation(Skeleton* pSkel);

		UINT GetTansfTrackNumber() {return m_arrTransfTrackName.size();}

		const char* GetTransfTrackNameByIndex(UINT index) {return m_arrTransfTrackName[index].c_str();}

		UINT GetTransfTrackIndexByName(const char* pszName)
		{
			if (pszName == NULL)
				return InvalidID<UINT>();

			for (UINT i = 0; i < m_arrTransfTrackName.size(); ++i)
			{
				const char* pszTrackName = m_arrTransfTrackName[i].c_str();
				if ( _stricmp(pszTrackName,pszName) == 0)
				{
					return i;
				}
			}

			return InvalidID<UINT>();
		}

	private: 
		void Init(AnimationData* pAniData);

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