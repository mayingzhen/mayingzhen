#include "SceneNode.h"


SceneNode::SceneNode(const char* pNodeName):
Object(pNodeName)
{
	
}

SceneNode::~SceneNode()
{

}


const D3DXMATRIX& SceneNode::GetWorldMatrix();

D3DXVECTOR3 SceneNode::GetWorldPos();

void SceneNode::SetWorldPos(const D3DXVECTOR3& pos);

void SyncWorld();
