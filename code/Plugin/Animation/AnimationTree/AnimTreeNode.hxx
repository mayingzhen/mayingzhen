#include "AnimTreeNode.h"

namespace ma
{
	void AnimTreeNode::RegisterAttribute()
	{
		ACCESSOR_ATTRIBUTE(AnimTreeNode, "AnimNodeName", GetName, SetName, const char*, NULL, AM_DEFAULT);
	}

	const char*	AnimTreeNode::GetName() const
	{
		return m_strName.c_str();
	}

	void AnimTreeNode::SetName(const char* pszName)
	{
		m_strName = pszName ? pszName : "";
	}
}