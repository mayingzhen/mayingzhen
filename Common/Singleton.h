#ifndef __Singleton_H__
#define __Singleton_H__


//#include <Animation/Common/assert.h>

namespace ma
{
	template <typename T> class COMMON_API Singleton
	{
	protected:

		static T* ms_singleton;

	public:
		Singleton( void )
		{
			assert( !ms_singleton );
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
			int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
			ms_singleton = (T*)((int)this + offset);
#else
			ms_singleton = static_cast< T* >( this );
#endif
		}

		~Singleton( void )
		{ 
			assert( NULL != ms_singleton ); 
			ms_singleton = 0;  
		}

		static T& GetInstance( void )
		{	
			assert( NULL != ms_singleton );  
			return ( *ms_singleton );
		}

		static T* GetInstancePtr( void )
		{ 
			return ms_singleton; 
		}
	};
}


#endif// __Singleton_H__

