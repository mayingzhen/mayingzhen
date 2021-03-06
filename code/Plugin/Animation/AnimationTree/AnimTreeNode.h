#ifndef  _Animation_TreeNode__H__
#define  _Animation_TreeNode__H__

#include "AnimationNodeOutput.h"

namespace ma
{
	class FrameEvent;
	class PoseModifier;
	class SkeletonPose;
	class AnimationComponent;
	class MachineTransition;
	class BlendNodeChildState;

	class IAnimCallback
	{
	public:
		virtual void OnEnter(AnimTreeNode* pAnim) = 0;
		virtual void OnLeave(AnimTreeNode* pAnim) = 0; 
		virtual void OnPlay(AnimTreeNode* pAnim) = 0;
		virtual void OnPause(AnimTreeNode* pAnim) = 0;
		virtual void OnStop(AnimTreeNode* pAnim) = 0;
		virtual void OnFrame(AnimTreeNode* pAnim,FrameEvent* evnt)  = 0;
	};

	struct AnimEvalContext
	{
		std::vector<Transform> m_arrTSFPS;
		std::vector<bool> m_arrFirst;

		const SkeletonPose* m_refNodePos;

		SkeletonPose* m_pNodePos;
	};

	struct SynchronizationInfo
	{
		float duration;

		float phase;

		SynchronizationInfo() : duration(0.0f), phase(0.0f) { }
	};

	class AnimatorCallback
	{
	public:
		AnimatorCallback(AnimationComponent* animator) : m_animator(animator)
		{
		}

		virtual void Execute() = 0;
	protected:
		AnimationComponent* m_animator;
	};

	class ActiveCallback
	{
	public:
		ActiveCallback(AnimationComponent* animator) :
			m_animator(animator)
		{

		}

		~ActiveCallback()
		{
		}


	private:
		AnimationComponent* m_animator;
	};


	class ExitTimeCallback : public AnimatorCallback
	{
	public:
		ExitTimeCallback(float normalizedTime, AnimationComponent* animator) :
			AnimatorCallback(animator), normalizedTime(normalizedTime)
		{
		}

		virtual ~ExitTimeCallback()
		{
		}

		void UpdateFrame(float oldPhase, float newPhase)
		{
			if ((oldPhase < newPhase && oldPhase < normalizedTime && normalizedTime <= newPhase) ||
				(oldPhase > newPhase && (oldPhase < normalizedTime || normalizedTime <= newPhase)))
			{
				//m_animator->m_callback_vec.push_back(this);
			}
		}

	private:
		float normalizedTime;
	};

	class StateEventCallback : public AnimatorCallback
	{
	public:
		StateEventCallback(std::string sig, AnimationComponent* animator)
			: AnimatorCallback(animator), m_signal(sig)
		{

		}
		void Execute()
		{
			//m_animator->onStateLeave(m_signal);
		}

	private:
		std::string m_signal;
	};

	class AnimTreeNode : public Serializable
	{
	public:
		
		AnimTreeNode();

		virtual ~AnimTreeNode();

		DECL_OBJECT(AnimTreeNode)

		static void			RegisterAttribute();

		static uint32_t		AnimNameToID(const char* pszName);

		virtual void		AdvanceTime(float fTimeElepse) = 0;

		virtual void		EvaluateAnimation(AnimationNodeOutput &output, float fWeight) = 0;

		virtual bool		IsReady() = 0;

		const char*			GetName() const;
		void				SetName(const char* pszName);	
		uint32_t			GetAnimID() const {return m_nAnimID;}

		float				GetFadeTime() const {return m_fFadeTime;}
		void				SetFadeTime(float fFadeTime) {m_fFadeTime = fFadeTime;}

		void				AddPoseModifier(PoseModifier* pPoseModifier);
		void				ProcessPoseModifier(SkeletonPose* pose, Skeleton* pSkeleton, float fWeight);			

		virtual bool		Import(rapidxml::xml_node<>* xmlNode);
		virtual bool		Export(rapidxml::xml_node<>* xmlNode,rapidxml::xml_document<>& doc);

		void				SetAnimCallBack(IAnimCallback* pCallBack) {m_pCallBack = pCallBack;}
		IAnimCallback*		GetAnimCallBack() const {return m_pCallBack;}

		BoneIndex			GetBoneIndex(const char *boneName) const;
		uint32_t			GetBoneCount() const;
		
		virtual void		Init(AnimationComponent* pAnimator);

		virtual void		Activate();
		virtual void		Deactivate() { m_active = false; }
		bool				IsActive() {return m_active;}

		AnimationComponent* GetAnimator() { return m_animator; }

	protected:
		std::string			m_strName;
		uint32_t			m_nAnimID;

		AnimationComponent* m_animator = nullptr;

		float				m_fFadeTime;

		bool				m_active;

		IAnimCallback*		m_pCallBack;

		typedef vector< RefPtr<PoseModifier> > VEC_PoseModifier;
		VEC_PoseModifier	m_vecPoseModifier;

	};

}

#endif
