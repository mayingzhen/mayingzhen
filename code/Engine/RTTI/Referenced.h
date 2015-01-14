#ifndef _CREFERENCED_H
#define _CREFERENCED_H 

#include "RefPtr.h"

namespace ma
{
	class  Referenced
	{
	public:
		Referenced():_refCount(0) {}
		
		Referenced(const Referenced&):_refCount(0) {}

		virtual ~Referenced()
		{ 
			if (_refCount>0)
			{
				ASSERT(false);
			}
		}

		inline Referenced& operator = (const Referenced&) { return *this; }

		/** Increment the reference count by one, indicating that 
			this object has another pointer which is referencing it.*/
		virtual int Ref() const {return ++_refCount;}
	    
		/** Decrement the reference count by one, indicating that 
			a pointer to this object is referencing it.  If the
			reference count goes to zero, it is assumed that this object
			is no longer referenced and is automatically deleted.*/
		virtual int Unref() const
		{
			int newRef = --_refCount;
			bool needDelete = (newRef == 0);
			if (needDelete)
			{
				delete this;
			}

			return newRef;
		}
	    
		inline int GetRefCount() const{return _refCount;}

	private:
		mutable int	_refCount;
	};

}


#endif
