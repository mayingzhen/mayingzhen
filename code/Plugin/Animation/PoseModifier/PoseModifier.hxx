#include "PoseModifier.h"

namespace ma
{
	IMPL_OBJECT(PoseModifier,Object)

	PoseModifier::PoseModifier() 
	{
		m_bEnable = true;
		m_fGain =1.0f;
	}

	PoseModifier::~PoseModifier() 
	{

	}

	void	PoseModifier::UpdatePose(SkeletonPose* pNodePose)
	{

	}

	void	PoseModifier::Serialize(Serializer& sl, const char* pszLable)
	{

	}
}