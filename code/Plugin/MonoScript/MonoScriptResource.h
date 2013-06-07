#ifndef __MonoScriptResource_h__
#define __MonoScriptResource_h__



namespace ma
{
	class MonoScriptAPI MonoScriptResource : public Resource
	{
	public:
		MonoScriptResource(const char* pszPath = NULL);

		~MonoScriptResource();

		virtual bool Load();

		virtual void Unload();

	};
}



#endif //__MonoScriptResource_h__