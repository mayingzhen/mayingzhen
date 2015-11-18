#ifndef _TSIGNAL_H_
#define _TSIGNAL_H_
#include <algorithm>
#include <stdio.h>
#include "delegate.h"

#include <vector>

typedef float float32 ;

using namespace std;
class SignalBase
{
public:

   SignalBase()
   {
      mList.next = mList.prev = &mList;
      mList.order = 0.5f;
   }

   ~SignalBase();

   /// Removes all the delegates from the signal.
   void removeAll();

   /// Returns true if the delegate list is empty.
   bool isEmpty() const
   {
      return mList.next == &mList;
   }

protected:

   struct DelegateLink
   {
      DelegateLink *next,*prev;
      float order;

      void insert(DelegateLink* node, float order);
      void unlink();
   };

   DelegateLink mList;

   vector<DelegateLink*> mTriggerNext;
};

template<typename Signature> 
class SignalBaseT : public SignalBase
{
public:

   typedef Delegate<Signature> DelegateSig;

   SignalBaseT() {}

   SignalBaseT( const SignalBaseT &base )
   {
      mList.next = mList.prev = &mList;
      merge( base );
   }

   void operator =( const SignalBaseT &base )
   {
      removeAll();
      merge( base );
   }

   int count()
   {
	   int result = 0;
	   for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
	   {
		   result++;
		   ptr = ptr->next;
	   }
	   return result;
   }

   DelegateSig& get(int index)
   {
	   int cursor = 0;
	   for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
	   {
		   if(cursor == index)
		   {
			   return this->getDelegate( ptr );
		   }
		   cursor++;
		   ptr = ptr->next;
	   }

   }

   void merge( const SignalBaseT &base )
   {
      for ( DelegateLink *ptr = base.mList.next; ptr != &base.mList; ptr = ptr->next )
      {
         DelegateLinkImpl *del = static_cast<DelegateLinkImpl*>( ptr );
         notify( del->mDelegate, del->order );
      }
   }

   void notify( const DelegateSig &dlg, float32 order = 0.5f)
   {
      mList.insert(new DelegateLinkImpl(dlg), order);
   }

   void remove( DelegateSig dlg )
   {
      for( DelegateLink* ptr = mList.next;ptr != &mList; ptr = ptr->next )
      {
         if( DelegateLinkImpl* del = static_cast< DelegateLinkImpl* >( ptr ) )
         {
            if( del->mDelegate == dlg )
            {
               for ( unsigned int i = 0; i < mTriggerNext.size(); i++ )
               {
                  if( mTriggerNext[i] == ptr )
                     mTriggerNext[i] = ptr->next;
               }

               del->unlink();
               delete del;
               return;
            }
         }
      }
   } 

   template <class T,class U>
   void notify(T obj,U func, float32 order = 0.5f)
   {
      DelegateSig dlg(obj, func);
      notify(dlg, order);
   }

   template <class T>
   void notify(T func, float32 order = 0.5f)
   {
      DelegateSig dlg(func);
      notify(dlg, order);
   }

   template <class T,class U>
   void remove(T obj,U func)
   {
      DelegateSig compDelegate(obj, func);
      remove(compDelegate);
   }

   template <class T>
   void remove(T func)
   {
      DelegateSig compDelegate(func);
      remove(compDelegate);
   } 

   /// Returns true if the signal already contains this delegate.
   bool contains( const DelegateSig &dlg ) const
   {
      for ( DelegateLink *ptr = mList.next; ptr != &mList; ptr = ptr->next )
      {
         DelegateLinkImpl *del = static_cast<DelegateLinkImpl*>( ptr );
         if ( del->mDelegate == dlg )
            return true;
      }

      return false;
   } 

protected:

   struct DelegateLinkImpl : public SignalBase::DelegateLink
   {
      DelegateSig mDelegate;
      DelegateLinkImpl(DelegateSig dlg) : mDelegate(dlg) {}
   };

   DelegateSig & getDelegate(SignalBase::DelegateLink * link)
   {
      return ((DelegateLinkImpl*)link)->mDelegate;
   }
};

//-----------------------------------------------------------------------------

template<typename Signature> 
class Signal;


template<> 
class Signal<bool()> : public SignalBaseT<bool()>
{
   public:

      bool trigger()
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            if( !this->getDelegate( ptr )() )
            {
               this->mTriggerNext.pop_back();
               return false;
            }
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
         return true;
      }
};

template<class A> 
class Signal<bool(A)> : public SignalBaseT<bool(A)>
{   
   public:

      bool trigger( A a )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            if( !this->getDelegate( ptr )( a ) )
            {
               this->mTriggerNext.pop_back();
               return false;
            }
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
         return true;
      }
};

template<class A, class B>
class Signal<bool(A,B)> : public SignalBaseT<bool(A,B)>
{
   public:

      bool trigger( A a, B b )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            if( !this->getDelegate( ptr )( a, b ) )
            {
               this->mTriggerNext.pop_back();
               return false;
            }
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
         return true;
      }
};

template<class A, class B, class C> 
class Signal<bool(A,B,C)> : public SignalBaseT<bool(A,B,C)>
{
   public:

      bool trigger( A a, B b, C c )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            if( !this->getDelegate( ptr )( a, b, c ) )
            {
               this->mTriggerNext.pop_back();
               return false;
            }
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
         return true;
      }
};

template<class A, class B, class C, class D> 
class Signal<bool(A,B,C,D)> : public SignalBaseT<bool(A,B,C,D)>
{
   public:

      bool trigger( A a, B b, C c, D d )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            if( !this->getDelegate( ptr )( a, b, c, d ) )
            {
               this->mTriggerNext.pop_back();
               return false;
            }
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
         return true;
      }
};

template<class A, class B, class C, class D, class E> 
class Signal<bool(A,B,C,D,E)> : public SignalBaseT<bool(A,B,C,D,E)>
{
   public:

      bool trigger( A a, B b, C c, D d, E e )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            if( !this->getDelegate( ptr )( a, b, c, d, e ) )
            {
               this->mTriggerNext.pop_back();
               return false;
            }
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
         return true;
      }
};

template<class A, class B, class C, class D, class E, class F> 
class Signal<bool(A,B,C,D,E,F)> : public SignalBaseT<bool(A,B,C,D,E,F)>
{
   public:

      bool trigger( A a, B b, C c, D d, E e, F f )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            if( !this->getDelegate( ptr )( a, b, c, d, e, f ) )
            {
               this->mTriggerNext.pop_back();
               return false;
            }
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
         return true;
      }
};

template<class A, class B, class C, class D, class E, class F, class G> 
class Signal<bool(A,B,C,D,E,F,G)> : public SignalBaseT<bool(A,B,C,D,E,F,G)>
{
   public:

      bool trigger( A a, B b, C c, D d, E e, F f, G g )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            if( !this->getDelegate( ptr )( a, b, c, d, e, f, g ) )
            {
               this->mTriggerNext.pop_back();
               return false;
            }
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
         return true;
      }
};

template<class A, class B, class C, class D, class E, class F, class G, class H> 
class Signal<bool(A,B,C,D,E,F,G,H)> : public SignalBaseT<bool(A,B,C,D,E,F,G,H)>
{
   public:

      bool trigger( A a, B b, C c, D d, E e, F f, G g, H h )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            if( !this->getDelegate( ptr )( a, b, c, d, e, f, g, h ) )
            {
               this->mTriggerNext.pop_back();
               return false;
            }
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
         return true;
      }
};

template<class A, class B, class C, class D, class E, class F, class G, class H, class I> 
class Signal<bool(A,B,C,D,E,F,G,H,I)> : public SignalBaseT<bool(A,B,C,D,E,F,G,H,I)>
{
   public:

      bool trigger( A a, B b, C c, D d, E e, F f, G g, H h, I i )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            if( !this->getDelegate( ptr )( a, b, c, d, e, f, g, h, i ) )
            {
               this->mTriggerNext.pop_back();
               return false;
            }
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
         return true;
      }
};

template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J> 
class Signal<bool(A,B,C,D,E,F,G,H,I,J)> : public SignalBaseT<bool(A,B,C,D,E,F,G,H,I,J)>
{
   public:

      bool trigger( A a, B b, C c, D d, E e, F f, G g, H h, I i, J j )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            if( !this->getDelegate( ptr )( a, b, c, d, e, f, g, h, i, j ) )
            {
               this->mTriggerNext.pop_back();
               return false;
            }
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
         return true;
      }
};

// Non short-circuit signal implementations

template<> 
class Signal<void()> : public SignalBaseT<void()>
{
   public:

      void trigger()
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            this->getDelegate( ptr )();
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
      }
};

template<class A>
class Signal<void(A)> : public SignalBaseT<void(A)>
{
   public:

      void trigger( A a )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;            
			this->getDelegate( ptr )( a );
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
      }
};

template<class A, class B> 
class Signal<void(A,B)> : public SignalBaseT<void(A,B)>
{
   public:

     void trigger( A a, B b )
     {
        this->mTriggerNext.push_back(NULL);
        for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
        {
           this->mTriggerNext.back() = ptr->next;
           this->getDelegate( ptr )( a, b );
           ptr = this->mTriggerNext.back();
        }
        this->mTriggerNext.pop_back();
      }
};

template<class A, class B, class C> 
class Signal<void(A,B,C)> : public SignalBaseT<void(A,B,C)>
{
   public:

      void trigger( A a, B b, C c )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            this->getDelegate( ptr )( a, b, c );
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
      }
};

template<class A, class B, class C, class D>
class Signal<void(A,B,C,D)> : public SignalBaseT<void(A,B,C,D)>
{
   public:

      void trigger( A a, B b, C c, D d )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            this->getDelegate( ptr )( a, b, c, d );
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
      }
};

template<class A, class B, class C, class D, class E> 
class Signal<void(A,B,C,D,E)> : public SignalBaseT<void(A,B,C,D,E)>
{
   public:

      void trigger( A a, B b, C c, D d, E e )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            this->getDelegate( ptr )( a, b, c, d, e );
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
      }
};

template<class A, class B, class C, class D, class E, class F> 
class Signal<void(A,B,C,D,E,F)> : public SignalBaseT<void(A,B,C,D,E,F)>
{
   public:

      void trigger( A a, B b, C c, D d, E e, F f )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            this->getDelegate( ptr )( a, b, c, d, e, f );
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
      }
};

template<class A, class B, class C, class D, class E, class F, class G> 
class Signal<void(A,B,C,D,E,F,G)> : public SignalBaseT<void(A,B,C,D,E,F,G)>
{
   public:

      void trigger( A a, B b, C c, D d, E e, F f, G g )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            this->getDelegate( ptr )( a, b, c, d, e, f, g );
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
      }
};

template<class A, class B, class C, class D, class E, class F, class G, class H> 
class Signal<void(A,B,C,D,E,F,G,H)> : public SignalBaseT<void(A,B,C,D,E,F,G,H)>
{
   public:

      void trigger( A a, B b, C c, D d, E e, F f, G g, H h )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            this->getDelegate( ptr )( a, b, c, d, e, f, g, h );
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
      }
};

template<class A, class B, class C, class D, class E, class F, class G, class H, class I> 
class Signal<void(A,B,C,D,E,F,G,H,I)> : public SignalBaseT<void(A,B,C,D,E,F,G,H,I)>
{
   public:

      void trigger( A a, B b, C c, D d, E e, F f, G g, H h, I i )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            this->getDelegate( ptr )( a, b, c, d, e, f, g, h, i );
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
      }
};

template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J> 
class Signal<void(A,B,C,D,E,F,G,H,I,J)> : public SignalBaseT<void(A,B,C,D,E,F,G,H,I,J)>
{
   public:

      void trigger( A a, B b, C c, D d, E e, F f, G g, H h, I i, J j )
      {
         this->mTriggerNext.push_back(NULL);
         for( SignalBase::DelegateLink* ptr = this->mList.next; ptr != &this->mList; )
         {
            this->mTriggerNext.back() = ptr->next;
            this->getDelegate( ptr )( a, b, c, d, e, f, g, h, i, j );
            ptr = this->mTriggerNext.back();
         }
         this->mTriggerNext.pop_back();
      }
};

#endif // _TSIGNAL_H_
