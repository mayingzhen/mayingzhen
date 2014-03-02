#include "AnimationSetData.h"
#include "ActionData.h"
#include "PoseModifier/PoseModifier.h"

namespace ma
{
	IMPL_OBJECT(AnimationSetData,Resource)

	template<class T>
	void SerializeArrObj(Serializer& sl,std::vector<T*>& arrObject,const char* pszLable)
	{
		sl.BeginSection(pszLable);

		UINT nSize = arrObject.size();
		sl.Serialize(nSize,"size");

		if (nSize != arrObject.size())
		{
			arrObject.resize(nSize);
		}

		for (UINT nCnt = 0;nCnt < nSize; ++nCnt)
		{
			std::string strClassName = T::StaticGetClass()->GetName();
			sl.Serialize(strClassName,"ClassName");

			char buf[32];
			sprintf(&buf[0],"Element_%u",nCnt);
			if (arrObject[nCnt] == NULL)
			{
				ObjectFactoryManager& objFac = ObjectFactoryManager::GetInstance();
				arrObject[nCnt] = SafeCast<T>( objFac.CreateObject(strClassName.c_str()) );
			}
			sl.Serialize(*(arrObject[nCnt]),buf);
		}

		sl.EndSection();
	}


	void AnimationSetData::SaveToFile(const char* pszPath)
	{
		std::string strSavePath = pszPath ? pszPath : m_sResPath;

		XMLOutputArchive ar;
		bool bLoadOK = ar.Open(strSavePath.c_str());
		if (!bLoadOK)
		{
			ASSERT(false && "Fail to save to file");
			return ;
		}

		Serialize(ar);

		ar.Close();
	}

	bool AnimationSetData::CreateFromMemeory()
	{
		XMLInputArchive arIn;
		bool bLoadOk = arIn.Open(m_pDataStream);
		if (!bLoadOk)
		{
			ASSERT(false && "Fail to Load to file");
			return  false;
		}

		Serialize(arIn);

		arIn.Close();

		m_eResState = ResLoaded;

		return true;
	}

	void AnimationSetData::Serialize(Serializer& sl, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_arrBoneSet,"arrBoneSet");

		SerializeArrObj<PoseModifier>(sl,m_arrPoseModifier,"arrPoseModifier");

		sl.Serialize(m_arrActionData,"arrAnimation");

		sl.EndSection();
	}

	void AnimationSetData::AddAnimClip(const char* skaPath,const char* pszActionName)
	{
		AnimClipNodeData* pAnmClip = new AnimClipNodeData();
		pAnmClip->m_sClipPath = "magician/602/bip01.ska";

		ActionData* pAction = new ActionData();
		pAction->m_sAnimName = pszActionName;
		pAction->m_pAnimNodeData = pAnmClip;

		m_arrActionData.push_back(pAction);
	}

}

