#include <iostream>
#include "cppversion_macro.h"

using namespace std;

#if(SUPPORT_CPP11)
	#include <atomic>

	static atomic_flag lock_ = ATOMIC_FLAG_INIT;

	template<typename T>
	class Singleton
	{
	public:
		template<typename ... Args>
		static T * Instance(Args && ...args)
		{
			static SingletonCleaner singleton_cleaner;
			if(instance_ != nullptr)
				return instance_;
			else
			{
				while(lock_.test_and_set());//spinlock
				if(instance_ == nullptr)
					instance_ = new T(std::forward<Args>(args)...);
				lock_.clear();
			}
			return instance_;
		}

	private:
		Singleton() = default;
		Singleton(const Singleton &) = delete;
		Singleton(Singleton && s);
		Singleton & operator=(const Singleton &) = delete;
		~Singleton(){}
	private:
		static T * instance_;

	private:
	    class SingletonCleaner
	    {
	    public:
	        ~SingletonCleaner()
	        {
	            if (NULL != instance_) delete instance_;
	            instance_ = NULL;
	            cout << "Instance destroied" << endl;
	        }
	    };
	};

	template<typename T>
	T * Singleton<T>::instance_ = nullptr;

	#if 1

		#include <thread>

		class Test
		{
		public:
			Test(int ii,double dd):i(ii),d(dd){ cout << "Test constructor" << endl; }
			~Test(){}
		private:
			int i;
			double d;
		};

		void test_func()
		{
			for(int i = 0; i < 100; ++i)
			{
				Test * t = Singleton<Test>::Instance(1,1.1);
				cout << this_thread::get_id() << "--" << t << endl;
			}
		}

		int main(int argc, char const *argv[])
		{
			thread t1(test_func);
			thread t2(test_func);
			thread t3(test_func);
			t1.join();
			t2.join();
			t3.join();
			return 0;
		}

	#endif

#else
	#include <pthread.h>

	template<typename T>
	class Singleton
	{
	public:
		static T * Instance()
		{
			static SingletonCleaner singleton_cleaner;
			pthread_once(&once,&Singleton::InstanceInit);
			return instance_;
		}
	private:
		Singleton(){}
		~Singleton(){}
		Singleton(const Singleton &){}
		Singleton & operator=(const Singleton &){}

		static void InstanceInit()
		{
			instance_ = new T();
		}
	private:
		static T * instance_;
		static pthread_once_t once;

	private:
	    class SingletonCleaner
	    {
	    public:
	        ~SingletonCleaner()
	        {
	            if (NULL != instance_)
	            {
	            	delete instance_;
	            	instance_ = NULL;
	            	cout << "Instance destroied" << endl;
	            }
	        }
	    };
	};

	template<typename T>T * Singleton<T>::instance_ = NULL;
	template<typename T>pthread_once_t Singleton<T>::once = PTHREAD_ONCE_INIT;	

	#if 1
		class Test{
		public:
			Test(){ cout << "test obj constructor" << endl; }
		};

		void * test_func(void * tid)
		{
			for(int i = 0; i < 100; ++i)
			{
				Test * t = Singleton<Test>::Instance();
				cout << "thread id:" << (long)pthread_self() <<" test obj addr:" << t << endl;
			}
		}

		int main(int argc, char const *argv[])
		{
			pthread_t thread1;
			pthread_t thread2;
			pthread_t thread3;
			pthread_create(&thread1,NULL,test_func,NULL);
			pthread_create(&thread2,NULL,test_func,NULL);
			pthread_create(&thread3,NULL,test_func,NULL);
			pthread_join(thread1,NULL);
			pthread_join(thread2,NULL);
			pthread_join(thread3,NULL);
			return 0;
		}
	#endif
#endif
