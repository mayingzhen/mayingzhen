#include "StateMachineNode.h"
#include "AnimationNodeOutput.h"
#include "ParameterHandler.h"

namespace ma
{

	/* 状态机状态转换的约束 */

	MachineCondition::MachineCondition(MachineTransition *trans) :
		m_trans(trans),
		m_handler(NULL)
	{
	}

	MachineCondition::~MachineCondition()
	{
		if (m_handler)
		{
			delete m_handler;
			m_handler = NULL;
		}
		m_trans = NULL;
	}

	void MachineCondition::HandleEvent(void)
	{
		m_trans->HandleEvent();
	}

	bool MachineCondition::Check(void)
	{
		return m_handler->Check();
	}

	/* 状态机状态转换的边 */

	MachineTransition::MachineTransition(MachineState* state) :
		m_pStartState(state)
	{
	}

	MachineTransition::~MachineTransition(void)
	{
	}

	void MachineTransition::RegisterAttribute()
	{
		ACCESSOR_ATTRIBUTE(MachineTransition, "dstState", GetEndState, SetEndState, const char*, NULL, AM_DEFAULT);
		ACCESSOR_ATTRIBUTE(MachineTransition, "blendTime", GetBlendTime, SetBlendTime, float, NULL, AM_DEFAULT);
	}


	bool MachineTransition::AddCondition(Condition mode, const char* pParam, float value)
	{
		MachineCondition* cond = new MachineCondition(this);
		m_conds.push_back(cond);

		cond->mode = mode;
		cond->param = pParam;
		cond->m_threshold.Setfloat(value);

		return true;
	}

	void MachineTransition::HandleEvent(void)
	{
		if (m_pStartState == m_pStartState->m_node->m_curState)
		{
			m_pStartState->m_node->HandleEvent(this);
		}
	}

	MachineState* MachineTransition::GetEndState(void)
	{
		return m_pEndState;
	}

	const char* MachineTransition::GetEndState() const
	{
		return m_strEndState.c_str();
	}

	void MachineTransition::SetEndState(const char* pszDstState)
	{
		ASSERT(pszDstState);
		m_strEndState = pszDstState ? pszDstState : "";
	}

	void MachineTransition::Initial(StateMachineNode *node)
	{
		m_pEndState = node->GetState(m_strEndState.c_str());
		ASSERT(m_pEndState);

		ParameterNode* pParmater = node->GetAnimator()->GetParameterNode();

		for (auto it = m_conds.begin(); it != m_conds.end(); ++it)
		{
			MachineCondition* cond = (*it).get();

			BaseParameter* pParam = pParmater->GetParameter(cond->param.c_str());
			ASSERT(pParam);
			cond->m_handler = BaseConditionHandler::CreateHandler(cond->mode, cond, pParam);
		}

		return;
	}

	bool MachineTransition::Check()
	{
		for (auto it = m_conds.begin(); it != m_conds.end(); ++it)
		{
			if (!(*it)->Check())
			{
				return false;
			}
		}

		return (m_conds.size() > 0);
	}

	bool MachineTransition::Import(rapidxml::xml_node<>* xmlNode)
	{
		Object::Import(xmlNode);

		rapidxml::xml_node<>* pXmlCondition = xmlNode->first_node("Condition");
		while (pXmlCondition)
		{
			const char* pszMode = pXmlCondition->findAttribute("mode");
			const char* pszParam = pXmlCondition->findAttribute("param");
			const char* pszThreshold = pXmlCondition->findAttribute("threshold");

			MachineCondition* cond = new MachineCondition(this);
			m_conds.push_back(cond);

			cond->mode = StringUtil::StringToEnum<Condition>(pszMode, strDECL_Condition);
			cond->param = pszParam;
			cond->m_threshold.Setfloat(StringConverter::parseReal(pszThreshold));
			

			pXmlCondition = pXmlCondition->next_sibling("Condition");
		}

		return true;
	}

	bool MachineTransition::Export(rapidxml::xml_node<>* xmlNode, rapidxml::xml_document<>& doc)
	{
		Object::Export(xmlNode,doc);

		for (uint32_t i = 0; i < m_conds.size(); ++i)
		{
			MachineCondition* pCond = m_conds[i].get();

			rapidxml::xml_node<>* xmlCondition = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Condition"));
			xmlNode->append_node(xmlCondition);

			const char* pszMode = strDECL_Condition[static_cast<int>(pCond->mode)];
			rapidxml::append_attribute(xmlCondition, doc, "mode", pszMode);
			
			rapidxml::append_attribute(xmlCondition, doc, "param", pCond->param.c_str());

			std::string strValue = StringConverter::toString(pCond->m_threshold.Getfloat());
			rapidxml::append_attribute(xmlCondition, doc, "threshold", strValue.c_str());
		}

		return true;
	}

	/* 状态机包含的状态节点 */
	MachineState::MachineState(const char* pszAnimNodeName) 
	{
		m_strName = pszAnimNodeName ? pszAnimNodeName : "";
	}

	MachineState::~MachineState(void)
	{

	}

	MachineTransition* MachineState::AddTransition()
	{
		MachineTransition* tran = new MachineTransition(this);
		m_trans.push_back(tran);

		return tran;
	}

	void MachineState::Enter()
	{
		m_pAnimNode->Activate();
	}

	void MachineState::Leave()
	{
		m_pAnimNode->Deactivate();
	}

	void MachineState::Initial(StateMachineNode *node)
	{
		m_node = node;

		AnimationSet* pAnimSet = node->GetAnimator()->GetAnimationSet();
		ASSERT(pAnimSet);
		if (pAnimSet == NULL)
			return;

		m_pAnimNode = pAnimSet->GetSkelAnimByName(m_strName.c_str());

		m_pAnimNode->Init(node->GetAnimator());

		for (auto it = m_trans.begin(); it != m_trans.end(); ++it)
		{
			MachineTransition* pTrans = (*it).get();

			pTrans->Initial(node);
		}
	}

	bool MachineState::Import(rapidxml::xml_node<>* xmlNode)
	{
		const char* pszAnimName = xmlNode->findAttribute("AnimName");
		m_strName = pszAnimName ? pszAnimName : "";

		rapidxml::xml_node<>* pXmlTransition = xmlNode->first_node("Transition");
		while (pXmlTransition)
		{
			MachineTransition* pTrans = new MachineTransition(this);
			m_trans.push_back(pTrans);
			
			pTrans->Import(pXmlTransition);

			pXmlTransition = pXmlTransition->next_sibling("Transition");
		}

		return true;
	}

	bool MachineState::Export(rapidxml::xml_node<>* xmlNode, rapidxml::xml_document<>& doc)
	{
		rapidxml::append_attribute(xmlNode, doc, "AnimName", m_strName.c_str());

		for (auto it = m_trans.begin(); it != m_trans.end(); ++it)
		{
			rapidxml::xml_node<>* pXmlTransition = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Transition"));
			xmlNode->append_node(pXmlTransition);

			MachineTransition* pTrans = (*it).get();
		
			pTrans->Export(pXmlTransition, doc);
		}

		return true;
	}

	RefPtr<MachineState> CreateMachineState(const char* pszAnimNodeName)
	{
		return new MachineState(pszAnimNodeName);
	}

	StateMachineNode::StateMachineNode() 
	{

	}

	StateMachineNode::~StateMachineNode(void)
	{

	}

	void StateMachineNode::RegisterAttribute()
	{
		REGISTER_OBJECT(StateMachineNode, CreateStateMachineNode);
	}

	void StateMachineNode::Init(AnimationComponent* pAnimator)
	{
		AnimTreeNode::Init(pAnimator);

		for (uint32_t i = 0; i < m_vecState.size(); ++i)
		{
			MachineState* pState = m_vecState[i].get();

			pState->Initial(this);
		}

		m_curState = this->GetState(m_startStateId.c_str());
		ASSERT(m_curState);
	}

	void StateMachineNode::Activate()
	{
		AnimTreeNode::Activate();

		m_curState->Enter();
	}

	MachineState* StateMachineNode::AddState(const char* pszName)
	{
		MachineState* pState = GetState(pszName);
		ASSERT(pState == NULL);
		if (pState)
			return NULL;

		MachineState* pMachineState = new MachineState(pszName);

		m_vecState.push_back(pMachineState);

		return pMachineState;
	}

	MachineState* StateMachineNode::GetState(const char* pszName)
	{
		for (uint32_t i = 0; i < m_vecState.size(); ++i)
		{
			MachineState* pState = m_vecState[i].get();

			if (pState->m_strName == pszName)
				return pState;
		}

		return NULL;
	}

	void StateMachineNode::AdvanceTime(float fTimeElepse)
	{
		ASSERT(m_curState);
		if (m_curState == NULL)
			return;

		if (m_preState)
		{
			m_preState->m_pAnimNode->AdvanceTime(fTimeElepse);
		}

		m_curState->m_pAnimNode->AdvanceTime(fTimeElepse);
	}

	void StateMachineNode::EvaluateAnimation(AnimationNodeOutput &output, float fWeight)
	{
		ASSERT(m_curState);
		if (m_curState == NULL)
			return;

// 		AnimationNodeOutputPool *pool = m_animator->GetAnimationNodeOutputPool();
// 		AnimationNodeOutput &childOutput = pool->get();
// 		childOutput.Init(this->GetBoneCount());

// 		if (m_preState)
// 		{
// 			m_preState->m_childState->EvaluateAnimation(output, pSpecificPhase, fWeight, pBoneSet);
// 		}

		m_curState->m_pAnimNode->EvaluateAnimation(output, fWeight);
	}

	void StateMachineNode::HandleEvent(MachineTransition *trans)
	{
		if (trans->Check())
		{
			TransitTo(trans->GetEndState(), 0/*trans->m_duration*/);
		}
	}

	void StateMachineNode::TransitTo(MachineState* state, float duration)
	{
		m_curState->Leave();
		m_preState = m_curState;
		m_curState = state;
		m_curState->Enter();

		for (uint32_t i = 0; i < m_curState->m_trans.size(); ++i)
		{
			MachineTransition *trans = m_curState->m_trans[i].get();
			HandleEvent(trans);
		}
	}

	bool StateMachineNode::SetStartStateId(const char* pszName)
	{
		m_startStateId = pszName;

		return true;
	}

	bool StateMachineNode::Import(rapidxml::xml_node<>* xmlNode)
	{
		AnimTreeNode::Import(xmlNode);

		const char* pszStartState = xmlNode->findAttribute("startState");
		ASSERT(pszStartState);
		m_startStateId = pszStartState ? pszStartState : "";

		rapidxml::xml_node<>* pXmlState = xmlNode->first_node("State");
		while (pXmlState)
		{
			MachineState* pState = new MachineState("");

			m_vecState.push_back(pState);

			pState->Import(pXmlState);

			pXmlState = pXmlState->next_sibling("State");
		}

		return true;
	}

	bool StateMachineNode::Export(rapidxml::xml_node<>* xmlNode, rapidxml::xml_document<>& doc)
	{
		AnimTreeNode::Export(xmlNode, doc);

		rapidxml::append_attribute(xmlNode, doc, "startState", m_startStateId.c_str());

		for (uint32_t i = 0; i < m_vecState.size(); ++i)
		{
			rapidxml::xml_node<>* pXmlState = doc.allocate_node(rapidxml::node_element, doc.allocate_string("State"));
			xmlNode->append_node(pXmlState);

			MachineState* pState = m_vecState[i].get();

			pState->Export(pXmlState, doc);
		}

		return true;
	}

	RefPtr<StateMachineNode> CreateStateMachineNode()
	{
		return new StateMachineNode();
	}
}