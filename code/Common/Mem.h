#pragma once

#include "list"

namespace ma
{
	class MemHeap
	{
	public:
		MemHeap(const char* pszClassName, int size);
		~MemHeap(void);

	public:
		void* Alloc(size_t size);
		void Free(void* ptr);
		void ClearMemHeap();
		unsigned long long GetSizeInBytesMemHeap() const;
	protected:
		int m_nClassSize;
		long m_lMaxAllocsInHeap;
		char m_strName[32];
		std::list<void*> m_vecFree;
	};

#define MEMHEAP_DECLARATION(cls)																				\
public:																											\
	void* operator new(size_t size){return s_heap##cls.Alloc(size);}											\
	void operator delete(void* p){s_heap##cls.Free(p);}															\
	static void ClearMemHeap(){return s_heap##cls.ClearMemHeap();}\
	static unsigned long long GetSizeInBytesMemHeap(){return s_heap##cls.GetSizeInBytesMemHeap();}\
	static CMemHeap s_heap##cls

#define MEMHEAP_IMPLEMENTATION(cls) CMemHeap cls::s_heap##cls(#cls, sizeof(cls))


	inline void* AlignedMemory_allocate(size_t size, size_t alignment)
	{
#ifdef WIN32
		return _aligned_malloc(size, alignment);
#elif __ANDROID__
		return ::memalign(alignment, size);
#else//slime
		void * aPtr = NULL;
		if (posix_memalign(&aPtr, alignment, size)) {
			aPtr = NULL;
		}
		return aPtr;
#endif

	}
	//---------------------------------------------------------------------
	inline void AlignedMemory_deallocate(void* ptr)
	{
		if (ptr == NULL)
			return;

#ifdef WIN32
		_aligned_free(ptr);
#elif __ANDROID__
		::free(ptr);
#else//slime
		::free(ptr);
#endif
	}
}
