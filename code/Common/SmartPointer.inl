


template<class T>
SmartConstPointer<T>::SmartConstPointer()
:m_pData(NULL)
{
}

template<class T>
SmartConstPointer<T>::SmartConstPointer(const T* pData)
:m_pData(pData)
{
	if (pData)
	{
		pData->IncReference();
	}
}

template<class T>
SmartConstPointer<T>::SmartConstPointer(const SmartConstPointer<T>& rhs)
:m_pData(NULL)
{
	*this = rhs;
}

template<class T>
SmartConstPointer<T>::~SmartConstPointer()
{
	if (m_pData)
	{
		m_pData->DecReference();
	}

}

template<class T>
bool SmartConstPointer<T>::IsNull()const
{
	return NULL == m_pData;
}

template<class T>
void SmartConstPointer<T>::SetNull()
{
	*this = NULL;
}

template<class T>
SmartConstPointer<T>& SmartConstPointer<T>::operator=(const SmartConstPointer<T>& rhs)
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
const T* SmartConstPointer<T>::operator->() const 
{
	ASSERT(NULL != m_pData);
	return m_pData;
}


template<class T>
const T* SmartConstPointer<T>::GetData() const 
{

	return m_pData;
}

template<class T>
bool				SmartConstPointer<T>::operator == (T* rhs) const
{
	return m_pData == rhs;
}