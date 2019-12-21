#pragma once

namespace ma
{

class SemaphorePrivate;

class Semaphore 
{
public:
	Semaphore(const char* pName, int initcount = 0);
	~Semaphore();

	void WaitForSignal(); 
	void Signal();

private:
	SemaphorePrivate* _private; 
};

}


