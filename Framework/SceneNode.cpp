#include "SceneNode.h"


SceneNode::SceneNode()
{

}

SceneNode::~SceneNode()
{

}

void SceneNode::Render()
{
	for(UINT i = 0; i < m_vChildNodes.size(); ++i)
	{
		m_vChildNodes[i].Render();
	}
}


void SceneNode::Update()
{
	for(UINT i = 0; i < m_vChildNodes.size(); ++i)
	{
		m_vChildNodes[i].Update();
	}
}

void SceneNode::AddChildNode(SceneNode* pSceneNode)
{
	if (pSceneNode == NULL)
		return;

	std::vector<SceneNode*>::iterator it = std::find(m_vChildNodes.begin(),m_vChildNodes.end(),pSceneNode);
	if (it != m_vChildNodes.end())
		return;

	m_vChildNodes.push_back(pSceneNode);
}

