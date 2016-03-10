#ifndef __PoseModifier_H__
#define __PoseModifier_H__

namespace ma
{
	class SkeletonPose;

	class PoseModifier : public Serializable
	{
	
	public:
		PoseModifier();

		~PoseModifier();

		DECL_OBJECT(PoseModifier)

		void			Enable(bool bEnable = true) {m_bEnable = bEnable;}

		bool			IsEnable() const  {return m_bEnable;}

		void			SetGain(float fGain) {m_fGain = fGain;}

		float			GetGain() {return m_fGain;}

		virtual void	UpdatePose(SkeletonPose* pNodePose);

	protected:
		bool  m_bEnable;

		float m_fGain;
	};
}



#endif// __PoseModifier_H__