#ifndef __EDU_CLOCK_H_
#define __EDU_CLOCK_H_

#include <pthread.h>
#include <iostream>

class CReadWriteLocker
{
private:
    pthread_rwlock_t m_sect;

    CReadWriteLocker(const CReadWriteLocker&);
    CReadWriteLocker& operator=(const CReadWriteLocker&);
public:
	CReadWriteLocker()          { std::cout<<"pthread_rwlock_init"<<std::endl;::pthread_rwlock_init(&m_sect, NULL); }
    ~CReadWriteLocker()         { std::cout<<"pthread_rwlock_destroy"<<std::endl;::pthread_rwlock_destroy(&m_sect); }


    bool readLock()             { return 0 == ::pthread_rwlock_rdlock(&m_sect); }
    bool writeLock()            { return 0 == ::pthread_rwlock_wrlock(&m_sect); }
    bool tryReadLock()          { return 0 == ::pthread_rwlock_tryrdlock(&m_sect); }
    bool tryWriteLock()         { return 0 == ::pthread_rwlock_trywrlock(&m_sect); }
    bool unlock()               { return 0 == ::pthread_rwlock_unlock(&m_sect); }
};

template < typename ReadWriteLockerT >
class ReadWriteLockerRL
{
	ReadWriteLockerT& m_cs;

	ReadWriteLockerRL(const ReadWriteLockerRL&);
	ReadWriteLockerRL& operator=(const ReadWriteLockerRL&);
public:
	ReadWriteLockerRL(ReadWriteLockerT& cs)
		: m_cs(cs)
	{
		m_cs.readLock();
	}

	~ReadWriteLockerRL()
	{
		m_cs.unlock();
	}
};

template < typename ReadWriteLockerT >
class ReadWriteLockerWL
{
	ReadWriteLockerT& m_cs;

	ReadWriteLockerWL(const ReadWriteLockerWL&);
	ReadWriteLockerWL& operator=(const ReadWriteLockerWL&);
public:
	ReadWriteLockerWL(ReadWriteLockerT& cs)
		: m_cs(cs)
	{
		m_cs.writeLock();
	}

	~ReadWriteLockerWL()
	{
		m_cs.unlock();
	}
};

#endif



