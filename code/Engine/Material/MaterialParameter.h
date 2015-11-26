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

		void		Improt(rapidxml::xml_node<>* pXmlElem);
		void		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);		

	private:
		Any				m_anyValue;

		std::string		m_sName;
	};
}

#endif
