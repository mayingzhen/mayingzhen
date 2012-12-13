#ifndef  _AnimationAction__H__
#define  _AnimationAction__H__

namespace ma
{
	class AnimTreeNode;
	class PoseModifier;

	// 合成动作（动作树+IK）
	class ANIMATION_API AnimationAction
	{
	public:
		AnimationAction();

		~AnimationAction();

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