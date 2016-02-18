#pragma once

namespace ma
{

class SemaphorePrivate;

class Semaphore 
{
public:
	Semaphore();
	~Semaphore();

	void Reset();
	void WaitForSignal(); 
	void Signal();

private:
	SemaphorePrivate* _private; 
};

}


