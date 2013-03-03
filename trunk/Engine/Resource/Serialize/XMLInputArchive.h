#ifndef __XMLInputArchive_H__
#define __XMLInputArchive_H__

#include "Engine/Resource/Serialize/SerializeListener.h"

class TiXmlDocument;
class TiXmlElement;

namespace ma
{

	class ENGINE_API XMLInputArchive : public SerializeListener 
	{
		std::string		m_strFilename;
		TiXmlDocument*	m_pDoc;
		TiXmlElement*	m_pCurElem;
		xmInt				m_nullChild;

	public:
		XMLInputArchive();

		~XMLInputArchive();

		void Release();

		bool IsReading() const;

		bool Open(const char* pszFilename);

		bool OpenFromMemory(const char* pszData);

		void Close();

		TiXmlDocument* GetXMLDocument();

		void BeginSection(const char* pszLable);

		void EndSection();

		void Serialize(bool& val,const char* pszLable = "bool");

		void Serialize(unsigned char& val,const char* pszLable);

		void Serialize(short& val,const char* pszLable);

		void Serialize(unsigned short& val, const char* pszLabel);

		void Serialize(unsigned int&val,const char* pszLable = "unsigned int");

		void Serialize(int&val,const char* pszLable = "int");

		void Serialize(unsigned long &val,const char* pszLable = "unsigned long");

		void Serialize(long &val,const char* pszLable = "long");


		void Serialize(xmUint64&val,const char* pszLable = "xmUint64");

		void Serialize(float& val,const char* pszLable = "float");

		void Serialize(std::string& val,const char* pszLable = "string");

		//void SerializeBasicType(IStringWrapper* val,const char* pszLable);


		SerializeListener* GetSerializeListener();

	private:
		void NextSiblingElement();
	};

}


#endif// __XMLInputArchive_H__