#ifndef __XMLInputArchive_H__
#define __XMLInputArchive_H__

#include "Engine/Resource/Serialize/Serializer.h"

class TiXmlDocument;
class TiXmlElement;

namespace ma
{
	class XMLOutputArchive;

	class ENGINE_API XMLInputArchive : public Serializer 
	{
		TiXmlDocument*	m_pDoc;

		TiXmlElement*	m_pRootElem;
		
		TiXmlElement*	m_pCurElem;

	public:
		XMLInputArchive();

		~XMLInputArchive();

		bool IsReading() const;

		bool Open(const char* pszFilename);

		bool Open(XMLOutputArchive& xmlOutAr);

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

		void Serialize(Uint64&val,const char* pszLable = "Uint64");

		void Serialize(float& val,const char* pszLable = "float");

		void Serialize(std::string& val,const char* pszLable = "string");

	};
}


#endif// __XMLInputArchive_H__