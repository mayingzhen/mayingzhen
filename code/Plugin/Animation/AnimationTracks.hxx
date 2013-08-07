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
// 		// 		UINT nTrackNum = m_pos.size();
// 		// 		for (UINT trackCnt = 0; trackCnt <nTrackNum; ++ trackCnt)
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
// 		// 		for (UINT nFloatTrackCnt = 0; nFloatTrackCnt < m_float.size(); ++ nFloatTrackCnt)
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
// 	void AnimationTracks::SetPositionTrack(UINT nTrackInd,const Vector3Track* pTrack)
// 	{
// 		//m_pos[nTrackInd]->Clone(pTrack);
// 	}
// 
// 
// 	void AnimationTracks::SetRotationTrack(UINT nTrackInd,const QuaternionTrack* pTrack)
// 	{
// 		//m_rot[nTrackInd]->Clone(pTrack);
// 	}
// 
// 	void AnimationTracks::SetScaleTrack(UINT nTrackInd,const Vector3Track* pTrack)
// 	{
// 		//m_scale[nTrackInd]->Clone(pTrack);
// 	}
// 
// 	void AnimationTracks::Clone(AnimationTracks* pTracks) const
// 	{
// 
// 		// 		ArrayDeepCopy<Array<Vector3Track*>,Vector3Track>(pTracks->m_scale,m_scale);
// 		// 		ArrayDeepCopy<Array<QuaternionTrack*>,QuaternionTrack>(pTracks->m_rot,m_rot);
// 		// 		ArrayDeepCopy<Array<Vector3Track*>,Vector3Track>(pTracks->m_pos,m_pos);
// 		// 		ArrayDeepCopy<Array<FloatTrack*>,FloatTrack>(pTracks->m_float,m_float);
// 		// 
// 		// 		pTracks->m_rootMotionPos = m_rootMotionPos;
// 		// 		pTracks->m_rootMotionRot = m_rootMotionRot;
// 		// 		pTracks->m_rootMotionScale = m_rootMotionScale;
// 	}
// 
// 	UINT AnimationTracks::ComputeDataSize() const
// 	{
// 		// 		UINT nScaleKeyFrameNum = GetKeyFrameNumber(m_scale);
// 		// 		nScaleKeyFrameNum += (UINT)m_rootMotionScale.m_arrValue.size();
// 		// 
// 		// 		UINT nRotKeyFrameNum = GetKeyFrameNumber(m_rot);
// 		// 		nRotKeyFrameNum += (UINT)m_rootMotionRot.m_arrValue.size();
// 		// 
// 		// 		UINT nPosKeyFrameNum = GetKeyFrameNumber(m_pos);
// 		// 		nPosKeyFrameNum += (UINT)m_rootMotionPos.m_arrValue.size();
// 		// 
// 		// 		UINT nDataSize = nScaleKeyFrameNum*(sizeof(Vector3)+sizeof(UINT))
// 		// 			+nPosKeyFrameNum*(sizeof(Vector3)+sizeof(UINT))
// 		// 			+nRotKeyFrameNum*(sizeof(Quaternion)+sizeof(UINT));
// 		// 		return nDataSize;
// 		return 0;
// 	}
}

