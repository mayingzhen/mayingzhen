#ifndef  _Animation_TreeNode__H__
#define  _Animation_TreeNode__H__


namespace ma
{
	struct AnimEvalContext;


	enum EBlendMode
	{
		BLENDMODE_OVERWRITE	= 0,	/**< Overwrite mode. This can be used to switch from for example walk into run. */
		BLENDMODE_ADDITIVE	= 1		/**< Additive mode. This can be used to add the given motion relatively to the current result. */
	};


	class ANIMATION_API AnimTreeNode : public Object
	{
	public:
		virtual void		AdvanceTime(float fTimeElapsed) = 0;

		virtual void		EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight,EBlendMode eBlendMode) = 0;

		virtual	void		SetFrame(float fFrame) = 0;

		virtual void		SetSkeletion(Skeleton* pSkeletion) = 0;

		virtual void		Serialize(Serializer& sl, const char* pszLable = "AnimTreeNode" ) = 0;

		virtual bool		OnLoadOver() = 0;
	};

}

#endif