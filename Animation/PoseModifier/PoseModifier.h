#ifndef __PoseModifier_H__
#define __PoseModifier_H__


struct PoseUpdateInfo
{		
	xmMatrix4x4		m_matWorld;
	xmMatrix4x4		m_matInvWorld;
	float			m_fTimeElapsed;

	//IS3AAnimatedSkeleton* m_pSkelInst;
};


class ANIMATION_API PoseModifier
{
public:
	PoseModifier();

	virtual ~PoseModifier() = 0;

	void Enable(bool bEnable = true) {m_bEnable = bEnable;}

	bool IsEnable() const  {return m_bEnable;}

	void SetGain(float fGain) {m_fGain = fGain;}

	float GetGain() {return m_fGain;}

	virtual void UpdatePose(NodePose* pNodePose, const PoseUpdateInfo& updateInfo) = 0;

private:
	bool  m_bEnable;

	float m_fGain;
};

#endif// __PoseModifier_H__