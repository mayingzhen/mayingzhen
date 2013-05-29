#ifndef  _AnimationTracks__H__
#define  _AnimationTracks__H__

namespace ma
{
	struct AnimationTracks
	{
		std::vector<VECTOR3Track*>		m_scale;
		std::vector<QuaternionTrack*>		m_rot;
		std::vector<VECTOR3Track*>		m_pos;

		std::vector<FloatTrack*>			m_float;

		//------------------------------------------------------------------------------
		//RootMotion
		//------------------------------------------------------------------------------
		// 		VECTOR3Track					m_rootMotionPos;
		// 		QuaternionTrack				m_rootMotionRot;
		// 		VECTOR3Track					m_rootMotionScale;

// 		AnimationTracks();
// 
// 		~AnimationTracks();
// 
// 
// 		void SetPositionTrack(UINT nTrackInd,const VECTOR3Track* pTrack);
// 
// 		void SetRotationTrack(UINT nTrackInd,const QuaternionTrack* pTrack);
// 
// 		void SetScaleTrack(UINT nTrackInd,const VECTOR3Track* pTrack);
// 
// 		void Clear();
// 
// 		void Clone(AnimationTracks* pTracks) const;
// 
// 		UINT ComputeDataSize() const;
	};
}



#endif