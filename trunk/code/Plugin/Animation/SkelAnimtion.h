#ifndef  _Action__H__
#define  _Action__H__

namespace ma
{
	class IAnimTreeNode;
	class PoseModifier;

	class ANIMATION_API SkelAnimtion : public Object
	{
	public:
		SkelAnimtion();

		~SkelAnimtion();

		void				SetSkeletion(Skeleton* pSkeleton);
		Skeleton*			GetSkeleton() const {return m_pSkeleton.get();}

		void				SetTreeNode(AnimTreeNode* pAnimNode);

		void				AddPoseModifier(PoseModifier* pPoseModifier);

		void				RemovePoseModifier(PoseModifier* pPoseModifier);

		void				AdvanceTime(float fTimeElepse);

		void				EvaluateAnimation(AnimEvalContext* pAnimContext,float fWeight);

		void				SetFrame(float fFrame);

		void				SetAnimName(const char* pszAnimName);
		const char*			GetAnimName() const; 
		
		virtual void		Serialize(Serializer& sl, const char* pszLable = "SkelAnimtion");

		bool				OnLoadOver();

	private:
		std::string					m_sAnimName;

		RefPtr<AnimTreeNode>		m_pAnimaNode;

		std::vector<PoseModifier*>	m_arrPoseModifier;

		RefPtr<Skeleton>			m_pSkeleton;
	};
}

#endif