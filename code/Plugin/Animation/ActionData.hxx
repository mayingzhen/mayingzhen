#include "ActionData.h"

namespace ma
{
	IMPL_OBJECT(AnimNodeData,Object)
	IMPL_OBJECT(AnimClipNodeData,AnimNodeData)	
	IMPL_OBJECT(AnimLayerNodeData,AnimNodeData)
	IMPL_OBJECT(AnimBlendNodData,AnimNodeData)

	template<class T>
	void SerializeObject(Serializer& sl, T* &pObject, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		std::string strClassName = pObject ? pObject->GetClass()->GetName() : "";
		sl.Serialize(strClassName,"ClassName");

		if (pObject == NULL)
		{
			ObjectFactoryManager& objFac = ObjectFactoryManager::GetInstance();
			pObject = SafeCast<T>( objFac.CreateObject(strClassName.c_str()) );
		}
		sl.Serialize(*(pObject),pszLable);

		sl.EndSection();
	}


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

		SerializeArrObj(sl,m_arrAnimNodeData,"arrAnimNode");

		sl.EndSection();
	}

	void AnimClipNodeData::Serialize(Serializer& sl, const char* pszLable /*= "AnimClipNode" */)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_sClipPath,"ClipName");
		sl.Serialize(m_sBoneSetName,"BonsetName");

		sl.EndSection();
	}



	void ActionData::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_sAnimName,"ActionName");

		SerializeObject<AnimNodeData>(sl,m_pAnimNodeData,"AnimationNode");

		sl.EndSection();
	}
}
