#ifndef __XMLInputSerializer_H__
#define __XMLInputSerializer_H__

#include "XMLSerializer.h"

class TiXmlDocument;
class TiXmlElement;

namespace ma
{
	class XMLOutputSerializer;
	class Stream;

	class ENGINE_API XMLInputSerializer : public XMLSerializer 
	{
	public:
		XMLInputSerializer();

		~XMLInputSerializer();

		bool IsReading() const;

		bool Open(XMLOutputSerializer& xmlOutAr);

		bool Open(MemoryStream* pDataStream);

		bool Close();

		void BeginSection(const char* pszLable);

		void EndSection();

	private:
		TiXmlDocument*	m_pDoc;

		TiXmlElement*	m_pRootElem;

		TiXmlAttribute* m_pCurAttri;

	};


}


#endif// __XMLInputSerializer_H__