#ifndef  _Animation_TreeNode__H__
#define  _Animation_TreeNode__H__


namespace ma
{
	struct AnimEvalContext
	{
		std::vector<Transform> m_arrTSFPS;

		const SkeletonPose* m_refNodePos;

		SkeletonPose* m_pNodePos;
	};


	class AnimTreeNode : public Object
	{
	public:

		DECL_OBJECT(AnimTreeNode)

		static void			RegisterAttribute();

		virtual void		AdvanceTime(float fTimeElapsed) = 0;

		virtual void		EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight) = 0;

		virtual	void		SetFrame(float fFrame) = 0;

		virtual bool		Instantiate(Skeleton* pSkeleton) = 0;

		virtual bool		IsReady() = 0;

		const char*			GetName() const;
		void				SetName(const char* pszName);

	protected:
		std::string			m_strName;

	};

}

#endif
