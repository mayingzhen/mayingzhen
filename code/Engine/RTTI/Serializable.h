
#ifndef __Serializable_H__
#define __Serializable_H__

#include "Attribute.h"
#include "Engine/Resource/Resource.h"

class TiXmlElement;

namespace ma
{
	class AttributeManager;
	class Serializer;
	class XmlFile;
	
	class Serializable : public Object
	{
	public:
		Serializable();
		
		virtual ~Serializable();

		void					SaveToXML(const char* pszXMLFile);
		void					LoadFromXML(const char* pszXMlFile);

		bool					IsReady();

		XmlFile*				GetXMLFile() {return m_pXMlFile.get();}
	protected:
		RefPtr<XmlFile>			m_pXMlFile;
		bool					m_bLoadOver;
	};
}

#endif
