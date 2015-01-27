#ifndef __PoseModifier_H__
#define __PoseModifier_H__

namespace ma
{
	class SkeletonPose;

	struct PoseUpdateInfo
	{		
		Matrix4		m_matWorld;
		Matrix4		m_matInvWorld;
		float			m_fTimeElapsed;

		//IAnimatedSkeleton* m_pSkelInst;
	};


	class PoseModifier : public Object
	{
		DECL_OBJECT(PoseModifier)

	public:
		PoseModifier();

		~PoseModifier();

		void			Enable(bool bEnable = true) {m_bEnable = bEnable;}

		bool			IsEnable() const  {return m_bEnable;}

		void			SetGain(float fGain) {m_fGain = fGain;}

		float			GetGain() {return m_fGain;}

		virtual void	UpdatePose(SkeletonPose* pNodePose);

		virtual void	Serialize(Serializer& sl, const char* pszLable = "PoseModifier");

	protected:
		bool  m_bEnable;

		float m_fGain;
	};

	DeclareRefPtr(PoseModifier)
}



#endif// __PoseModifier_H__