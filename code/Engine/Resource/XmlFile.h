#ifndef  _XmlFile__H__
#define  _XmlFile__H__

#include "Resource.h"

#include "Serialize/tinyxml/tinyxml.h"

namespace ma
{
	class XmlFile : public Resource
	{
	public:
		XmlFile();

		TiXmlDocument& GetDoc() {return m_doc;}
			
	protected:
		TiXmlDocument	m_doc;
	};
}

#endif