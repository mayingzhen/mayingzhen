#include "Texture.h"



namespace ma
{
	IMPL_OBJECT(Texture,Resource);

	bool Texture::CreateFromMemeory()
	{
		if (m_eResState == ResLoaded)
			return true;

		ASSERT(m_pDataStream);
		if (m_pDataStream == NULL)
			return false;

		Load(m_pDataStream);

		m_eResState = ResLoaded;

		return true;
	}

	Sampler::Sampler(Texture* texture)
		: m_pTexture(texture), m_eWrap(CLAMP),
		m_eFilter(TFO_BILINEAR)
	{
		ASSERT(texture);
	}

	Sampler::~Sampler()
	{
		//SAFE_RELEASE(_texture);
	}

	Sampler* Sampler::create(Texture* texture)
	{
		ASSERT(texture);
		return new Sampler(texture);
	}

	Sampler* Sampler::create(const char* path, bool generateMipmaps)
	{
		return NULL;
		//Texture* texture = Texture::create(path, generateMipmaps);
		//return texture ? new Sampler(texture) : NULL;
	}

	void Sampler::setWrapMode(Wrap eWrap)
	{
		m_eWrap = eWrap;
	}

	void Sampler::setFilterMode(FilterOptions ficationFilter)
	{
		m_eFilter = ficationFilter;
	}

	Texture* Sampler::getTexture() const
	{
		return m_pTexture;
	}
}