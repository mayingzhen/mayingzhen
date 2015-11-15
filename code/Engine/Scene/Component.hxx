#include "Component.h"

namespace ma
{
	Component::Component()
	{
		m_pSceneNode = NULL;
	}

	Component::~Component()
	{

	}

	void Component::SetSceneNode(SceneNode* pSceneNode)
	{
		m_pSceneNode = pSceneNode;

		if (m_pSceneNode != NULL)
		{
			OnAddToSceneNode(pSceneNode);
		}
		else
		{
			OnRemoveFromSceneNode(pSceneNode);
		}
	}

	SceneNode* Component::GetSceneNode() const
	{
		return m_pSceneNode;
	}


}

