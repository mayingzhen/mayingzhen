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
		XmlFile(const char* pszPath);

		~XmlFile();

		TiXmlDocument& GetDoc() {return m_doc;}

		virtual bool	CreateFromMemeory();
			
	protected:
		TiXmlDocument	m_doc;
	};

	RefPtr<XmlFile> CreateXmlFile(const char* pszFile);
}

#endif