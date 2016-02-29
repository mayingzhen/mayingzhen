#pragma once

#ifdef WIN32
#include <intrin.h>
#elif __APPLE__
#include <libkern/OSAtomic.h>
#elif __ANDROID__ 
// use gcc builtin __sync_*
#endif



// AtomicCompareExchange - Returns value is the initial value of the Destination pointer (as defined by _InterlockedCompareExchange)
#ifdef WIN32
inline bool AtomicCompareExchange (int volatile* i, int newValue, int expectedValue) {
	return _InterlockedCompareExchange ((long volatile*)i, (long)newValue, (long)expectedValue) == expectedValue;
}
#elif __APPLE__
inline bool AtomicCompareExchange (int volatile* i, int newValue, int expectedValue) {
	return OSAtomicCompareAndSwap32Barrier (expectedValue, newValue, reinterpret_cast<volatile int32_t*>(i));
}
#elif __ANDROID__
inline bool AtomicCompareExchange (int volatile* i, int newValue, int expectedValue) {
	return __sync_bool_compare_and_swap(i, expectedValue, newValue);
}
#endif


// AtomicAdd - Returns the new value, after the operation has been performed (as defined by OSAtomicAdd32Barrier)
inline int AtomicAdd (int volatile* i, int value) {
#ifdef __APPLE__
	return OSAtomicAdd32Barrier (value, (int*)i);
#elif WIN32
	return _InterlockedExchangeAdd ((long volatile*)i, value) + value;
#elif __ANDROID__
	return __sync_add_and_fetch(i,value);
#else
#error "Atomic op undefined for this platform"
#endif
}

// AtomicSub - Returns the new value, after the operation has been performed (as defined by OSAtomicSub32Barrier)
inline int AtomicSub (int volatile* i, int value) {

#ifdef __ANDROID__
	return __sync_sub_and_fetch(i,value);
#else
	return AtomicAdd(i, -value);
#endif
}

// AtomicIncrement - Returns the new value, after the operation has been performed (as defined by OSAtomicAdd32Barrier)
inline int AtomicIncrement (int volatile* i) {
#ifdef WIN32
	return _InterlockedIncrement ((long volatile*)i);
#else
	return AtomicAdd(i, 1);
#endif
}

inline int AtomicDecrement (int volatile* i) {
#ifdef WIN32
	return _InterlockedDecrement ((long volatile*)i);
#else
	return AtomicSub(i, 1);
#endif
}

// Memory barrier.
//
// Necessary to call this when using volatile to order writes/reads in multiple threads.
inline void UnityMemoryBarrier()
{
#ifdef WIN32
	#ifdef MemoryBarrier
	MemoryBarrier();
	#else
	long temp;
	__asm xchg temp,eax;
	#endif
	
#elif __APPLE__
	OSMemoryBarrier();
#endif
}
