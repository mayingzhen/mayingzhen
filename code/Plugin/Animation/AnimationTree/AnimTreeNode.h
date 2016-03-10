#ifndef  _Animation_TreeNode__H__
#define  _Animation_TreeNode__H__


namespace ma
{
	class FrameEvent;
	class PoseModifier;
	class SkeletonPose;

	struct AnimEvalContext
	{
		std::vector<Transform> m_arrTSFPS;

		const SkeletonPose* m_refNodePos;

		SkeletonPose* m_pNodePos;
	};

	class AnimTreeNode : public Serializable, public Animatable
	{
	public:
		
		AnimTreeNode();

		virtual ~AnimTreeNode();

		DECL_OBJECT(AnimTreeNode)

		static void			RegisterAttribute();

		static uint32		AnimNameToID(const char* pszName);

		virtual void		AdvanceTime(float fTimeElepse);

		virtual void		EvaluateAnimation(AnimEvalContext* pEvalContext, float fWeight) = 0;

		virtual bool		Instantiate(Skeleton* pSkeleton) = 0;

		virtual bool		IsReady() = 0;

		virtual uint32		GetFrameCount() = 0;

		const char*			GetName() const;
		void				SetName(const char* pszName);	

		uint32				GetAnimID() const {return m_nAnimID;}

// 		uint32				GetLoop() const {return m_nLoops == 0 ? -1 : m_nLoops;}
// 		void				SetLoop(uint32 nLoop) {m_nLoops = nLoop;}
// 
// 		float				GetPlaySpeed() const {return m_fSpeed;}
// 		void				SetPlaySpeed(float fPlaySpeed) {m_fSpeed = fPlaySpeed;}
// 
// 		PlayerStatus		GetPlayState() const {return m_ePlayerStatus;}
// 		void				SetPlayState(PlayerStatus ePlayerStatus) {m_ePlayerStatus = ePlayerStatus;}

		float				GetFadeTime() const {return m_fFadeTime;}
		void				SetFadeTime(float fFadeTime) {m_fFadeTime = fFadeTime;}

		void				ProcessPoseModifier(SkeletonPose* pose, float fWeight);
		
		virtual	void		SetFrame(float fFrame);

		virtual bool		Import(rapidxml::xml_node<>* xmlNode);
		virtual bool		Export(rapidxml::xml_node<>* xmlNode,rapidxml::xml_document<>& doc);

	protected:
		void				ProcessFrameEvent(float fFrameFrome,float fFrameTo);
		
	protected:
		std::string			m_strName;
		uint32				m_nAnimID;

		float				m_fFadeTime;

		float				m_fLocalFrame;

		typedef vector< RefPtr<FrameEvent> > VEC_FRAMEEVENT; 
		VEC_FRAMEEVENT		m_vecFrameEvent;

		typedef vector< RefPtr<PoseModifier> > VEC_PoseModifier;
		VEC_PoseModifier	m_vecPoseModifier;
	};

}

#endif
