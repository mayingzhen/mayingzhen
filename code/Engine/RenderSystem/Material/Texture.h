#ifndef  _IRENDTEXTURE__H__
#define  _IRENDTEXTURE__H__


namespace ma
{
	class ENGINE_API Texture : public Object
	{
		

	public:
		//static Texture* create(const char* path,bool generateMipmaps = false);

		virtual bool CreateRT(int nWidth,int nHeight,FORMAT format = FMT_A8R8G8B8) = 0;

		virtual bool Load(const char* pszPath = NULL, bool generateMipmaps = false) = 0;

		virtual bool Load(DataStream* pDataStream, bool generateMipmaps = false) = 0;

		//virtual bool Load(Stream* pStream, bool generateMipmaps = false) = 0;

		virtual bool LoadFromData(FORMAT format,UINT width,UINT height,Uint8* data,UINT size, bool generateMipmaps = false) = 0;

		int getWidth() {return m_nWidth;}

		int getHeight() {return m_nHeight;}

	protected:
		int				m_nWidth;
		int				m_nHeight;
		int				m_nMipLevels;
		USAGE			m_eUsage;
		FORMAT			m_eFormat;
		TEXTURE_TYPE	m_eType;
	};


	/**
     * Defines the set of supported texture formats.
     */
//     enum Format
//     {
//         UNKNOWN = 0,
//         RGB    /* = GL_RGB*/,
//         RGBA    /*= GL_RGBA*/,
//         ALPHA   /*= GL_ALPHA*/
//     };

    /**
     * Defines the set of supported texture filters.
     */
//     enum Filter
//     {
//         NEAREST /*= GL_NEAREST*/,
//         LINEAR /*= GL_LINEAR*/,
//         NEAREST_MIPMAP_NEAREST /*= GL_NEAREST_MIPMAP_NEAREST*/,
//         LINEAR_MIPMAP_NEAREST/* = GL_LINEAR_MIPMAP_NEAREST*/,
//         NEAREST_MIPMAP_LINEAR /*= GL_NEAREST_MIPMAP_LINEAR*/,
//         LINEAR_MIPMAP_LINEAR /*= GL_LINEAR_MIPMAP_LINEAR*/
//     };


		    /**
     * Defines a texture sampler.
     *
     * A texture sampler is basically an instance of a texture that can be
     * used to sample a texture from a material. In addition to the texture
     * itself, a sampler stores per-instance texture state information, such
     * as wrap and filter modes.
     */
    class ENGINE_API Sampler /*: public RefCountObject*/
    {
        friend class Texture;

    public:

		enum FilterOptions
		{
			/// Equal to: min=FO_POINT, mag=FO_POINT, mip=FO_NONE
			TFO_POINT,
			/// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_POINT
			TFO_BILINEAR,
			/// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_LINEAR
			TFO_TRILINEAR,
			/// Equal to: min=FO_ANISOTROPIC, max=FO_ANISOTROPIC, mip=FO_LINEAR
			TFO_ANISOTROPIC
		};

		/**
		 * Defines the set of supported texture wrapping modes.
		 */
		enum Wrap
		{
			REPEAT /*= GL_REPEAT*/,
			CLAMP /*= GL_CLAMP_TO_EDGE*/
		};


        /**
         * Destructor.
         */
        virtual ~Sampler();

        /**
         * Creates a sampler for the specified texture.
         *
         * @param texture The texture.
         *
         * @return The new sampler.
         * @script{create}
         */
        static Sampler* create(Texture* texture);

        /**
         * Creates a sampler for the specified texture.
         *
         * @param path Path to the texture to create a sampler for.
         * @param generateMipmaps True to force a full mipmap chain to be generated for the texture, false otherwise.
         *
         * @return The new sampler.
         * @script{create}
         */
        static Sampler* create(const char* path, bool generateMipmaps = false);

        /**
         * Sets the wrap mode for this sampler.
         *
         * @param wrapS The horizontal wrap mode.
         * @param wrapT The vertical wrap mode.
         */
        void setWrapMode(Wrap wrapS, Wrap wrapT);

        /**
         * Sets the texture filter modes for this sampler.
         *
         * @param minificationFilter The texture minification filter.
         * @param magnificationFilter The texture magnification filter.
         */
        void setFilterMode(FilterOptions ficationFilter);

        /**
         * Gets the texture for this sampler.
         *
         * @return The texture for this sampler.
         */
        Texture* getTexture() const;

        /**
         * Binds the texture of this sampler to the renderer and applies the sampler state.
         */
        //void Bind();

    //private:
	public:

        /**
         * Constructor.
         */
        Sampler(Texture* texture);

        /**
         * Hidden copy assignment operator.
         */
        Sampler& operator=(const Sampler&);

        Texture* _texture;
        Wrap _wrapS;
        Wrap _wrapT;
        //Filter _minFilter;
        //Filter _magFilter;
		FilterOptions _filter;
    };


}

#endif
