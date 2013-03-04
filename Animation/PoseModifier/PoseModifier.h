#ifndef __PoseModifier_H__
#define __PoseModifier_H__

namespace ma
{
	class SkeletonPose;

	struct ANIMATION_API PoseUpdateInfo
	{		
		D3DXMATRIX		m_matWorld;
		D3DXMATRIX		m_matInvWorld;
		float			m_fTimeElapsed;

		//IAnimatedSkeleton* m_pSkelInst;
	};


	class ANIMATION_API PoseModifier
	{
	public:
		PoseModifier() 
		{
			m_bEnable = true;
			m_fGain =1.0f;
		}

		virtual ~PoseModifier() {};

		void Enable(bool bEnable = true) {m_bEnable = bEnable;}

		bool IsEnable() const  {return m_bEnable;}

		void SetGain(float fGain) {m_fGain = fGain;}

		float GetGain() {return m_fGain;}

		virtual void UpdatePose(SkeletonPose* pNodePose) = 0;

protected:
		bool  m_bEnable;

		float m_fGain;
	};
}



#endif// __PoseModifier_H__