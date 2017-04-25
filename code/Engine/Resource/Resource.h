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
		ResReady,
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
		void			SetResState(ResState eState) {m_eResState = eState;}

		void			AddRes(Resource* pRes);

		MemoryStream*	GetDataStream() {return m_pDataStream.get();}

	protected:
		virtual bool    InitRes() {return true;}
		virtual bool	ChildResFinish(){return true;}

	private:
		bool			LoadFileToMemeory();
		bool			CreateFromMemeory();

	protected:
		std::string				m_sResPath;
		volatile ResState		m_eResState;
		RefPtr<MemoryStream>	m_pDataStream;	

		typedef list< RefPtr<Resource> > LST_RESOURCE;
		LST_RESOURCE			m_lstChild;

		friend class DataThread;

	public:
		Signal< void() >		mLoadOverEvent;
	};

	RefPtr<Resource> CreateResource(const char* pszPath);
}


#endif
