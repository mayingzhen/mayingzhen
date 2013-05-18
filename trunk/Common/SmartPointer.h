#ifndef __SmartPointer_H__
#define __SmartPointer_H__


template<class T>
class SmartPointer
{
	T*				m_pData;

public:

	typedef T value_type;

	SmartPointer();

	SmartPointer(T* pData);

	SmartPointer(const SmartPointer<T>& rhs);

	~SmartPointer();

	bool				IsNull()const;

	void				SetNull();

	SmartPointer&	operator=(const SmartPointer<T>& rhs);

	T*					operator->();

	T*					GetData();

	const T*			operator->() const;

	const T*			GetData() const;

	bool				operator == (T*) const;



};

template<class T>
SmartPointer<T>::SmartPointer()
:m_pData(NULL)
{
}

template<class T>
SmartPointer<T>::SmartPointer(T* pData)
:m_pData(pData)
{
	if (pData)
	{
		pData->IncReference();
	}
}

template<class T>
SmartPointer<T>::SmartPointer(const SmartPointer<T>& rhs)
:m_pData(NULL)
{
	*this = rhs;
}

template<class T>
SmartPointer<T>::~SmartPointer()
{
	if (m_pData)
	{
		m_pData->DecReference();
	}

}

template<class T>
bool SmartPointer<T>::IsNull()const
{
	return NULL == m_pData;
}

template<class T>
void SmartPointer<T>::SetNull()
{
	*this = NULL;
}

template<class T>
SmartPointer<T>& SmartPointer<T>::operator=(const SmartPointer<T>& rhs)
{
	if (this != &rhs)
	{
		if (rhs.m_pData)
		{
			rhs.m_pData->IncReference();
		}

		if (m_pData)
		{
			m_pData->DecReference();
		}	

		m_pData = rhs.m_pData;
	}

	return *this;
}

template<class T>
T* SmartPointer<T>::operator->()
{
	ASSERT(NULL != m_pData);
	return m_pData;
}

template<class T>
const T* SmartPointer<T>::operator->() const 
{
	ASSERT(NULL != m_pData);
	return m_pData;
}

template<class T>
T* SmartPointer<T>::GetData() 
{
	return m_pData;
}


template<class T>
const T* SmartPointer<T>::GetData() const 
{
	ASSERT(NULL != m_pData);
	return m_pData;
}



template<class T>
bool				SmartPointer<T>::operator == (T* rhs) const
{
	return m_pData == rhs;
}

template<class T>
class SmartConstPointer
{
	const T*				m_pData;

public:

	typedef T value_type;

	SmartConstPointer();

	SmartConstPointer(const T* pData);

	SmartConstPointer(const SmartConstPointer<T>& rhs);

	~SmartConstPointer();

	bool				IsNull()const;

	void				SetNull();

	SmartConstPointer&	operator=(const SmartConstPointer<T>& rhs);



	const T*			operator->() const;

	const T*			GetData() const;

	bool				operator == (T* rhs) const;


};


#include "SmartPointer.inl"

#define DeclareSmartPtr(classname)  \
	class classname;                \
	typedef SmartConstPointer<classname> classname##Ptr



#endif// __SmartPointer_H__