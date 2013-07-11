#ifndef  _RESOURCE__H__
#define  _RESOURCE__H__

#include "Engine/DataThread/DataThread.h"

namespace ma
{
	class Serializer;

	class ENGINE_API Resource : public Object
	{
		DECL_OBJECT(Resource)

	public:
		Resource(const char* pszPath = NULL);

		virtual ~Resource();

		virtual bool Load();

		virtual void LoadImp(DataStream* pDataStream) = 0;

		virtual bool Save(const char* pszPath = NULL);

		virtual const char* GetResPath() const {return m_sResPath.c_str();}

		virtual void SetResPath(const char* pResPath) {m_sResPath = pResPath;}

		virtual bool IsLoad() {return m_bLoaded;}

		virtual void Serialize(Serializer& sl, const char* pszLable = "Resource");

		DataStream*	 GetDataStream() {return m_pDataStream;}

	protected:
		std::string		m_sResPath;
		bool			m_bLoaded;

		DataStream*		m_pDataStream;
	};
}


#endif
