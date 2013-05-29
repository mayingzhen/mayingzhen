#ifndef  _RESOURCE__H__
#define  _RESOURCE__H__

#include "Common/RTTI/Object.h"

namespace ma
{
	class SerializeListener;

	class ENGINE_API Resource : public Object
	{
	public:
		Resource(const char* pszPath = NULL);

		virtual ~Resource();

		virtual bool Load(const char* pszPath = NULL);

		virtual bool Save(const char* pszPath = NULL);

		virtual const char* GetResPath() const {return m_sResPath.c_str();}

		virtual void SetResPath(const char* pResPath) {m_sResPath = pResPath;}

		virtual bool IsLoad() {return m_bLoaded;}

		virtual void Serialize(SerializeListener& sl, const char* pszLable = "Resource");

	protected:
		std::string m_sResPath;
		bool		m_bLoaded;
	};
}


#endif
