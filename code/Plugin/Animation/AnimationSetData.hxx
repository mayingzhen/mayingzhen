#include "AnimationSetData.h"
#include "SkelAnimtionData.h"
#include "PoseModifier/PoseModifier.h"

namespace ma
{
	IMPL_OBJECT(AnimationSetData,Resource)


	AnimationSetData::~AnimationSetData()
	{
		for (UINT i = 0; i < m_arrBoneSet.size(); ++i)
		{
			SAFE_DELETE(m_arrBoneSet[i]);
		}
		m_arrBoneSet.clear();

		for (UINT i = 0; i < m_arrPoseModifier.size(); ++i)
		{
			SAFE_DELETE(m_arrPoseModifier[i]);
		}
		m_arrPoseModifier.clear();

		for (UINT i = 0; i < m_arrActionData.size(); ++i)
		{
			SAFE_DELETE(m_arrActionData[i]);
		}
		m_arrActionData.clear();
	}

	void AnimationSetData::SaveToFile(const char* pszPath)
	{
		std::string strSavePath = pszPath ? pszPath : m_sResPath;

		XMLOutputSerializer ar;
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
		if (m_eResState == ResLoaded)
			return true;

		XMLInputSerializer arIn;
		bool bLoadOk = arIn.Open(m_pDataStream.get());
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
		pAnmClip->m_sClipPath = skaPath;

		SkelAnimData* pActionData = new SkelAnimData();
		pActionData->m_sAnimName = pszActionName;
		pActionData->m_pAnimNodeData = pAnmClip;

		m_arrActionData.push_back(pActionData);
	}

}

