#include "Texture.h"

#include "il/il.h"

namespace ma
{
	Sampler::Sampler(Texture* texture)
		: _texture(texture), _wrapS(/*Texture::*/REPEAT), _wrapT(/*Texture::*/REPEAT),
		_filter(TFO_BILINEAR)
	{
		ASSERT(texture);
		//_minFilter = texture->_minFilter;
		//_magFilter = texture->_magFilter;
	}

	Sampler::~Sampler()
	{
		//SAFE_RELEASE(_texture);
	}

	Sampler* Sampler::create(Texture* texture)
	{
		ASSERT(texture);
		//texture->IncReference();
		return new Sampler(texture);
	}

	Sampler* Sampler::create(const char* path, bool generateMipmaps)
	{
		return NULL;
		//Texture* texture = Texture::create(path, generateMipmaps);
		//return texture ? new Sampler(texture) : NULL;
	}

	void Sampler::setWrapMode(Wrap wrapS, Wrap wrapT)
	{
		_wrapS = wrapS;
		_wrapT = wrapT;
	}

	void Sampler::setFilterMode(FilterOptions ficationFilter)
	{
		_filter = ficationFilter;
	}

	Texture* Sampler::getTexture() const
	{
		return _texture;
	}

//	void Sampler::Bind()
//	{
//		ASSERT(_texture);
//
//		if (_texture == NULL)
//			return;

		//GetRenderDevice()->ActiveSampler(this);

// 		GL_ASSERT( glBindTexture(GL_TEXTURE_2D, _texture->_handle) );
// 
// 		if (_texture->_minFilter != _minFilter)
// 		{
// 			_texture->_minFilter = _minFilter;
// 			GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)_minFilter) );
// 		}
// 
// 		if (_texture->_magFilter != _magFilter)
// 		{
// 			_texture->_magFilter = _magFilter;
// 			GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)_magFilter) );
// 		}
// 
// 		if (_texture->_wrapS != _wrapS)
// 		{
// 			_texture->_wrapS = _wrapS;
// 			GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)_wrapS) );
// 		}
// 
// 		if (_texture->_wrapT != _wrapT)
// 		{
// 			_texture->_wrapT = _wrapT;
// 			GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)_wrapT) );
// 		}
//	}
}