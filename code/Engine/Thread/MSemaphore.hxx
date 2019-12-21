#include "MSemaphore.h"
#include <stdio.h>
#include <string>
#ifdef WIN32
#include <Windows.h>
#else
#ifdef __ANDROID__
#include <fcntl.h>
#include <linux/stat.h>
#endif
#ifdef __APPLE__
#include <sys/types.h>
#include <unistd.h>
#endif
#include <semaphore.h>
#include <errno.h>
#endif

namespace ma
{

#ifdef WIN32
class SemaphorePrivate
{

public:
	SemaphorePrivate(const char* pName, int initcount)
	{
		m_strName = pName ? pName : "";
		m_Semaphore = CreateSemaphore( NULL, initcount, 256, NULL );
	}

	~SemaphorePrivate()
	{ 
		CloseHandle(m_Semaphore);
	}

	void WaitForSignal()
	{ 
		while (1)
		{
			uint32_t result = WaitForSingleObjectEx( m_Semaphore, INFINITE, TRUE );
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
	SemaphorePrivate(const char* pName, int initcount)
	{
        m_strName = pName ? pName : "";
		
		char name[32];
		snprintf(name, 32, "%d-%d", this,  getpid());

        m_pSemaphore = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, initcount);

        if (m_pSemaphore == SEM_FAILED)
        {
            int errorCode = errno;
            printf("%d",errno);
            switch (errno) {
                case EEXIST:
                    printf ("Error open %s \n,Both O_CREAT and O_EXCL were specified in oflag, but a semaphore with this name already exists",m_strName.c_str());
                    break;
                case EACCES:
                    printf ("Error open %s \n,The semaphore exists, but the caller does not have permission to open it",m_strName.c_str());
                    break;
                case EINVAL:
                    printf ("Error open %s \n,value was greater than SEM_VALUE_MAX",m_strName.c_str());
                    break;
                case EMFILE:
                    printf ("Error open %s \n,The per-process limit on the number of open file descriptors has been reached",m_strName.c_str());
                    break;
                case ENAMETOOLONG:
                    printf ("Error open %s \n,name was too long",m_strName.c_str());
                    break;
                case ENFILE:
                    printf ("Error open %s \n,The system-wide limit on the total number of open files has been reached",m_strName.c_str());
                    break;
                case ENOENT:
                    printf ("Error open %s \n,The O_CREAT flag was not specified in oflag and no semaphore with this name exists; or, O_CREAT was specified, but name wasn't well formed",m_strName.c_str());
                    break;
                case ENOMEM:
                    printf ("Error open %s \n,Insufficient memory",m_strName.c_str());
                    break;
                default:
                    break;
            }
        }
	}

	~SemaphorePrivate()
	{
        if (m_pSemaphore == SEM_FAILED) {
            return;
        }
        
		if (sem_close(m_pSemaphore) == -1)
			printf ("Error destroy %s \n",m_strName.c_str());
        
        if (sem_unlink(m_strName.c_str()) == -1)
            printf ("Error destroy %s \n",m_strName.c_str());
	}


	void WaitForSignal()
	{
        if (m_pSemaphore == SEM_FAILED) {
            return;
        }
        
		if (sem_wait(m_pSemaphore) == -1)
		{
			printf ("Error wait on %s \n", m_strName.c_str());
		}
	}

	void Signal()
	{
        if (m_pSemaphore == SEM_FAILED) {
            return;
        }
        
		if (sem_post(m_pSemaphore) == -1)
			printf ("Error post to %s \n", m_strName.c_str());
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

Semaphore::Semaphore(const char* pName,int initcount)
{
	_private = new SemaphorePrivate(pName, initcount);
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