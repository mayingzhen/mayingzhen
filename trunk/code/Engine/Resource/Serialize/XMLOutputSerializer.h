#ifndef __XMLOutputSerializer_H__
#define __XMLOutputSerializer_H__

#include "XMLSerializer.h"

class TiXmlDocument;
class TiXmlElement;
class IAllocator;

namespace ma
{
	class ENGINE_API XMLOutputSerializer : public XMLSerializer 
	{
	public:
		XMLOutputSerializer();

		~XMLOutputSerializer();

		void Release();

		bool IsReading() const;

 		bool Open(const char* pszFilename);
 
 		void Close();

		void Save(const char* pszFileName);

		TiXmlElement*	GetRootElement() {return m_pRootElem;}

		void BeginSection(const char* pszLable);

		void EndSection();

	private:
		std::string		m_strFilename;

		TiXmlElement*	m_pRootElem;
	};
}



#endif// __XMLOutputSerializer_H__