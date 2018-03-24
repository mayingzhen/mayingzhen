#pragma once

#include "ParameterNode.h"
#include "ParameterHandler.h"

namespace ma
{

	class Animator;
	class BaseParameter;
	class StateMachineNode;
	class MachineState;
	class BaseConditionHandler;
	class MachineTransition;

	// 状态机中的约束
	class MachineCondition : public Referenced
	{
	public:
		MachineCondition(MachineTransition *trans);
		~MachineCondition();

		void HandleEvent(void);
		bool Check(void);
	public:
		CondValue m_threshold;
		BaseConditionHandler *m_handler = nullptr;
		Condition mode;
		std::string param;
		MachineTransition *m_trans;
	};

	// 状态机中的边
	class MachineTransition : public Referenced
	{
	public:

		MachineTransition(MachineState* state);
		~MachineTransition(void);

		bool AddfloatCondition(Condition mode, const char* pParam, float value);
	
		void Initial(StateMachineNode *node);
		void HandleEvent(void);
		void UpdateFrame(float oldPhase, float newPhase);
		bool Check(bool arriveExitTime = false);
		void RestartExitTimer(void);
		MachineState* GetTransState(void);

		virtual bool		Import(rapidxml::xml_node<>* xmlNode);
		virtual bool		Export(rapidxml::xml_node<>* xmlNode, rapidxml::xml_document<>& doc);

	public:
		float m_duration;
		std::string m_dstStateId;
		bool m_absoluteTransitDuration;

	private:
		friend class MachineState;

		float m_exitTime;
		float m_remainingExitTime;

		MachineState* m_startState;
		MachineState* m_endState;
		typedef std::vector< RefPtr<MachineCondition> > CondVec;
		CondVec m_conds;
	};

	// 状态机中的状态
	class MachineState : public Referenced
	{
	public:
		
		MachineState(const char* pszAnimNodeName);
		virtual ~MachineState(void);

		MachineTransition* AddTransition(const char* dstStateId, float duration, float exitTime, bool absoluteTransitDuration);

		void Initial(StateMachineNode *node);

		void Enter(void);
		void Leave(void);

		virtual bool		Import(rapidxml::xml_node<>* xmlNode);
		virtual bool		Export(rapidxml::xml_node<>* xmlNode, rapidxml::xml_document<>& doc);

	public:
		StateMachineNode* m_node = nullptr;
		
		typedef std::vector< RefPtr<MachineTransition> > TransitionVec;
		TransitionVec m_trans;

		RefPtr<AnimTreeNode> m_pAnimNode;
		std::string m_strName;
	};

	RefPtr<MachineState> CreateMachineState(const char* pszAnimNodeName);

	// 状态机节点
	class StateMachineNode : public AnimTreeNode
	{
	public:
		StateMachineNode();
		virtual ~StateMachineNode(void);

		DECL_OBJECT(StateMachineNode)

		static void		RegisterAttribute();

		virtual void  Init(AnimationComponent* pAnimator);

		MachineState* AddState(const char* pszName);

		MachineState* GetState(const char* pszName);

		virtual void		AdvanceTime(float fTimeElepse);

		virtual void		EvaluateAnimation(AnimationNodeOutput &output, float fWeight);

		void HandleEvent(MachineTransition *trans);
		void TransitTo(MachineState* state, float duration);

		bool SetStartStateId(const char* pszName);

		virtual std::string GetNodePrint(void);
		//virtual void Reset(void);
		virtual void Activate();

		virtual bool		IsReady() { return true; }

		virtual bool		Import(rapidxml::xml_node<>* xmlNode);
		virtual bool		Export(rapidxml::xml_node<>* xmlNode, rapidxml::xml_document<>& doc);

	private:
		MachineState* m_curState = nullptr;		// 当前状态

		MachineState* m_preState = nullptr;

		std::vector< RefPtr<MachineState> > m_vecState;

		std::string m_startStateId;

		bool m_resetOnActive = false;

		friend class MachineTransition;	// 用于在Transition中访问m_animator
	};

	RefPtr<StateMachineNode> CreateStateMachineNode();
}

