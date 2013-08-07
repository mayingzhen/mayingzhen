#ifndef  _RESOURCE__H__
#define  _RESOURCE__H__

#include "DataThread.h"

namespace ma
{
	enum ResState
	{
		ResUnLoad,
		ResLoadIng,
		ResLoaded,
	};

	class Serializer;

	class ENGINE_API Resource : public Object, IDataObj
	{
		DECL_OBJECT(Resource)

	public:
		Resource(const char* pszPath = NULL);

		virtual ~Resource();

		virtual void	LoadSync();

		virtual void	LoadAsync();

		virtual void	SaveToFile(const char* pszPath);

		const char*		GetResPath() const {return m_sResPath.c_str();}

		void			SetResPath(const char* pResPath) {m_sResPath = pResPath;}

		ResState		GetResState() {return m_eResState;}

		virtual bool	LoadFileToMemeory();

		virtual bool	CreateFromMemeory();

	protected:
		virtual void	Serialize(Serializer& sl, const char* pszLable = "Resource");

		DataStream*		GetDataStream() {return m_pDataStream;}

	protected:
		std::string		m_sResPath;
		ResState		m_eResState;
		DataStream*		m_pDataStream;
	};
}


#endif
