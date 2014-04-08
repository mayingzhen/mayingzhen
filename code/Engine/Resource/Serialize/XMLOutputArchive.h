#ifndef __XMLOutputArchive_H__
#define __XMLOutputArchive_H__

#include "Engine/Resource/Serialize/Serializer.h"

class TiXmlDocument;
class TiXmlElement;
class IAllocator;

namespace ma
{
	class ENGINE_API XMLOutputArchive : public Serializer 
	{
	public:
		XMLOutputArchive();

		~XMLOutputArchive();

		void Release();

		bool IsReading() const;

 		bool Open(const char* pszFilename);
 
 		void Close();

		void Save(const char* pszFileName);

		TiXmlElement*	GetRootElement() {return m_pRootElem;}

		void BeginSection(const char* pszLable);

		void EndSection();

		void Serialize(bool& val,const char* pszLable = "bool");

		void Serialize(unsigned char& val, const char* pszLabel = "unsigned char");

		void Serialize(char& val,const char* pszLable);

		void Serialize(short& val,const char* pszLable);

		void Serialize(unsigned short& val, const char* pszLabel);

		void Serialize(unsigned int&val,const char* pszLable = "unsigned int");

		void Serialize(int&val,const char* pszLable = "int");

		void Serialize(unsigned long &val,const char* pszLable = "unsigned long");

		void Serialize(long &val,const char* pszLable = "long");

		void Serialize(Uint64&val,const char* pszLable = "Uint64");

		void Serialize(float& val,const char* pszLable = "float");

		void Serialize(std::string& val,const char* pszLable = "string");

	private:
		std::string		m_strFilename;

		TiXmlElement*	m_pCurElem;

		TiXmlElement*	m_pRootElem;
	};
}



#endif// __XMLOutputArchive_H__