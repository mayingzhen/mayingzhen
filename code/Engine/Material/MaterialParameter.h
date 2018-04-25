#pragma once


namespace ma
{

	class Parameter 
	{

	public:
		Parameter();

		~Parameter();

		const char* GetName() const;
		void		SetName(const char* pName);

		void		SetValue(const Any& value);
		const Any&	GetValue() const;

		void		Import(rapidxml::xml_node<>* pXmlElem,Resource* pParent);
		void		Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);		

	private:
		Any				m_anyValue;

		std::string		m_sName;
	};
}
