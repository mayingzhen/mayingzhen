#ifndef  _Action__H__
#define  _Action__H__

namespace ma
{
	class IAnimTreeNode;
	class PoseModifier;

	class ANIMATION_API SkelAnimtion : public Object
	{
	public:
		SkelAnimtion(const char* pName,Skeleton* pSkeleton);

		SkelAnimtion(SkelAnimData* pSkelAnimData,Skeleton* pSkeleton);

		~SkelAnimtion();

		void				SetTreeNode(AnimTreeNode* pAnimNode);

		void				AddPoseModifier(PoseModifier* pPoseModifier);

		void				RemovePoseModifier(PoseModifier* pPoseModifier);

		void				AdvanceTime(float fTimeElepse);

		void				EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight);

		void				SetFrame(float fFrame);

		void				SetAnimName(const char* pszAnimName) {m_sAnimName = pszAnimName;}

		const char*			GetAnimName() {return m_sAnimName.c_str();}

		Skeleton*			GetSkeleton() {return m_pSkeleton;}

		AnimLayerNode*		CreateLayerNode(AnimLayerNodeData* pLayerData = NULL);

		AnimBlendNode*		CreateBlendNode(AnimBlendNodData* pBlendData = NULL);

		AnimClipNode*		CreateClipNode(AnimClipNodeData* pClipData = NULL);

		AnimClipNode*		CreateClipNode(const char* pSkaPath,const char* pBonsetName);

	private:
		AnimTreeNode*		CreateAnimNode(AnimNodeData* pAnimNodeData);

	private:
		std::string					m_sAnimName;

		AnimTreeNode*				m_pAnimaNode;

		std::vector<PoseModifier*>	m_arrPoseModifier;

		Skeleton*					m_pSkeleton;

		SkelAnimData*				m_pSkelAnimData;
	};
}

#endif