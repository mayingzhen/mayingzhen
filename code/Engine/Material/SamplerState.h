#ifndef  _SamplerState__H__
#define  _SamplerState__H__


namespace ma
{
	class RENDER_API SamplerState : public Referenced 
    {
    public:
		SamplerState();

        void					SetWrapMode(Wrap eWrap);

		Wrap					GetWrapMode() const {return m_eWrap;}

        void					SetFilterMode(FilterOptions ficationFilter);

		FilterOptions			GetFilterMode() const {return m_eFilter;} 

		void					SetTexture(RefPtr<Texture> pTextute);

		Texture*				GetTexture() const;

		void					SetTexturePath(const char* pTexPath);

		const char*				GetTexturePath() const;

		virtual void			Serialize(Serializer& sl, const char* pszLable = "SamplerState");


    //private:
        RefPtr<Texture>			m_pTexture;
       
		Wrap					m_eWrap;

		FilterOptions			m_eFilter;		
    };

	DeclareRefPtr(SamplerState);

	RefPtr<SamplerState> CreateSamplerState();

}

#endif // _SamplerState__H__

