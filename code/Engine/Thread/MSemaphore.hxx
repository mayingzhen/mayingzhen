#include "MSemaphore.h"
#include <stdio.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <semaphore.h>
#include <errno.h>
#endif

namespace ma
{

#ifdef WIN32
class SemaphorePrivate
{

public:
	SemaphorePrivate(const char* pName)
	{
		m_strName = pName ? pName : "";
		m_Semaphore = CreateSemaphore( NULL, 0, 256, NULL );
	}

	~SemaphorePrivate()
	{ 
		CloseHandle(m_Semaphore);
	}

	void WaitForSignal()
	{ 
		while (1)
		{
			DWORD result = WaitForSingleObjectEx( m_Semaphore, INFINITE, TRUE );
			switch (result)
			{
			case WAIT_OBJECT_0:
				// We got the signal
				return;
			case WAIT_IO_COMPLETION:
				// Allow thread to run IO completion task
				Sleep(1);
				break;
			default:
				ASSERT(false);
				break;
			}
		}
	}

	void Signal()
	{
		ReleaseSemaphore( m_Semaphore, 1, NULL ); 
	}

private:
	HANDLE m_Semaphore;

	std::string m_strName;
};
#elif __APPLE__
class SemaphorePrivate
{

public:
	SemaphorePrivate(const char* pName)
	{
        m_strName = pName ? pName : "";
        m_pSemaphore = sem_open(pName, O_CREAT, S_IRUSR | S_IWUSR, 0);
        ASSERT(m_pSemaphore);
        if (m_pSemaphore == NULL)
        {
            printf ("Error open \n");
        }
	}

	~SemaphorePrivate()
	{
		if (sem_close(m_pSemaphore) == -1)
			printf ("Error destroy \n");
        
        if (sem_unlink(m_strName.c_str()) == -1)
            printf ("Error destroy \n");
	}


	void WaitForSignal()
	{
		if (sem_wait(m_pSemaphore) == -1)
			printf ("Error wait on \n");
	}

	void Signal()
	{
		if (sem_post(m_pSemaphore) == -1)
			printf ("Error post to \n");
	}


private:
	sem_t* m_pSemaphore;
    
    std::string m_strName;
};
#elif __ANDROID__
class SemaphorePrivate
{

public:
	SemaphorePrivate(const char* pName)
	{
		m_strName = pName ? pName : "";
		if (sem_init(&m_Semaphore, 0, 0) == -1) 
			printf ("Error open \n"); 
	}

	~SemaphorePrivate()
	{
		if (sem_destroy(&m_Semaphore) == -1) 
			printf ("Error destroy \n"); 
	}


	void WaitForSignal()
	{
		if (sem_wait(&m_Semaphore) == -1)
			printf ("Error wait on \n");
	}

	void Signal()
	{
		if (sem_post(&m_Semaphore) == -1)
			printf ("Error post to \n");
	}


private:
	sem_t m_Semaphore;

	std::string m_strName;
};
#endif

Semaphore::Semaphore(const char* pName)
{
	_private = new SemaphorePrivate(pName);
}

Semaphore::~Semaphore()
{
	SAFE_DELETE(_private);
}

void Semaphore::WaitForSignal()
{
	_private->WaitForSignal();
}

void Semaphore::Signal()
{
	_private->Signal();
}

}