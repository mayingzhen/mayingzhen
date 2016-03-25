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

		virtual	void	SetGoalObjectSpace(const Vector3& vGoalOS) {}

		virtual void	UpdatePose(SkeletonPose* pNodePose,Skeleton* pSkeleton,float fWeight) = 0;

	protected:
		bool			m_bEnable;
	};
}



#endif// __PoseModifier_H__