#ifndef __ReferenceCountObject_H__
#define __ReferenceCountObject_H__

namespace ma
{
	class COMMON_API RefCountObject
	{
		mutable UINT m_nRefCnt;

	public:

		RefCountObject();

		RefCountObject(const RefCountObject& rhs);

		virtual ~RefCountObject();

		virtual void IncReference() const;

		virtual void DecReference() const;

		RefCountObject& operator=(const RefCountObject& rhs);

		UINT GetReferenceCount() const;
	};

}


#endif// __ReferenceCountObject_H__