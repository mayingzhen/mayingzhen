#ifndef  _Action__H__
#define  _Action__H__

namespace ma
{
	class AnimTreeNode;
	class PoseModifier;

	class ANIMATION_API Action
	{
	public:
		Action();

		~Action();

		void			SetTreeNode(AnimTreeNode* pAnimNode);

		void			AddPoseModifier(PoseModifier* pPoseModifier);

		void			AdvanceTime(float fTimeElepse);

		void			EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight);

		void			SetFrame(float fFrame);

		void			SetAnimName(const char* pszAnimName) {m_sAnimName = pszAnimName;}

		const char*		GetAnimName() {return m_sAnimName.c_str();}

		virtual void	Serialize(Serializer& sl, const char* pszLable = "Action");

	private:
		std::string		m_sAnimName;

		AnimTreeNode*	m_pAnimaNode;

		std::vector<PoseModifier*> m_arrPoseModifier;
	};
}

#endif