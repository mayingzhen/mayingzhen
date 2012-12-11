#ifndef  _SkeletonAnimation__H__
#define  _SkeletonAnimation__H__

namespace ma
{
	class AnimTreeNode;
	class PoseModifier;

	// �ϳɶ�����������+IK��
	class ANIMATION_API SkeletonAnimation
	{
	public:
		SkeletonAnimation();

		~SkeletonAnimation();

		void SetTreeNode(AnimTreeNode* pAnimNode);

		void AddPoseModifier(PoseModifier* pPoseModifier);

		void AdvanceTime(float fTimeElepse);

		void EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight);

		void SetAnimName(const char* pszAnimName) {m_sAnimName = pszAnimName;}

		const char* GetAnimName() {return m_sAnimName.c_str();}

	private:
		std::string		m_sAnimName;

		AnimTreeNode*	m_pAnimaNode;

		std::vector<PoseModifier*> m_arrPoseModifier;
	};
}

#endif