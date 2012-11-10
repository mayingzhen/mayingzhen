#ifndef  _RESOURCE__H__
#define  _RESOURCE__H__

namespace ma
{
	class FRAMEWORK_API Resource
	{
	public:
		Resource(const char* pResPath = NULL);

		virtual ~Resource();

		virtual void Load();

		virtual const char* GetResPath() {return m_sResPath.c_str();}

		virtual bool IsLoad() {return m_bLoaded;}

	protected:
		std::string m_sResPath;
		bool		m_bLoaded;
	};
}



#endif