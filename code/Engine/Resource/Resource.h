#pragma once

#include "DataThread.h"
#include <functional> 

namespace ma
{
	enum ResState
	{
		ResUnLoad,
		ResLoadIng,
		ResLoadError,
		ResReady,
	};

	class Serializer;
	class MemoryStream;

	typedef std::function<void(Resource*)> RES_CALL_BACK;

	class Resource : public Referenced
	{
	public:
		Resource();

		virtual ~Resource();

		virtual	bool	Load(const char* pszFile);
		virtual bool	LoadSync(const char* pszFile);

		virtual bool	SaveToFile(const char* pszPath);

		virtual bool	IsReady();

		const char*		GetResPath() const {return m_sResPath.c_str();}
		void			SetResPath(const char* pResPath) {m_sResPath = pResPath;}

		ResState		GetResState() const {return m_eResState;}
		void			SetResState(ResState eState) {m_eResState = eState;}

		//void			AddRes(Resource* pRes);

		MemoryStream*	GetDataStream() {return m_pDataStream.get();}

		void			AddCallBack(const RES_CALL_BACK& fCallBack);

	protected:
		virtual bool    InitRes() {return true;}
		virtual bool	ChildResFinish(){return true;}

	private:
		bool			LoadFileToMemeory();
		bool			CreateFromMemeory();

	protected:
		std::string				m_sResPath;
		std::atomic<ResState>	m_eResState;
		RefPtr<MemoryStream>	m_pDataStream;	

		//typedef list< RefPtr<Resource> > LST_RESOURCE;
		//LST_RESOURCE			m_lstChild;

		std::vector<RES_CALL_BACK> m_listCallBack;

		friend class DataThread;
	};

	RefPtr<Resource> CreateResource(const char* pszPath);
}

