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

	class Serializer;
	class MemoryStream;

	class Resource : public Referenced
	{

	public:
		Resource();

		virtual ~Resource();

		virtual	bool	Load(const char* pszFile);
		virtual void	LoadSync(const char* pszFile);

		virtual bool	SaveToFile(const char* pszPath);

		virtual bool	IsReady();

		const char*		GetResPath() const {return m_sResPath.c_str();}
		void			SetResPath(const char* pResPath) {m_sResPath = pResPath;}

		ResState		GetResState() const {return m_eResState;}

		MemoryStream*	GetDataStream() {return m_pDataStream.get();}

	protected:
		virtual bool	LoadFileToMemeory();

		virtual bool	CreateFromMemeory();

		virtual bool    InitRes() {return true;}

	protected:
		std::string				m_sResPath;
		ResState				m_eResState;
		RefPtr<MemoryStream>	m_pDataStream;	

		friend class DataThread;
	};

	RefPtr<Resource> CreateResource(const char* pszPath);
}


#endif
