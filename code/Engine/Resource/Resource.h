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
		ResInited,
		ResLoadError,
	};

	enum CreateType
	{
		XML_RES,
		BIN_RES,
		MEM_RES, 
	};

	class Serializer;
	class MemoryStream;

	class Resource : public Object
	{
		DECL_OBJECT(Resource)

	public:
		Resource(const char* pszPath = NULL);

		virtual ~Resource();

		virtual	bool	LoadFromXML(const char* pFilePath);
		virtual bool	SaveToXML(const char* pFilePath);

		virtual	bool	LoadFromFile(const char* pFilePath);
		virtual bool	SaveToFile(const char* pszPath);

		virtual	bool	Load();
		virtual void	LoadSync();

		virtual bool	OnLoadOver();

		const char*		GetResPath() const {return m_sResPath.c_str();}
		void			SetResPath(const char* pResPath) {m_sResPath = pResPath;}

		ResState		GetResState() const {return m_eResState;}

		MemoryStream*	GetDataStream() {return m_pDataStream.get();}

	protected:
		virtual void	Serialize(Serializer& sl, const char* pszLable = "Resource");

		virtual bool	LoadFileToMemeory();

		virtual bool	CreateFromMemeory();

	protected:
		std::string				m_sResPath;
		ResState				m_eResState;
		RefPtr<MemoryStream>	m_pDataStream;	
		CreateType				m_eCreateType;

		friend class DataThread;
	};

	RefPtr<Resource> CreateResource(const char* pszPath);
}


#endif
