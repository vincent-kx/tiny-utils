#include <iostream>
#include <atomic>

using namespace std;

static atomic_flag lock_ = ATOMIC_FLAG_INIT;

template<typename T>
class Singleton
{
public:
	template<typename ... Args>
	static T * Instance(Args && ...args)
	{
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
	for(int i = 0; i < 1000; ++i)
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