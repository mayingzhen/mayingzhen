#ifndef  _IAction__H__
#define  _IAction__H__

namespace ma
{
	struct AnimEvalContext;
	class IAnimLayerNode;
	class IAnimClipNode;
	class IAnimBlendNode;
	class Skeleton;
	class AnimLayerNodeData;
	class AnimBlendNodData;
	class AnimClipNodeData;


	enum EBlendMode
	{
		BLENDMODE_OVERWRITE	= 0,	/**< Overwrite mode. This can be used to switch from for example walk into run. */
		BLENDMODE_ADDITIVE	= 1		/**< Additive mode. This can be used to add the given motion relatively to the current result. */
	};

	class ENGINE_API IAnimTreeNode : public Object
	{
	public:
		virtual void		AdvanceTime(float fTimeElapsed) = 0;

		virtual void		EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight,EBlendMode eBlendMode) = 0;

		virtual	void		SetFrame(float fFrame) = 0;

		//virtual void		SetSkeleton(Skeleton* pSkeleton) = 0;

		//virtual void		Serialize(Serializer& sl, const char* pszLable = "AnimTreeNode" ) = 0;
	};


	class ENGINE_API IAnimBlendNode : public IAnimTreeNode
	{
	public:
		virtual void SetSrcAnimNode(IAnimTreeNode* pAnimNode) = 0;

		virtual void SetDestAnimNode(IAnimTreeNode* pAnimNode) = 0; 

		virtual void SetWeight(float fWeight) = 0;
	};


	class ENGINE_API IAnimLayerNode : public IAnimTreeNode
	{
	public:
		virtual void AddLayer(IAnimTreeNode* pAnimTreeNode) = 0;

		virtual void RemoveLayer(IAnimTreeNode* pAnimTreeNode) = 0;
	};


	class ENGINE_API IAnimClipNode : public IAnimTreeNode
	{
	public:
		virtual void	SetAnimationClip(const char* pszSkaPath) = 0;

		virtual void	SetBoneSet(const char* pBoneSetName) = 0;
	};

	class ENGINE_API IPoseModifier : public Object
	{
	public:
		virtual void	Enable(bool bEnable = true) = 0;

		virtual bool	IsEnable() = 0;

		virtual void	SetGain(float fGain)  = 0;

		virtual float	GetGain()  = 0;
	};

	class ENGINE_API IAction : public Object
	{
	public:
		virtual void				SetTreeNode(IAnimTreeNode* pAnimNode) = 0;

		virtual void				AddPoseModifier(IPoseModifier* pPoseModifier) = 0;

		virtual void				RemovePoseModifier(IPoseModifier* pPoseModifier) = 0;

		virtual void				SetFrame(float fFrame) = 0;

		virtual void				SetAnimName(const char* pszAnimName) = 0;

		virtual const char*			GetAnimName()  = 0;

		virtual IAnimLayerNode*		CreateLayerNode(AnimLayerNodeData* pLayerData = NULL) = 0;

		virtual IAnimBlendNode*		CreateBlendNode(AnimBlendNodData* pBlendData = NULL) = 0;

		virtual IAnimClipNode*		CreateClipNode(AnimClipNodeData* pClipData = NULL) = 0;

		virtual IAnimClipNode*		CreateClipNode(const char* pSkaPath,const char* pBonsetName) = 0;

	};
}

#endif // _IAction__H__