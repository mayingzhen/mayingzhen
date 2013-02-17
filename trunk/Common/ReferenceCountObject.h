#ifndef __ReferenceCountObject_H__
#define __ReferenceCountObject_H__

namespace ma
{
	class COMMON_API ReferenceCountObject
	{
		mutable xmUint m_nRefCnt;

	public:

		ReferenceCountObject();

		ReferenceCountObject(const ReferenceCountObject& rhs);

		virtual ~ReferenceCountObject();

		virtual void IncReference() const;

		virtual void DecReference() const;

		ReferenceCountObject& operator=(const ReferenceCountObject& rhs);

		xmUint GetReferenceCount() const;
	};

}


#endif// __ReferenceCountObject_H__