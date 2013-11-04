#ifndef  _ResoureceSystem_RTTI__H__
#define  _ResoureceSystem_RTTI__H__


namespace ma
{
	void ResourceSystemRTTIInit()
	{
		Resource::StaticInitClass();
	}

	void ResourceSystemRTTIShutdown()
	{
		Resource::StaticShutdownClass();
	}

}



#endif