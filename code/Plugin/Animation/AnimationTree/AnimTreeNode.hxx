#include "AnimTreeNode.h"

namespace ma
{
	void AnimTreeNode::RegisterAttribute()
	{
		ACCESSOR_ATTRIBUTE(AnimTreeNode, "AnimNodeName", GetName, SetName, const char*, NULL, AM_DEFAULT);
	}
}