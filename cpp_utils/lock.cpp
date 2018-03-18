#include <iostream>

#if defined (__cplusplus) && (__cplusplus > 201103L)
	#define _CPP11_
#endif

#ifdef _CPP11_
	#include <atomic>
	#include <mutex>
#else
	#include <pthread.h>
	#include <memory.h>
#endif

class ZSpinLock
{
#ifdef _CPP11_
public:
	ZSpinLock() = default;
	ZSpinLock(const ZSpinLock &) = delete;
	ZSpinLock(ZSpinLock &&) = delete;
	ZSpinLock & operator=(ZSpinLock &) = delete;
	ZSpinLock & operator=(ZSpinLock &&) = delete;
	~ZSpinLock() = default;

public:
	bool lock() noexcept//always return true,should block
	{ 
		while(lock_.test_and_set(std::memory_order_acquire)) 
			return true; 
	}
	bool try_lock() noexcept
	{
		int times = 100;
		while(times > 0)
		{
			if(lock_.test_and_set(std::memory_order_acquire))
				--times;
			else return true;
		}
		return false;
	}
	bool unlock() noexcept { return lock_.clear(ste::memory_order_release); }
private:
		atomic_flag lock_ = ATOMIC_FLAG_INIT;
#else
public:
	ZSpinLock()
	{
		// memset(&lock_,0,sizeof(lock_));
		pthread_spin_init(&lock_,0);
	}
	~ZSpinLock(){ pthread_spin_destroy(&lock_);}
	bool lock() { return pthread_spin_lock(&lock_) == 0;}
	bool try_lock() {return pthread_spin_trylock(&lock_) == 0;}
	bool unlock() { return pthread_spin_unlock(&lock_) == 0;}
private:
	ZSpinLock(const ZSpinLock &) {}
	ZSpinLock & operator=(const ZSpinLock &){}
private:
	pthread_spinlock_t lock_;
	
#endif	
};

class ZMutexLock
{
#ifdef _CPP11_
public:
	ZMutexLock() = default;
	~ZMutexLock() = default;
	bool lock() { mutex_.lock(); return true; }//always return true,should block
	bool try_lock() { return mutex_.try_lock();}
	bool unlock() { mutex_.unlock(); return true;}
private:
	ZMutexLock(const ZMutexLock &) = delete;
	ZMutexLock(ZMutexLock &&) = delete;
	ZMutexLock & operator=(const ZMutexLock &) = delete;
	ZMutexLock & operator=(ZMutexLock &&) = delete;

private:
	std::mutex mutex_;
#else
public:
	ZMutexLock()
	{
		memset(&mutex_,0,sizeof(mutex_));
		pthread_mutex_init(&mutex_,NULL);
	}

	~ZMutexLock() { pthread_mutex_destroy(&mutex_); }

    bool lock() { return pthread_mutex_lock(&mutex_) == 0; }
    bool try_lock() { return pthread_mutex_trylock(&mutex_) == 0;}
    bool unlock() { return pthread_mutex_unlock(&mutex_) == 0; }
private:
	ZMutexLock(const ZMutexLock &) {}
	ZMutexLock & operator=(const ZMutexLock &){}
private:
	pthread_mutex_t mutex_;
#endif
};


#if 1
#include <thread>
using namespace std;

static int count = 0;

void test_spin(ZSpinLock * lock)
{
	for(int i = 0; i < 1000000; ++i)
	{
		lock->lock();
		++count;
		lock->unlock();
	}
}

void test_mutex(ZMutexLock * lock)
{
	for(int i = 0; i < 1000000; ++i)
	{
		lock->lock();
		++count;
		lock->unlock();
	}
}

int main(int argc, char const *argv[])
{
	// ZSpinLock lock;
	ZMutexLock lock;
	thread t1(test_mutex,&lock);
	thread t2(test_mutex,&lock);
	t1.join();
	t2.join();
	cout << count << endl;	
	return 0;
}
#endif