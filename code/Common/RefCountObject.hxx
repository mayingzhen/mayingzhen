#include <Common/RefCountObject.h>

namespace ma
{
	RefCountObject::RefCountObject()
		:m_nRefCnt(1)
	{
	}

	RefCountObject::RefCountObject(const RefCountObject& rhs)
		:m_nRefCnt(1)
	{
		*this = rhs;
	}


	RefCountObject::~RefCountObject()
	{
	}


	void RefCountObject::IncReference() const
	{
		ASSERT(m_nRefCnt > 0);
		++m_nRefCnt;
	}


	void RefCountObject::DecReference() const
	{
		if (m_nRefCnt == 1)
		{
			m_nRefCnt = 0;
			delete this;
		}else{
			--m_nRefCnt;
		}

	}

	UINT RefCountObject::GetReferenceCount() const
	{
		return m_nRefCnt;
	}

	RefCountObject& RefCountObject::operator=(const RefCountObject& rhs)
	{
		return *this;
	}

}

