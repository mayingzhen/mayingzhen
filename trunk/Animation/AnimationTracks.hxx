#include "Animation/AnimationTracks.h"

namespace ma
{
// 	AnimationTracks::AnimationTracks()
// 	{
// 
// 	}
// 
// 	AnimationTracks::~AnimationTracks()
// 	{
// 		Clear();
// 	}
// 
// 	void AnimationTracks::Clear()
// 	{
// 		// 		xmUint nTrackNum = m_pos.size();
// 		// 		for (xmUint trackCnt = 0; trackCnt <nTrackNum; ++ trackCnt)
// 		// 		{
// 		// 			delete m_scale[trackCnt];
// 		// 			delete m_rot[trackCnt];
// 		// 			delete m_pos[trackCnt];
// 		// 		}
// 		// 		m_scale.clear();
// 		// 		m_rot.clear();
// 		// 		m_pos.clear();
// 		// 
// 		// 
// 		// 		for (xmUint nFloatTrackCnt = 0; nFloatTrackCnt < m_float.size(); ++ nFloatTrackCnt)
// 		// 		{
// 		// 			delete m_float[nFloatTrackCnt];
// 		// 		}
// 		// 		m_float.clear();
// 		// 
// 		// 
// 		// 		m_rootMotionScale.Clear();
// 		// 		m_rootMotionRot.Clear();
// 		// 		m_rootMotionPos.Clear();
// 
// 	}
// 
// 	void AnimationTracks::SetPositionTrack(xmUint nTrackInd,const xmVector3Track* pTrack)
// 	{
// 		//m_pos[nTrackInd]->Clone(pTrack);
// 	}
// 
// 
// 	void AnimationTracks::SetRotationTrack(xmUint nTrackInd,const xmQuaternionTrack* pTrack)
// 	{
// 		//m_rot[nTrackInd]->Clone(pTrack);
// 	}
// 
// 	void AnimationTracks::SetScaleTrack(xmUint nTrackInd,const xmVector3Track* pTrack)
// 	{
// 		//m_scale[nTrackInd]->Clone(pTrack);
// 	}
// 
// 	void AnimationTracks::Clone(AnimationTracks* pTracks) const
// 	{
// 
// 		// 		S3AArrayDeepCopy<S3AArray<xmVector3Track*>,xmVector3Track>(pTracks->m_scale,m_scale);
// 		// 		S3AArrayDeepCopy<S3AArray<xmQuaternionTrack*>,xmQuaternionTrack>(pTracks->m_rot,m_rot);
// 		// 		S3AArrayDeepCopy<S3AArray<xmVector3Track*>,xmVector3Track>(pTracks->m_pos,m_pos);
// 		// 		S3AArrayDeepCopy<S3AArray<xmFloatTrack*>,xmFloatTrack>(pTracks->m_float,m_float);
// 		// 
// 		// 		pTracks->m_rootMotionPos = m_rootMotionPos;
// 		// 		pTracks->m_rootMotionRot = m_rootMotionRot;
// 		// 		pTracks->m_rootMotionScale = m_rootMotionScale;
// 	}
// 
// 	xmUint AnimationTracks::ComputeDataSize() const
// 	{
// 		// 		xmUint nScaleKeyFrameNum = GetKeyFrameNumber(m_scale);
// 		// 		nScaleKeyFrameNum += (xmUint)m_rootMotionScale.m_arrValue.size();
// 		// 
// 		// 		xmUint nRotKeyFrameNum = GetKeyFrameNumber(m_rot);
// 		// 		nRotKeyFrameNum += (xmUint)m_rootMotionRot.m_arrValue.size();
// 		// 
// 		// 		xmUint nPosKeyFrameNum = GetKeyFrameNumber(m_pos);
// 		// 		nPosKeyFrameNum += (xmUint)m_rootMotionPos.m_arrValue.size();
// 		// 
// 		// 		xmUint nDataSize = nScaleKeyFrameNum*(sizeof(xmVector3)+sizeof(xmUint))
// 		// 			+nPosKeyFrameNum*(sizeof(xmVector3)+sizeof(xmUint))
// 		// 			+nRotKeyFrameNum*(sizeof(xmQuaternion)+sizeof(xmUint));
// 		// 		return nDataSize;
// 		return 0;
// 	}
}
