#pragma once

namespace ma
{

class SemaphorePrivate;

class Semaphore 
{
public:
	Semaphore(const char* pName);
	~Semaphore();

	void WaitForSignal(); 
	void Signal();

private:
	SemaphorePrivate* _private; 
};

}


