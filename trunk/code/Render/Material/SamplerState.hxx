#include "SamplerState.h"


namespace ma
{
	static const char* strWrap[] =
	{
		"REPEAT",
		"CLAMP", 
		0
	};

	static const char* strFilterOptions[] =
	{
		"TFO_POINT",
		"TFO_BILINEAR",
		"TFO_TRILINEAR",
		"TFO_ANISOTROPIC",
		0
	};

	SamplerState::SamplerState()
	{
		m_pTexture = NULL;
		m_eWrap = CLAMP;
		m_eFilter = TFO_BILINEAR;
	}

	void SamplerState::SetWrapMode(Wrap eWrap)
	{
		m_eWrap = eWrap;
	}

	void SamplerState::SetFilterMode(FilterOptions ficationFilter)
	{
		m_eFilter = ficationFilter;
	}

 	Texture* SamplerState::GetTexture() const
 	{
 		return m_pTexture.get();
 	}

	const char* SamplerState::GetTexturePath() const
	{
		return m_pTexture ? m_pTexture->GetResPath() : "";
	}

	void SamplerState::SetTexturePath(const char* pTexPath)
	{
		m_pTexture = LoadResourceASync<Texture>(pTexPath,NULL);
	}

	void SamplerState::SetTexture(RefPtr<Texture> pTextute)
	{
		m_pTexture = pTextute;
	}

	void SamplerState::Serialize(Serializer& sl, const char* pszLable/* = "SamplerState"*/)
	{
		sl.BeginSection(pszLable);

 		std::string sTexurePath = GetTexturePath();

 		sl.Serialize(sTexurePath,"TexturePath");
 		sl.Serialize(m_eFilter,strFilterOptions,"Filter");
 		sl.Serialize(m_eWrap,strWrap,"Wrap");

		if ( sl.IsReading() )
		{
			SetTexturePath(sTexurePath.c_str());
		}

		sl.EndSection();
	}
}
