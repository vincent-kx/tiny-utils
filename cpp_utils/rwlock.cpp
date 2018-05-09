
#include <iostream>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

using namespace std;

enum RWLock_ErrCode
{
	OK = 0,
	EC_LOCK_INVALID = 1,
	EC_LOCK_USING = 2,
	EC_LOCK_TIMEOUT = 3,
	EC_LOCK_BAD_STAT = 4,
	EC_LOCK_BUSY = -1
};

class ReadWriteLock
{
public:
	ReadWriteLock()
	{
		// mutex_ = PTHREAD_MUTEX_INITIALIZER;
		// reader_cond_ = PTHREAD_COND_INITIALIZER;
		// writer_cond_ = PTHREAD_COND_INITIALIZER;

		ref_count_ = 0; // < 0:writing,>0:reading
		reader_waiting_num_ = 0;
		writer_waiting_num_ = 0;
		valid_ = 1;
	}

public:
	int init()
	{
		if(0 != pthread_mutex_init(&mutex_,NULL))
		{
			pthread_mutex_destroy(&mutex_);
			return -1;
		}
		if(0 != pthread_cond_init(&reader_cond_,NULL))
		{
			pthread_cond_destroy(&reader_cond_);
			return -1;
		}
		if(0 != pthread_cond_init(&writer_cond_,NULL))
		{
			pthread_cond_destroy(&writer_cond_);
			return -1;
		}
		return 0;
	}
	int destroy()
	{
		if(!valid_)
			return EC_LOCK_INVALID;
		if(ref_count_ != 0 || reader_waiting_num_ != 0 || writer_waiting_num_ != 0)
			return EC_LOCK_USING;
		pthread_mutex_destroy(&mutex_);
		pthread_cond_destroy(&reader_cond_);
		pthread_cond_destroy(&writer_cond_);
		valid_ = 0;
		return OK;
	}

	int read_lock()
	{
		int ret = OK;
		if(!valid_) 
			return EC_LOCK_INVALID;

		if(OK != pthread_mutex_lock(&mutex_)) 
			return EC_LOCK_BUSY;

		while(ref_count_ < 0/* || writer_waiting_num_ > 0*/)
		{
			++reader_waiting_num_;
			//如果线程被其他线程取消了，则需要执行清理，否则锁的状态就不对了
			pthread_cleanup_push(cancel_read_lock_wait,this);
			ret = pthread_cond_wait(&reader_cond_,&mutex_);//shall block
			pthread_cleanup_pop(0);//wait正常结束不需要执行清理函数
			--reader_waiting_num_;
			if(OK != ret) 
			{
				ret = EC_LOCK_BUSY;
				break;
			}
		}

		if(OK == ret) ++ref_count_;
		pthread_mutex_unlock(&mutex_);
		return ret;
	}

	int timed_read_lock(const struct timespec * timewait)
	{
		int ret = OK;
		if(!valid_) 
			return EC_LOCK_INVALID;

		if(OK != pthread_mutex_lock(&mutex_)) 
			return EC_LOCK_BUSY;

		while(ref_count_ < 0/* || writer_waiting_num_ > 0*/)
		{
			++reader_waiting_num_;
			ret = pthread_cond_timedwait(&reader_cond_,&mutex_,timewait);//shall block
			--reader_waiting_num_;
			if(OK != ret)
			{
				ret = EC_LOCK_BUSY;
				break;
			}
		}

		if(OK == ret) ++ref_count_;
		pthread_mutex_unlock(&mutex_);
		return ret;
	}

	int try_read_lock()
	{
		int ret = OK;
		if(!valid_)
			return EC_LOCK_INVALID;

		if(OK != pthread_mutex_lock(&mutex_)) 
			return EC_LOCK_BUSY;
		if(ref_count_ < 0/* || writer_waiting_num_ > 0*/)
			ret = EC_LOCK_BUSY;
		else
			++ref_count_;
		pthread_mutex_unlock(&mutex_);
		return ret;
	}

	int write_lock()
	{
		int ret = OK;
		if(!valid_) 
			return EC_LOCK_INVALID;

		if(OK != pthread_mutex_lock(&mutex_)) 
			return EC_LOCK_BUSY;
		while(ref_count_ != 0)
		{
			++writer_waiting_num_;
			pthread_cleanup_push(cancel_write_lock_wait,this);
			ret = pthread_cond_wait(&writer_cond_,&mutex_);
			pthread_cleanup_pop(0);
			--writer_waiting_num_;
			if(OK != ret)
			{
				ret = EC_LOCK_BUSY;
				break;
			}
		}
		if(OK == ret)
			--ref_count_;
		pthread_mutex_unlock(&mutex_);
		return ret;
	}

	int try_write_lock()
	{
		int ret = 0;
		if(!valid_) 
			return EC_LOCK_INVALID;

		if(OK != pthread_mutex_lock(&mutex_)) 
			return EC_LOCK_BUSY;
		if(ref_count_ != 0)
			ret = EC_LOCK_BUSY;
		else
			ref_count_ = -1;
		pthread_mutex_unlock(&mutex_);
		return ret;
	}

	int timed_write_wait(const struct timespec * timewait)
	{
		int ret = OK;
		if(!valid_) 
			return EC_LOCK_INVALID;

		if(OK != pthread_mutex_lock(&mutex_)) 
			return EC_LOCK_BUSY;
		while(ref_count_ != 0)
		{
			++writer_waiting_num_;
			ret = pthread_cond_timedwait(&writer_cond_,&mutex_,timewait);
			--writer_waiting_num_;
			if(OK != ret)
			{
				ret = EC_LOCK_BUSY;
				break;
			}
		}
		if(OK == ret)
			--ref_count_;
		pthread_mutex_unlock(&mutex_);
		return ret;
	}

	int unlock()
	{
		int ret = OK;
		if(!valid_) 
			return EC_LOCK_INVALID;

		if(0 != pthread_mutex_lock(&mutex_))
			ret = EC_LOCK_BUSY;
		
		if(ref_count_ > 0)
			--ref_count_;
		else if(ref_count_ < 0)
			ref_count_ = 0;
		else
			ret = EC_LOCK_BAD_STAT;

		if(writer_waiting_num_ > 0 && ref_count_ == 0)
		{
			if(OK != pthread_cond_signal(&writer_cond_))
				ret = EC_LOCK_INVALID;
		}
		else if(reader_waiting_num_ > 0)
		{
			if(OK != pthread_cond_broadcast(&reader_cond_))
				ret = EC_LOCK_INVALID;
		}
		else
			ret = EC_LOCK_BAD_STAT;
		pthread_mutex_unlock(&mutex_);
		return ret;
	}

private:
	static void cancel_read_lock_wait(void * args)
	{
		ReadWriteLock * rw_lock = (ReadWriteLock *)args;
		--rw_lock->reader_waiting_num_;
		pthread_mutex_unlock(&rw_lock->mutex_);
	}

	static void cancel_write_lock_wait(void * args)
	{
		ReadWriteLock * rw_lock = (ReadWriteLock *)args;
		--rw_lock->writer_waiting_num_;
		pthread_mutex_unlock(&rw_lock->mutex_);
	}

private:
	pthread_mutex_t mutex_;
	pthread_cond_t reader_cond_;
	pthread_cond_t writer_cond_;
	int ref_count_;
	uint32_t reader_waiting_num_;
	uint32_t writer_waiting_num_;
	uint32_t valid_;
};

#include <unistd.h>

void * GetReadLock(void * args)
{
	ReadWriteLock * rw_lock = (ReadWriteLock *) args;
	rw_lock->read_lock();
	cout << "read locking " << pthread_self() << endl;
	sleep(5);
	rw_lock->unlock();
	cout << "read unlock " << pthread_self() << endl;
}

void * GetWriteLock(void * args)
{
	ReadWriteLock * rw_lock = (ReadWriteLock *) args;
	rw_lock->write_lock();
	cout << "write locking " << pthread_self() << endl;
	sleep(3);
	rw_lock->unlock();
	cout << "write unlock " << pthread_self() << endl;
}

int main(int argc, char const *argv[])
{
	ReadWriteLock rw_lock;
	rw_lock.init();

	pthread_t t1;
	pthread_t t2;
	pthread_t t3;

	pthread_create(&t1,NULL,GetReadLock,(void *)&rw_lock);
	pthread_create(&t3,NULL,GetReadLock,(void *)&rw_lock);
	pthread_create(&t2,NULL,GetWriteLock,(void *)&rw_lock);
	// pthread_join(t1,NULL);
	// pthread_join(t2,NULL);
	// pthread_join(t3,NULL);

	sleep(10);
	cout << rw_lock.destroy() << endl;
	return 0;
}