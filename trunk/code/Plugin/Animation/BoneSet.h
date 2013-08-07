#ifndef  _BONE_SET__H__
#define  _BONE_SET__H__

namespace ma
{
	class BoneSet
	{
	public:
		BoneSet(const char* pszName = NULL){m_sName = pszName ? pszName : "";}
	
		UINT			GetBoneNumber() {return m_arrBone.size();}

		BoneIndex		GetBoneIdByIndex(int index) {return m_arrBone[index];}

		void			AddBoneInd(BoneIndex uBoneID) {m_arrBone.push_back(uBoneID);}

		void			RemoveBoneInd(BoneIndex uBoneID); 

		const char*		GetBoneSetName() {return m_sName.c_str();}

		void			SetBoneSetName(const char* pszName) {m_sName = pszName ? pszName : "";}

		virtual void	Serialize(Serializer& sl, const char* pszLable = "BoneSet");

	private:
		std::string m_sName;

		std::vector<BoneIndex> m_arrBone;
	};
}

#endif