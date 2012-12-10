#ifndef  _BONE_SET__H__
#define  _BONE_SET__H__

namespace ma
{

	class ANIMATION_API BoneSet
	{
	public:
		BoneSet(const char* pszName){m_sName = pszName ? pszName : "";}
	
		~BoneSet();

		UINT GetBoneNumber() {return m_arrBone.size();}

		BoneIndex GetBoneIdByIndex(int index) {return m_arrBone[index];}

		const char* GetBoneSetName() {return m_sName.c_str();}
		
		void AddBoneInd(BoneIndex uBoneID) {m_arrBone.push_back(uBoneID);}

		void RemoveBoneInd(BoneIndex uBoneID) 
		{
			for (UINT i = 0; i<m_arrBone.size(); ++i)
			{
				if (m_arrBone[i] == uBoneID)
				{
					m_arrBone.erase(m_arrBone.begin() + i);
					return;
				}
			}
		}
		
		void SetBoneSetName(const char* pszName) {m_sName = pszName ? pszName : "";}

	private:
		std::string m_sName;

		std::vector<BoneIndex> m_arrBone;
	};
}

#endif