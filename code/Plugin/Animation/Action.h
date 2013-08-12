#ifndef  _Action__H__
#define  _Action__H__

namespace ma
{
	class IAnimTreeNode;
	class PoseModifier;

	class Action : public IAction
	{
	public:
		Action(const char* pName = NULL);

		~Action();

		void			SetTreeNode(IAnimTreeNode* pAnimNode);

		void			AddPoseModifier(IPoseModifier* pPoseModifier);

		void			RemovePoseModifier(IPoseModifier* pPoseModifier);

		void			AdvanceTime(float fTimeElepse);

		void			EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight);

		void			SetFrame(float fFrame);

		void			SetAnimName(const char* pszAnimName) {m_sAnimName = pszAnimName;}

		const char*		GetAnimName() {return m_sAnimName.c_str();}

		virtual void	Serialize(Serializer& sl, const char* pszLable = "Action");

		Skeleton*		GetSkeleton() {return m_pSkeleton;}

		void			SetSkeleton(Skeleton* pSkeleton) {m_pSkeleton = pSkeleton;}

		virtual IAnimLayerNode*		CreateLayerNode();

		virtual IAnimBlendNode*		CreateBlendNode();

		virtual IAnimClipNode*		CreateClipNode(const char* pSkaPath,const char* pBonsetName);


	private:
		std::string		m_sAnimName;

		IAnimTreeNode*	m_pAnimaNode;

		std::vector<PoseModifier*> m_arrPoseModifier;

		Skeleton*		m_pSkeleton;
	};
}

#endif