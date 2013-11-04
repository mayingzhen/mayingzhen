#include "SamplerState.h"


namespace ma
{
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
		return m_pTexture;
	}

	void SamplerState::SetTexture(const char* pTexPath)
	{
		Texture* pTexture = LoadResourceASync<Texture>(pTexPath);
		SetTexture(pTexture);
	}

	void SamplerState::SetTexture(Texture* pTextute)
	{
		m_pTexture = pTextute;
	}

	void SamplerState::Serialize(Serializer& sl, const char* pszLable/* = "SamplerState"*/)
	{
		sl.BeginSection(pszLable);
	
		if (sl.IsReading())
		{
			std::string sTexurePath;
			sl.Serialize(sTexurePath,"Texture");

			SetTexture(sTexurePath.c_str());
		}
		else
		{
			std::string sTexurePath = m_pTexture->GetResPath();
			sl.Serialize(sTexurePath,"Texture");
		}

		//sl.Serialize(m_eFilter);
		//sl.Serialize(m_eWrap);

		sl.EndSection();
	}
}
