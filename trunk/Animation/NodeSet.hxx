#include "Animation/NodeSet.h"

namespace S3ASkelUtil
{
	xmUint64 CalculateNodeGUID(const std::vector<std::string>& arrNodeName,maGUID nSkelGUID)
	{

	}
}

namespace ma
{
	NodeSet::NodeSet()
		:m_bNodeGUIDDirty(true)
		,m_nNodeGUID(0)
	{
		m_nSkelGUID.Clear();
	}

	void NodeSet::Init(maGUID nSkelGUID,const std::vector<std::string>& arrBoneName)
	{
		m_nSkelGUID = nSkelGUID;
		m_arrNodeName = arrBoneName;
		m_bNodeGUIDDirty = true;

		UpdateNodeGUID();
	}

	void			NodeSet::Init(maGUID nSkelGUID,xmUint nNodeNum)
	{
		m_nSkelGUID = nSkelGUID;
		m_arrNodeName.resize(nNodeNum);
		m_bNodeGUIDDirty = true;
	}

	void			NodeSet::SetNodeNameByIndex(xmUint nNodeInd,const char* pszNodeName)
	{
		m_arrNodeName[nNodeInd] = pszNodeName;
		m_bNodeGUIDDirty = true;
	}

	xmUint64 NodeSet::GetNodeGUID() const
	{
		if (m_bNodeGUIDDirty)
		{
			UpdateNodeGUID();
		}
		return m_nNodeGUID;
	}

	xmUint NodeSet::GetNodeNumber() const
	{
		return m_arrNodeName.size();
	}

	const char* NodeSet::GetNodeNameByIndex(xmUint nBoneIndex) const
	{
		return m_arrNodeName[nBoneIndex].c_str();
	}

	BoneIndex NodeSet::GetNodeIndexByName(const char* pszBoneName) const
	{
		BoneIndex nBoneIndex = InvalidID<BoneIndex>();
		for (xmUint nBoneCnt = 0; nBoneCnt < m_arrNodeName.size(); ++nBoneCnt)
		{
			if (_stricmp(pszBoneName,m_arrNodeName[nBoneCnt].c_str()) == 0)
			{
				nBoneIndex = nBoneCnt;
				break;
			}
		}

		return nBoneIndex;
	}

	bool NodeSet::IsNodeNameEqual(const std::vector<std::string>& arrBoneName) const
	{
		bool bEqual = m_arrNodeName.size() == arrBoneName.size();
		if (bEqual)
		{
			for (xmUint nBoneCnt = 0; nBoneCnt < m_arrNodeName.size(); ++nBoneCnt)
			{
				if (_stricmp(m_arrNodeName[nBoneCnt].c_str(),arrBoneName[nBoneCnt].c_str()) != 0)
				{
					bEqual = false;
					break;
				}
			}
		}
		return bEqual;
	}

	void NodeSet::UpdateNodeGUID() const
	{
		m_nNodeGUID = S3ASkelUtil::CalculateNodeGUID(m_arrNodeName,m_nSkelGUID);
		m_bNodeGUIDDirty = false;
	}
}


