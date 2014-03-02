#ifndef  _Action__H__
#define  _Action__H__

namespace ma
{
	class IAnimTreeNode;
	class PoseModifier;


	class Action : public IAction
	{
		//DECL_OBJECT(Action)

	public:
		Action(const char* pName = NULL);

		Action(ActionData* pActionData,Skeleton* pSkeleton);

		~Action();

		void			SetTreeNode(IAnimTreeNode* pAnimNode);

		void			AddPoseModifier(IPoseModifier* pPoseModifier);

		void			RemovePoseModifier(IPoseModifier* pPoseModifier);

		void			AdvanceTime(float fTimeElepse);

		void			EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight);

		void			SetFrame(float fFrame);

		void			SetAnimName(const char* pszAnimName) {m_sAnimName = pszAnimName;}

		const char*		GetAnimName() {return m_sAnimName.c_str();}

		//virtual void	Serialize(Serializer& sl, const char* pszLable = "Action");

		Skeleton*		GetSkeleton() {return m_pSkeleton;}

		void			SetSkeleton(Skeleton* pSkeleton); 

		IAnimLayerNode*		CreateLayerNode(AnimLayerNodeData* pLayerData);

		IAnimBlendNode*		CreateBlendNode(AnimBlendNodData* pBlendData);

		IAnimClipNode*		CreateClipNode(AnimClipNodeData* pClipData);

		//IAnimClipNode*		CreateClipNode(const char* pSkaPath,const char* pBonsetName);

	private:
		IAnimTreeNode*		CreateAnimNode(AnimNodeData* pAnimNodeData);

	private:
		std::string		m_sAnimName;

		IAnimTreeNode*	m_pAnimaNode;

		std::vector<PoseModifier*> m_arrPoseModifier;

		Skeleton*		m_pSkeleton;

		ActionData*		m_pActionData;
	};
}

#endif