#ifndef  _SamplerState__H__
#define  _SamplerState__H__


namespace ma
{
    class ENGINE_API SamplerState 
    {
    public:
		SamplerState();

        void			SetWrapMode(Wrap eWrap);

		Wrap			GetWrapMode() const {return m_eWrap;}

        void			SetFilterMode(FilterOptions ficationFilter);

		FilterOptions	GetFilterMode() const {return m_eFilter;} 

		void			SetTexture(const char* pTexPath);

		void			SetTexture(Texture* pTextute);

        Texture*		GetTexture() const;

		virtual void	Serialize(Serializer& sl, const char* pszLable = "SamplerState");


    private:
        Texture*		m_pTexture;
       
		Wrap			m_eWrap;

		FilterOptions	m_eFilter;
    };


}

#endif // _SamplerState__H__
