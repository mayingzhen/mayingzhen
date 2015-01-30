#ifndef _MaterialParameter_H_
#define _MaterialParameter_H_


namespace ma
{

	class Parameter 
	{

	public:
		Parameter();

		~Parameter();

		const char* GetName() const;
		void		SetName(const char* pName);

		void		SetValue(const Any& value) {m_anyValue = value;}
		const Any&	GetValue() const {return m_anyValue;}

		void		Serialize(Serializer& sl, const char* pszLable = "Parameter");

	private:
		Any				m_anyValue;

		std::string		m_sName;
	};
}

#endif
