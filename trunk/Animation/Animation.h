#ifndef  _Animation__H__
#define  _Animation__H__

class AnimationData;
//class xmVector3Track;
//class xmQuaternionTrack;
//class xmFloatTrack;


#include "Animation/Track.h"

namespace ma
{
	struct ANIMATION_API AnimationTracks
	{
		std::vector<xmVector3Track*>		m_scale;
		std::vector<xmQuaternionTrack*>		m_rot;
		std::vector<xmVector3Track*>		m_pos;

		std::vector<xmFloatTrack*>			m_float;

		//------------------------------------------------------------------------------
		//RootMotion
		//------------------------------------------------------------------------------
// 		xmVector3Track					m_rootMotionPos;
// 		xmQuaternionTrack				m_rootMotionRot;
// 		xmVector3Track					m_rootMotionScale;

		AnimationTracks();

		~AnimationTracks();


		void SetPositionTrack(xmUint nTrackInd,const xmVector3Track* pTrack);

		void SetRotationTrack(xmUint nTrackInd,const xmQuaternionTrack* pTrack);

		void SetScaleTrack(xmUint nTrackInd,const xmVector3Track* pTrack);

		void Clear();

		void Clone(AnimationTracks* pTracks) const;

		xmUint ComputeDataSize() const;
	};

	class ANIMATION_API Animation
	{
	public: 
		Animation();

		~Animation();

		void SampleAndAddSingleTrackByFrame(xmNodeTransform* pTSF,BoneIndex nTrackID,float fWeight,float fFrame) const;

		void SampleSingleTrackByFrame(xmNodeTransform* pTSF, BoneIndex nTrackID,float fFrame) const;

		bool Load(const char* pszPath);

		void EvaluateAnimation(std::vector<maNodeTransform>&m_arrTSFLS);

		void AdvanceTime(float fTimeElapsed);

		void WrapLocalFrame();

	private: 
		void Init(AnimationData* pAniData);

	private:

		float m_fLocalFrame;

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