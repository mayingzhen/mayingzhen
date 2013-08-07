namespace ma
{
	enum AnimVersion
	{
		EXP_ANIM_VER_INITIAL = 1,

		EXP_ANIM_VER_CURRENT = EXP_ANIM_VER_INITIAL,
	};


	struct AnimationHeader
	{
		UINT m_nIden;
		UINT m_nVersion;

		AnimationHeader()
		{
			m_nIden = 'MAAD';
			m_nVersion = EXP_ANIM_VER_CURRENT;
		}
	};


	void SerializeData(Serializer& sl,AnimationHeader& aniHeader, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(aniHeader.m_nIden,"Iden");
		sl.Serialize(aniHeader.m_nVersion,"Version");

		sl.EndSection();
	}

	void SerializeData(Serializer& sl, Vector3Track& data, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(data.m_arrFrame,"Frame");
		sl.Serialize(data.m_arrValue,"Data");

		sl.EndSection();
	}

	void SerializeData(Serializer& sl, QuaternionTrack& data, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(data.m_arrFrame,"Frame");
		sl.Serialize(data.m_arrValue,"Data");

		sl.EndSection();
	}

	void SerializeDataV0(Serializer& sl, Animation& data, const char* pszLable)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(data.m_arrTransfTrackName,"TransfTrackName");
		sl.Serialize(data.m_nFrameNumber,"FrameNumber");
		sl.Serialize(data.m_arrScaleTrack,"ScalePSTrack");
		sl.Serialize(data.m_arrRotTrack,"RotationPSTrack");
		sl.Serialize(data.m_arrPosTrack,"PositionPSTrack");
	
		sl.EndSection();
	}


}
