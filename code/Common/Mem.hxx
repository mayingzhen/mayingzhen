#include "Mem.h"
#include "StaticFunc.h"
#include "Assert.h"

namespace ma
{
	MemHeap::MemHeap(const char* pszClassName, int size)
	{
		m_nClassSize = size;
		m_lMaxAllocsInHeap = 0;

		if (pszClassName)
		{
			strncpy(m_strName, pszClassName, sizeof(m_strName) - 1);
			m_strName[sizeof(m_strName) - 1] = '\0';
		}
	}

	MemHeap::~MemHeap(void)
	{
		if (m_lMaxAllocsInHeap > 0)
		{
			StaticFunc::DebugMsg("heap of class %s destroyed, with max alloc(%ld).", m_strName, m_lMaxAllocsInHeap);
		}

		long nAllocs = m_lMaxAllocsInHeap - m_vecFree.size();
		if (nAllocs > 0)
		{
			StaticFunc::DebugMsg("CMemHeap::~CMemHeap() [%s] have [%d] memm trunk not realse", m_strName, nAllocs);
		}

		for (std::list<void*>::iterator iter = m_vecFree.begin();iter != m_vecFree.end();++iter)
		{
			free(*iter);
		}
		m_vecFree.clear();
	}

	void* MemHeap::Alloc( size_t size )
	{
		if (m_nClassSize != size)
		{
			ASSERT(false);
			return NULL;
		}

		if (m_vecFree.empty())
		{
			++m_lMaxAllocsInHeap;
			return malloc(size);
		}

		void* p = m_vecFree.back();
		m_vecFree.pop_back();
		return p;
	}

	void MemHeap::Free( void* ptr )
	{
		m_vecFree.push_back(ptr);
	}

	void MemHeap::ClearMemHeap()
	{
		m_lMaxAllocsInHeap -= m_vecFree.size();
		for (std::list<void*>::iterator iter = m_vecFree.begin();iter != m_vecFree.end();++iter)
		{
			free(*iter);
		}
		m_vecFree.clear();
	}

	unsigned long long MemHeap::GetSizeInBytesMemHeap() const
	{
		return m_lMaxAllocsInHeap*m_nClassSize;
	}
}


