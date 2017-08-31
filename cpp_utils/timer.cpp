#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Timer
{
public:
	Timer() : clock_begin_(high_resolution_clock::now()){};
	~Timer() = default;

	void reset() { clock_begin_ = high_resolution_clock::now(); }

	template<typename Duration = milliseconds>
	unsigned long elapsed() const
	{
		return duration_cast<Duration>(high_resolution_clock::now() - clock_begin_).count();
	}

	unsigned long elapsed_hours()
	{
		return elapsed<std::chrono::hours>();
	}

	unsigned long elapsed_minutes()
	{
		return elapsed<std::chrono::minutes>();
	}

	unsigned long elapsed_seconds()
	{
		return elapsed<std::chrono::seconds>();
	}

	unsigned long elapsed_milliseconds()
	{
		return elapsed<std::chrono::milliseconds>();
	}

	unsigned long elapsed_microseconds()
	{
		return elapsed<std::chrono::microseconds>();
	}

	unsigned long elapsed_nanoseconds()
	{
		return elapsed<std::chrono::nanoseconds>();
	}
private:
	time_point<high_resolution_clock> clock_begin_;
};

#if 1
void func()
{
	for(int i = 0; i < 1000000; ++i);
}

int main(int argc, char const *argv[])
{
	Timer timer;
	func();
	cout << "elapsed nanoseconds:" << timer.elapsed_nanoseconds() << endl;
	cout << "elapsed microseconds:" << timer.elapsed_microseconds() << endl;
	cout << "elapsed milliseconds:" << timer.elapsed_milliseconds() << endl;
	return 0;
}
#endif