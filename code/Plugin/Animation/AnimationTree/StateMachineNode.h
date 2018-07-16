#pragma once

#include "ParameterNode.h"
#include "ParameterHandler.h"

namespace ma
{
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
	class MachineTransition : public Object
	{
	public:

		MachineTransition(MachineState* state);
		~MachineTransition(void);

		DECL_OBJECT(MachineTransition)

		static void			RegisterAttribute();

		bool				AddCondition(Condition mode, const char* pParam, float value);
	
		void				Initial(StateMachineNode *node);
		void				HandleEvent();
		bool				Check();
	
		float				GetBlendTime() const { return m_fBlendTime; }
		void				SetBlendTime(float fBlendTime) { m_fBlendTime = fBlendTime; }

		MachineState*		GetEndState();
		const char*			GetEndState() const;
		void				SetEndState(const char* pszDstState);

		virtual bool		Import(rapidxml::xml_node<>* xmlNode);
		virtual bool		Export(rapidxml::xml_node<>* xmlNode, rapidxml::xml_document<>& doc);

	private:
		friend class MachineState;

		MachineState*	m_pStartState;
	
		std::string		m_strEndState;
		MachineState*	m_pEndState;

		float			m_fBlendTime;

		typedef std::vector< RefPtr<MachineCondition> > CondVec;
		CondVec m_conds;
	};

	// 状态机中的状态
	class MachineState : public Referenced
	{
	public:
		
		MachineState(const char* pszAnimNodeName);
		virtual ~MachineState(void);

		MachineTransition*	AddTransition();

		void				Initial(StateMachineNode *node);

		void				Enter();
		void				Leave();

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

		static void			RegisterAttribute();

		virtual void		Init(AnimationComponent* pAnimator);

		virtual void		Activate();

		virtual void		AdvanceTime(float fTimeElepse);

		virtual void		EvaluateAnimation(AnimationNodeOutput &output, float fWeight);

		void				HandleEvent(MachineTransition *trans);

		void				TransitTo(MachineState* state, float duration);

		bool				SetStartStateId(const char* pszName);

		MachineState*		AddState(const char* pszName);

		MachineState*		GetState(const char* pszName);

		virtual bool		IsReady() { return true; }

		virtual bool		Import(rapidxml::xml_node<>* xmlNode);
		virtual bool		Export(rapidxml::xml_node<>* xmlNode, rapidxml::xml_document<>& doc);

	private:
		MachineState* m_curState = nullptr;		// 当前状态

		MachineState* m_preState = nullptr;

		std::vector< RefPtr<MachineState> > m_vecState;

		std::string m_startStateId;

		friend class MachineTransition;	// 用于在Transition中访问m_animator
	};

	RefPtr<StateMachineNode> CreateStateMachineNode();
}

