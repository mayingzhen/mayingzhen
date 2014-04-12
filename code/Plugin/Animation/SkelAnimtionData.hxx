#include "SkelAnimtionData.h"

namespace ma
{
	IMPL_OBJECT(AnimNodeData,Object)
	IMPL_OBJECT(AnimClipNodeData,AnimNodeData)	
	IMPL_OBJECT(AnimLayerNodeData,AnimNodeData)
	IMPL_OBJECT(AnimBlendNodData,AnimNodeData)

	void AnimNodeData::Serialize(Serializer& sl, const char* pszLable/* = "AnimNode" */)
	{
		ASSERT(false);
	}

	void AnimBlendNodData::Serialize(Serializer& sl, const char* pszLable/* = "AnimBlendNode" */)
	{
		sl.BeginSection(pszLable);
		
		SerializeObject<AnimNodeData>(sl,m_pSrcAnimNodeData,"SrcAnimNode");
		SerializeObject<AnimNodeData>(sl,m_pSrcAnimNodeData,"DestAnimNode");
		sl.Serialize(m_fWeight,"Weight");

		sl.EndSection();
	}

	void AnimLayerNodeData::Serialize(Serializer& sl, const char* pszLable/* = "AnimLayerNode" */)
	{
		sl.BeginSection(pszLable);

		SerializeArrObj<AnimNodeData>(sl,m_arrAnimNodeData,"arrAnimNode");

		sl.EndSection();
	}

	void AnimClipNodeData::Serialize(Serializer& sl, const char* pszLable /*= "AnimClipNode" */)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_sClipPath,"ClipName");
		sl.Serialize(m_sBoneSetName,"BonsetName");

		sl.EndSection();
	}



	void SkelAnimData::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_sAnimName,"ActionName");

		SerializeObject<AnimNodeData>(sl,m_pAnimNodeData,"AnimationNode");

		sl.EndSection();
	}
}
