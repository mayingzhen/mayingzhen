#ifndef  _NODESET__H__
#define  _NODESET__H__

namespace ma
{
	class NodeSet
	{
		NodeSet();

		void Init(maGUID nSkelGUID,const std::vector<std::string>& arrNodeName);

		void Init(maGUID nSkelGUID,UINT nNodeNum);

		void SetNodeNameByIndex(UINT nNodeInd,const char* pszNodeName);

		xmUint64 GetNodeGUID() const;

		UINT GetNodeNumber() const;

		const char* GetNodeNameByIndex(UINT nBondeIndex) const;

		BoneIndex GetNodeIndexByName(const char* pszBoneName) const;

		bool IsNodeNameEqual(const std::vector<std::string>& arrNodeName) const;

	private:
		void UpdateNodeGUID() const;

	private:
		maGUID					 m_nSkelGUID;
		std::vector<std::string> m_arrNodeName;

		mutable bool			 m_bNodeGUIDDirty;
		mutable xmUint64		 m_nNodeGUID;	
	}


}




#endif