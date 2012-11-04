#ifndef  _RESOURCE__H__
#define  _RESOURCE__H__

namespace ma
{
	class Resource
	{
	public:
		Resource();
		
		virtual ~Resource();

		virtual void Load() = 0;

		virtual bool IsLoad() = 0;
	
	
	};
}



#endif