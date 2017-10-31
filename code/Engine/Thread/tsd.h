#pragma once

#ifdef __GNUC__
#define FORCEINLINE		inline __attribute__ ((always_inline))
#include <pthread.h>
#endif

#ifdef WIN32
#define FORCEINLINE		__forceinline
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

//TSD Thread-Specific data area
//TLS Thread-Local storage
//this two are the same !~
namespace ma
{
	FORCEINLINE bool TSDAlloc(unsigned long* index)
	{
#ifdef WIN32
		unsigned long i = TlsAlloc();
		if (i == TLS_OUT_OF_INDEXES)
		{
			return false;
		}
		else
		{
			*index = i;
			return true;
		}
#endif
#ifdef __GNUC__
		pthread_key_t key = -1;
		int result = pthread_key_create(&key, 0);
		if (result == 0)
		{
			*index = key;
			return true;
		}
		else
		{
			return false;
		}
#endif
	}

	FORCEINLINE void TSDFree(unsigned long index)
	{
#ifdef WIN32
		TlsFree(index);
#endif
#ifdef __GNUC__
		pthread_key_delete(index);
#endif
	}

	FORCEINLINE void* TSDGet(unsigned long index)
	{
#ifdef WIN32
		return TlsGetValue(index);
#endif
#ifdef __GNUC__
		return pthread_getspecific(index);
#endif
	}

	FORCEINLINE bool TSDSet(unsigned long index, void* value)
	{
#ifdef WIN32
		return TlsSetValue(index, value) != 0;
#endif
#ifdef __GNUC__
		return pthread_setspecific(index, value) == 0;
#endif
	}

	// 用法:
	//     static ThreadLocalStorage<int> s_tls;
	//     if (!s_tls) { s_tls.alloc(1); }	// 当前线程还未分配, 则为当前线程分配一个TLS存储空间
	//     *s_tls.get() = 1;				// 赋值
	///////////////////////////Thread local storage///////////////////////////
	template<typename T>
	class ThreadLocalStorage
	{
	private:
		unsigned long m_key;

	public:
		ThreadLocalStorage()
		{
			TSDAlloc(&m_key);
		}

		~ThreadLocalStorage()
		{
			free();
			TSDFree(m_key);
		}

		bool operator !()
		{
			return TSDGet(m_key) == NULL;
		}

		T* operator ->()
		{
			return static_cast<T*>(TSDGet(m_key));
		}

		FORCEINLINE T* get()
		{
			return static_cast<T*>(TSDGet(m_key));
		}

		T* alloc(int size)
		{
			T* p = get();
			if (p)
			{
				delete[] p;
			}
			p = new T[size];
			TSDSet(m_key, p);
			return p;
		}

		void free()
		{
			T* p = get();
			if (p)
			{
				delete[] p;
				TSDSet(m_key, NULL);
			}
		}
	};
}


