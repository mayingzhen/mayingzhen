#ifndef __ReferenceCountObject_H__
#define __ReferenceCountObject_H__

namespace ma
{
	class COMMON_API ReferenceCountObject
	{
		mutable UINT m_nRefCnt;

	public:

		ReferenceCountObject();

		ReferenceCountObject(const ReferenceCountObject& rhs);

		virtual ~ReferenceCountObject();

		virtual void IncReference() const;

		virtual void DecReference() const;

		ReferenceCountObject& operator=(const ReferenceCountObject& rhs);

		UINT GetReferenceCount() const;
	};

}


#endif// __ReferenceCountObject_H__