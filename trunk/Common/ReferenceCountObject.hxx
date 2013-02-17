#include <Common/ReferenceCountObject.h>

namespace ma
{
	ReferenceCountObject::ReferenceCountObject()
		:m_nRefCnt(1)
	{
	}

	ReferenceCountObject::ReferenceCountObject(const ReferenceCountObject& rhs)
		:m_nRefCnt(1)
	{
		*this = rhs;
	}


	ReferenceCountObject::~ReferenceCountObject()
	{
	}


	void ReferenceCountObject::IncReference() const
	{
		assert(m_nRefCnt > 0);
		++m_nRefCnt;
	}


	void ReferenceCountObject::DecReference() const
	{
		if (m_nRefCnt == 1)
		{
			m_nRefCnt = 0;
			delete this;
		}else{
			--m_nRefCnt;
		}

	}

	xmUint ReferenceCountObject::GetReferenceCount() const
	{
		return m_nRefCnt;
	}

	ReferenceCountObject& ReferenceCountObject::operator=(const ReferenceCountObject& rhs)
	{
		return *this;
	}

}

