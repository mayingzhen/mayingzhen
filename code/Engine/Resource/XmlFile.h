#ifndef  _XmlFile__H__
#define  _XmlFile__H__

#include "Resource.h"
#include "ResourceSystem.h"

namespace ma
{
	class XmlFile : public Resource
	{
	public:
		XmlFile();

		~XmlFile();

		rapidxml::xml_document<>& GetDoc() {return m_doc;}

		virtual bool	CreateFromMemeory();
			
	protected:
		 rapidxml::xml_document<>	m_doc;
	};

	RefPtr<XmlFile> CreateXmlFile(const char* pszFile);

	extern ResourceSystem<XmlFile>* g_pXmlFileManager;
}

#endif