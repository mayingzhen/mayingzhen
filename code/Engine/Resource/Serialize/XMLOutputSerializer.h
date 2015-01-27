#ifndef __XMLOutputSerializer_H__
#define __XMLOutputSerializer_H__

#include "XMLSerializer.h"

class TiXmlDocument;
class TiXmlElement;
class IAllocator;

namespace ma
{
	class XMLOutputSerializer : public XMLSerializer 
	{
	public:
		XMLOutputSerializer();

		~XMLOutputSerializer();

		void Release();

		bool IsReading() const;

		bool Save(const char* pszFileName);

		TiXmlElement*	GetRootElement() {return m_pRootElem;}

		void BeginSection(const char* pszLable);

		void EndSection();

	private:
		TiXmlElement*	m_pRootElem;
	};
}



#endif// __XMLOutputSerializer_H__