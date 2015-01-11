#ifndef __TARRAY_H__
#define __TARRAY_H__

namespace ma
{

// this is an allocator that's allocates 16-byte-aligned blocks,
// using the normal mem manager. The overhead of allocation is always 16 byte more,
// to keep the original address in the DWORD before the aligned address
// NOTE: since this is like a garbage collector (the items are not guaranteed to be
// freed immediately), the destructors won't be called
// for simplicity, constructors won't be called either
//
// DOES NOT construct / destruct objects, so use with care!
template <typename T>
class TAllocator16
{
public:
	typedef size_t    size_type;
#ifdef WIN64
	typedef int64    difference_type;
#else
	typedef int      difference_type;
#endif //WIN64
	typedef T*        pointer;
	typedef const T*  const_pointer;
	typedef T&        reference;
	typedef const T&  const_reference;
	typedef T         value_type;

	template <class U> 
	struct rebind { typedef TAllocator16<U> other; };

	size_type max_size() const
	{	// estimate maximum array size
		size_type _Count = (size_type)(-1) / sizeof (T);
		return (0 < _Count ? _Count : 1);
	}

	TAllocator16 ()
#if defined(_DEBUG) && defined(_INC_CRTDBG) && !defined(WIN64) && !defined(PS3)
		: m_szParentObject("TAllocator16"),
		m_nParentIndex (1)
#endif
	{
	}
	TAllocator16 (const char* szParentObject, int nParentIndex)
#if defined(_DEBUG) && defined(_INC_CRTDBG) && !defined(WIN64) && !defined(PS3)
		: m_szParentObject(szParentObject),
		m_nParentIndex (nParentIndex)
#endif
	{
	}

	// allocates the aligned memory for the given number of objects;
	// puts the pointer to the actually allocated block before the aligned memory block
	pointer allocate (size_type _Count)
	{
		pointer p;
#if defined(_DEBUG) && defined(_INC_CRTDBG) && !defined(WIN64) && !defined(PS3)
		p = (pointer)_malloc_dbg (0x10+_Count * sizeof(T), _NORMAL_BLOCK, m_szParentObject, m_nParentIndex);
#else
		p = (pointer)malloc (0x10+_Count*sizeof(T));
#endif
		pointer pResult = (pointer)(((UINT_PTR)p+0x10)&~0xF);
		// save the offset to the actual allocated address behind the useable aligned address
		reinterpret_cast<int*>(pResult)[-1] = (char*)p - (char*)pResult;
		return pResult;
	}

	pointer allocate_construct (size_type _Count)
	{
		pointer p = allocate(_Count);
		return p;
	}

	void deallocate_destroy(pointer _Ptr)
	{
		if (!_Ptr)
			return;
		int nOffset = ((int*)_Ptr)[-1];
		ASSERT (nOffset >= -16 && nOffset <= -4);
#if defined(_DEBUG) && defined(_INC_CRTDBG) && !defined(WIN64) && !defined(PS3)
		_free_dbg (((char*)_Ptr)+nOffset, _NORMAL_BLOCK);
#else
		free (((char*)_Ptr)+nOffset);
#endif
	}

	void deallocate(pointer _Ptr,size_type _Count) {}
	void destroy(pointer _Ptr) {}
	void construct(pointer _Ptr, const T& _Val) {}

protected:
#if defined(_DEBUG) && defined(_INC_CRTDBG) && !defined(WIN64) && !defined(PS3)
	// the parent object (on behalf of which to call the new operator)
	const char* m_szParentObject; // the file name
	int m_nParentIndex; // the file line number
#endif
};


// General array class.
// Can refer to a general (unowned) region of memory (m_nAllocatedCount = 0).
// Can allocate, grow, and shrink an array.
// Does not deep copy.

template<class T> class TArray
{
protected:
  T*    m_pElements;
  unsigned int m_nCount;
  unsigned int m_nAllocatedCount;

public:
	typedef T value_type;

	// Empty array.
  TArray()       
	{ 
		m_pElements = NULL;
		m_nCount = m_nAllocatedCount = 0; 
	}

	// Create a new array, delete it on destruction.
  TArray(int Count)  
	{ 
		m_nCount = Count; m_nAllocatedCount = Count; 
		m_pElements = NULL; 
		Realloc(); 
	}
  TArray(int Use, int Max)  
	{ 
		m_nCount = Use; m_nAllocatedCount = Max; 
		m_pElements = NULL; 
		Realloc(); 
	}

	// Reference pre-existing memory. Does not delete it.
  TArray(T* Elems, int Count)  
	{ 
		m_pElements = Elems; m_nCount = Count; m_nAllocatedCount = 0; 
	}
  ~TArray()
  {
    Free();
  }

  void Reset() 
	{ 
		Free(); 
	}

  void Free()
  {
    if (m_nAllocatedCount)
    {
      free(m_pElements);
    }
    m_pElements = NULL;
    m_nCount = m_nAllocatedCount = 0;
  }

  void Create (int Count) 
	{ 
		m_pElements = NULL; m_nCount = Count; m_nAllocatedCount = Count; Realloc(); Clear(); 
	}
  void Copy (const TArray<T>& src)
  {
    m_pElements = NULL;
    m_nCount = m_nAllocatedCount = src.Num();
    Realloc();
    memcpy(m_pElements, src.m_pElements, src.Num()*sizeof(T));
  }
  void Copy (const T *src, unsigned int numElems)
  {
    int nOffs = m_nCount;
    Grow(numElems);
    memcpy(&m_pElements[nOffs], src, numElems*sizeof(T));
  }
  void Align4Copy (const T *src, unsigned int& numElems)
  {
    int nOffs = m_nCount;
    Grow((numElems+3)&~3);
    memcpy(&m_pElements[nOffs], src, numElems*sizeof(T));
    if (numElems & 3)
    {
      int nSet = 4 - (numElems & 3);
      memset(&m_pElements[nOffs+numElems], 0, nSet);
      numElems += nSet;
    }
  }

  void Realloc()
  {
		if (!m_nAllocatedCount)
			m_pElements = NULL;
		else
		{
			//PREFAST_SUPPRESS_WARNING(6308) 
			m_pElements = (T *)realloc(m_pElements, m_nAllocatedCount*sizeof(T));

			ASSERT (m_pElements);
		}
  }

  void Remove(unsigned int Index,unsigned int Count=1)
  {
    if ( Count )
    {
      memmove(m_pElements+Index, m_pElements+(Index+Count), sizeof(T)*(m_nCount-Index-Count));
      m_nCount -= Count;
    }
  }

  void Shrink()
  {
    if (m_nCount == 0 || m_nAllocatedCount == 0)
      return;
    ASSERT(m_nAllocatedCount>=m_nCount);
    if( m_nAllocatedCount != m_nCount )
    {
      m_nAllocatedCount = m_nCount;
      Realloc();
    }
  }

  void _Remove(unsigned int Index,unsigned int Count)
  {
    ASSERT ( Index>=0 );
    ASSERT ( Index<=m_nCount );
    ASSERT ( (Index+Count)<=m_nCount );

    Remove(Index, Count);
  }

  unsigned int Num(void) const  { return m_nCount; }
  unsigned int Capacity(void) const { return m_nAllocatedCount; }
  unsigned int MemSize(void) const { return m_nCount * sizeof(T); }
  void SetNum(unsigned int n) { m_nCount = m_nAllocatedCount = n; }
  void SetUse(unsigned int n) { m_nCount = n; }
  void Alloc(unsigned int n) { m_nAllocatedCount = n; Realloc(); }
  void Reserve(unsigned int n) { SetNum(n); Realloc(); Clear(); }
  void ReserveNoClear(unsigned int n) { SetNum(n); Realloc(); }
  void Expand(unsigned int n)
  {
    if (n > m_nAllocatedCount)
      ReserveNew(n);
  }
  void ReserveNew(unsigned int n)
  {
    int num = m_nCount;
    if (n > m_nAllocatedCount)
    {
      m_nAllocatedCount = n * 2;
      Realloc();
    }
    m_nCount = n;
    memset(&m_pElements[num], 0, sizeof(T)*(m_nCount-num));
  }
  T* Grow(unsigned int n)
  {
    int nStart = m_nCount;
    m_nCount += n;
    if (m_nCount > m_nAllocatedCount)
    {
      m_nAllocatedCount = m_nCount * 2;
      Realloc();
    }
    return &m_pElements[nStart];
  }
  T* GrowReset(unsigned int n)
  {
    int num = m_nAllocatedCount;
    T* Obj = AddIndex(n);
    if (num != m_nAllocatedCount)
      memset(&m_pElements[num], 0, sizeof(T)*(m_nAllocatedCount-num));
    return Obj;
  }

  unsigned int *GetNumAddr(void) { return &m_nCount; }
  T** GetDataAddr(void) { return &m_pElements; }

  T* Data(void) const { return m_pElements; }
  T& Get(unsigned int id) const { return m_pElements[id]; }

	
  void Assign(TArray& fa)
  {
    m_pElements = fa.m_pElements;
    m_nCount = fa.m_nCount;
    m_nAllocatedCount = fa.m_nAllocatedCount;
  }
	

  const T& operator[](unsigned int i) const { ASSERT(i < m_nAllocatedCount); return m_pElements[i]; }
        T& operator[](unsigned int i)       { ASSERT(i < m_nAllocatedCount); return m_pElements[i]; }
  T& operator * ()													{ ASSERT(m_nAllocatedCount > 0); return *m_pElements;   }

	TArray<T> operator()(unsigned int Start)							
	{ 
		ASSERT(Start < m_nCount); 
		return TArray<T>(m_pElements+Start, m_nCount-Start); 
	}
	TArray<T> operator()(unsigned int Start, unsigned int Count)	
	{ 
		ASSERT(Start < m_nCount); 
		ASSERT(Start+Count <= m_nCount); 
		return TArray<T>(m_pElements+Start, Count); 
	}

  TArray(const TArray<T>& cTA)
  {
    m_pElements = NULL;
    m_nCount = m_nAllocatedCount = cTA.Num();
    Realloc();
    m_nCount = 0;
    for (unsigned int i=0; i<cTA.Num(); i++ )
    {
      AddElem(cTA[i]);
    }
  }
  void ClearArr(void)
  {
    m_pElements = NULL;
    m_nCount = 0;
    m_nAllocatedCount = 0;
  }

  void Clear(void)
  {
    memset(m_pElements, 0, m_nCount*sizeof(T));
  }

  void AddString(const char *szStr)
  {
    ASSERT(szStr);
    int nLen = strlen(szStr)+1;
    T *pDst = Grow(nLen);
    memcpy(pDst, szStr, nLen);
  }

  void Set(unsigned int m)
  {
    memset(m_pElements, m, m_nCount*sizeof(T));
  }

  inline T* AddIndex(unsigned int inc)
  {
    unsigned int nIndex = m_nCount;
    unsigned int nNewCount = m_nCount + inc;
    
		if ( nNewCount > m_nAllocatedCount )
    {
      m_nAllocatedCount = nNewCount + (nNewCount>>1) + 10;
      Realloc();
    }

		m_nCount = nNewCount;
    return &m_pElements[nIndex];
  }

  T& Insert(unsigned int nIndex, unsigned int inc=1)
  {
    m_nCount += inc;
    if ( m_nCount > m_nAllocatedCount )
    {
      m_nAllocatedCount = m_nCount + (m_nCount>>1) + 32;
      Realloc();
    }
    memmove(&m_pElements[nIndex+inc], &m_pElements[nIndex], (m_nCount-inc-nIndex)*sizeof(T));

    return m_pElements[nIndex];
  }

  void AddIndexNoCache(unsigned int inc)
  {
    m_nCount += inc;
    if ( m_nCount > m_nAllocatedCount )
    {
      m_nAllocatedCount = m_nCount;
      Realloc();
    }
  }

  void Add(const T & elem){AddElem(elem);}
  void AddElem(const T& elem)
  {
    unsigned int m = m_nCount;
    AddIndex(1);
    m_pElements[m] = elem;
  }
  void AddElemNoCache(const T& elem)
  {
    unsigned int m = m_nCount;
    AddIndexNoCache(1);
    m_pElements[m] = elem;
  }

  int Find(const T & p)
  {
    for(unsigned int i=0; i<m_nCount; i++)
    {
      if(p == (*this)[i])
        return i;
    }
    return -1;
  }

  void Delete(unsigned int n){DelElem(n);}
  void DelElem(unsigned int n)
  {
//    memset(&m_pElements[n],0,sizeof(T));
    _Remove(n, 1);
  }


  void SaveToBuffer(const unsigned char * pBuffer, int & nPos)
  {
    // copy size of element
    int nSize = sizeof(T);
    if(pBuffer)
      memcpy((void*)&pBuffer[nPos],&nSize,4);
    nPos+=4;

    // copy count
    if(pBuffer)
      memcpy((void*)&pBuffer[nPos],&m_nCount,4);
    nPos+=4;

    // copy data
    if(m_nCount)
    {
      if(pBuffer)
        memcpy((void*)&pBuffer[nPos], m_pElements, sizeof(T)*m_nCount);
      nPos += sizeof(T)*m_nCount;
    }
  }

  void LoadFromBuffer(const unsigned char * pBuffer, int & nPos)
  {
    Reset();

    // copy size of element
    int nElemSize = 0;
    memcpy(&nElemSize,(void*)&pBuffer[nPos],4);
    ASSERT(nElemSize == sizeof(T));
    nPos+=4;

    // copy count
    int nNewCount=0;
    memcpy((void*)&nNewCount,&pBuffer[nPos],4);
    ASSERT(nNewCount>=0 && nNewCount<1000000);
    nPos+=4;

    // copy data
    if(nNewCount)
    {
			Reserve(nNewCount);
      memcpy((void*)m_pElements, &pBuffer[nPos], sizeof(T)*nNewCount);
      nPos += sizeof(T)*nNewCount;
    }
  }

	// Standard compliance interface
	//
	// This is for those who don't want to learn the non standard and 
	// thus not very convenient interface of TArray, but are unlucky
	// enough not to be able to avoid using it.
	void clear(){Free();}
	void resize( int nSize ) { Alloc(nSize); m_nCount = nSize; }
	unsigned size()const {return m_nCount;}
	unsigned capacity() const {return m_nAllocatedCount;}
	bool empty()const {return size() == 0;}
	void push_back (const T& rSample) {Add(rSample);}
  void pop_back () {m_nCount--;}
	T* begin() {return m_pElements;}
	T* end() {return m_pElements + m_nCount;}
  T last() {return m_pElements[m_nCount-1];}
	const T* begin()const {return m_pElements;}
	const T* end()const {return m_pElements + m_nCount;}

	int GetMemoryUsage() { return (int)(m_nAllocatedCount*sizeof(T)); }
};

template <class T> inline void Exchange(T& X, T& Y)
{
  const T Tmp = X;
  X = Y;
  Y = Tmp;
}

}

#endif // __TARRAY_H__
