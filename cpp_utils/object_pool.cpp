
#include <iostream>
#include <queue>

using namespace std;

#if defined(__cplusplus) && (__cplusplus >= 201103L)

#include <memory>
#include <atomic>
#include <functional>
#include <mutex>


template<typename T>
class ObjectPool
{
public:
	using ReleaserType = std::function<void (T*)>;
	ObjectPool() = default;
	~ObjectPool() = default;

	template<typename ... Args>
	bool init(std::size_t init_pool_size,std::size_t max_pool_size,Args && ... args)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if(inited_) return true;

		if(init_pool_size > max_pool_size_)
			return false;

		for(std::size_t i = 0; i < init_pool_size; ++i)
		{
			unique_ptr<T> pObject(new T(std::forward<Args>(args)...));
			pool_.push(std::move(pObject));
		}
		inited_ = true;
		return true;
	}

	unique_ptr<T,ReleaserType> take_one()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if(pool_.size() == 0)
			return nullptr;
		std::unique_ptr<T,ReleaserType> pobj(
			pool_.front().release(),//should use raw pointer
			[this](T * t) { 
				pool_.push(unique_ptr<T>(t)); 
				cout << "object returned !" << endl;
			}
		);
		pool_.pop();
		return std::move(pobj);
	}

	bool add_one(unique_ptr<T> pObject)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if(pool_.size() >= max_pool_size_)
			return false;

		pool_.push(std::move(pObject));
		return true;
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return pool_.empty();
	}

	std::size_t size()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return pool_.size();
	}
	ObjectPool(const ObjectPool &) = delete;
	ObjectPool(const ObjectPool &&) = delete;
	ObjectPool & operator=(const ObjectPool &) = delete;

private:
	std::queue<unique_ptr<T>> pool_;
	std::size_t max_pool_size_ = 10;
	std::mutex mutex_;
	bool inited_ = false;
};

#else

#include <pthread.>


template<typename T>
class ObjectPool
{
public:
	ObjectPool(std::size_t max_pool_size)
	{
		max_pool_size_ = max_pool_size_;
		memset(&mutex_,0,sizeof(mutex_));
		pthread_mutex_init(&mutex_,NULL);
	}

	~ObjectPool() { pthread_mutex_destroy(&mutex_); }
	bool add_one(T * pt)
	{
		LockGuard lock(&mutex_);
		if(pool_.size() >= max_pool_size_)
			return false;
		pool_.push_back(pt);
		return true;
	}
	
	T * take_one()
	{
		LockGuard lock(&mutex_);
		if(pool_.empty())
			return NULL;
		T * tmp_ptr = pool_.pop_front();
		return tmp_ptr;
	}

	bool return_one(T * pt)
	{
		LockGuard lock(&mutex_);
		if(pool_.size() >= max_pool_size_)
			return false;
		pool_.push_back(pt);
		return true;
	}

private:
	ObjectPool(const ObjectPool &){}
	ObjectPool(const ObjectPool &&){}
	ObjectPool & operator=(const ObjectPool &){}

private:
	std::size_t max_pool_size_;
	pthread_mutex_t mutex_;
	std::queue<T*> pool_;


private:

class LockGuard
{
public:
	LockGuard(pthread_mutex_t * pmutex)
	{
		pmutex_ = pmutex;
		pthread_mutex_lock(pmutex_);
	}
	~LockGuard()
	{
		pthread_mutex_unlock(pmutex_);
		pmutex_ = NULL;
	}
private:
	pthread_mutex_t * pmutex_;
};

};

#endif

#if 1

#include <thread>

class Test
{
public:
	Test(int i, double d) : ii(i),dd(d) {}
	~Test(){}
	void print(){ cout << this << endl; }
private:
	int ii;
	double dd;
};

void func(ObjectPool<Test> * ptest_pool)
{
	auto pt1 = ptest_pool->take_one();
	auto pt2 = ptest_pool->take_one();
	pt1->print();
	pt2->print();
}

int main(int argc, char const *argv[])
{
	
	ObjectPool<Test> test_pool;
	test_pool.init(10,20,1,1.111);
	cout << "pool_size:" << test_pool.size() << endl;
	thread t1(func,&test_pool);
	thread t2(func,&test_pool);
	thread t3(func,&test_pool);

	t1.join();
	t2.join();
	t3.join();

	cout << "pool_size:" << test_pool.size() << endl;
	return 0;
}
#endif