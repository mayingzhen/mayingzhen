#pragma once

#include "Attribute.h"
#include "Engine/Resource/Resource.h"

class TiXmlElement;

namespace ma
{
	class AttributeManager;
	class XmlFile;
	
	class Serializable : public Object
	{
	public:
		Serializable();
		
		virtual ~Serializable();

		void					SaveToXML(const char* pszXMLFile);
		void					LoadFromXML(const char* pszXMlFile,const RES_CALL_BACK& call_back = nullptr);
		void					LoadFromXMLSync(const char* pszXMlFile);

		bool					IsReady();

		XmlFile*				GetXMLFile() {return m_pXMlFile.get();}

		const char*				GetResPath() { return m_strPath.c_str(); }

	protected:
		std::string				m_strPath;
		RefPtr<XmlFile>			m_pXMlFile;
	};
}

