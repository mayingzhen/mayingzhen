
namespace ma
{
	
	enum SkelVersion
	{
		EXP_SKEL_VER_INITIAL = 1,
		EXP_SKEL_VER_CURRENT = EXP_SKEL_VER_INITIAL,
	};

	struct SkeletonHeader
	{
		UINT m_nIden;
		UINT m_nVersion;

		SkeletonHeader()
		{
			m_nIden = 'MAED';
			m_nVersion = EXP_SKEL_VER_CURRENT;
		}
	};


	void SerializeData(Serializer& sl, SkeletonHeader& data, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(data.m_nIden,"Iden");
		sl.Serialize(data.m_nVersion,"Version");
		
		sl.EndSection();
	}

	void SerializeDataV0(Serializer& sl, Skeleton& data, const char* pszLable)
	{	
		sl.BeginSection(pszLable);

		sl.Serialize(data.m_arrBoneName,"BoneName");
		sl.Serialize(data.m_arrParentIndice,"ParentIndice");
		sl.Serialize(data.m_arrTsfOS,"tsfOS");

		sl.EndSection();
	}

}
