#ifndef  _AnimationTracks__H__
#define  _AnimationTracks__H__

namespace ma
{
	struct AnimationTracks
	{
		std::vector<D3DXVECTOR3Track*>		m_scale;
		std::vector<xmQuaternionTrack*>		m_rot;
		std::vector<D3DXVECTOR3Track*>		m_pos;

		std::vector<xmFloatTrack*>			m_float;

		//------------------------------------------------------------------------------
		//RootMotion
		//------------------------------------------------------------------------------
		// 		D3DXVECTOR3Track					m_rootMotionPos;
		// 		xmQuaternionTrack				m_rootMotionRot;
		// 		D3DXVECTOR3Track					m_rootMotionScale;

// 		AnimationTracks();
// 
// 		~AnimationTracks();
// 
// 
// 		void SetPositionTrack(xmUint nTrackInd,const D3DXVECTOR3Track* pTrack);
// 
// 		void SetRotationTrack(xmUint nTrackInd,const xmQuaternionTrack* pTrack);
// 
// 		void SetScaleTrack(xmUint nTrackInd,const D3DXVECTOR3Track* pTrack);
// 
// 		void Clear();
// 
// 		void Clone(AnimationTracks* pTracks) const;
// 
// 		xmUint ComputeDataSize() const;
	};
}



#endif