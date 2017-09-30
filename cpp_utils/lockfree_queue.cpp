#include <iostream>
#include <atomic>

using namespace std;
/*
单生产者单消费者无锁队列，不适用于多生产者多消费者
| producer owned | publisher| consumer owned|
队列分为三个部分
1.生产者拥有数据的部分，生产者生产完成尚未发布的数据
2.生产者和消费者数据的分隔点，不存储实际数据，只是一个冗余节点
3.消费者可消费的数据部分（已经被生产者发布出来的）
 */
template<class T>
class LockFreeQueue
{
public:
	LockFreeQueue()
	{
		head = new QueueNode();
		tail = new QueueNode();//始终保留一个待用节点
		head->next = tail;
		publisher = tail;//publisher前面的部分是已发布可以消费的部分，后面的部分（包含publisher）是尚未发布还不可消费的部分
		size.store(0);
	}

	~LockFreeQueue()
	{
		while(head != nullptr)
		{
			struct QueueNode * tmp = head;
			head = head->next;
			delete tmp;
		}
	}

	void Enqueue(const T & value)
	{
		tail->value = value;
		tail->next = new QueueNode();//始终保留一个待用节点
		tail = tail->next;//虽然节点已经被加入队列，但是尚未发布，消费者不可读
		publisher = tail;//将最新如队列的node发布出去，此后消费者可读取
		++size;
	}

	bool Dequeue(T & value)
	{
		if(head->next == publisher)//所有已发布的node都被消耗完了，head是冗余节点，head->next为第一个可消费节点
			return false;
		struct QueueNode * tmp = head->next;
		value = head->next->value;
		head->next = head->next->next;
		delete tmp;
		--size;
		return true;
	}

	uint32_t queue_size()
	{
		return size.load();
	}

	bool Empty()
	{
		return size.load() == 0;
	}

	bool HasUnpublished()
	{
		return publisher != tail;
	}
private:
	struct QueueNode
	{
		QueueNode() = default;
		QueueNode(const T & t) : value(t),next(nullptr) {}
		T value;
		struct QueueNode * next;
	};

	struct QueueNode * head = nullptr;
	struct QueueNode * tail = nullptr;
	struct QueueNode * publisher = nullptr;
	atomic<int> size;
};

#include <thread>
#include <vector>
#include <chrono>
#include <random>

void Produce(LockFreeQueue<int> & queue)
{
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_int_distribution<> dis(10, 20); 
    auto dice = std::bind(dis, engine);
	for(int i = 0; i < 1000; ++i)
	{
		queue.Enqueue(i);
		cout << "enqueue value:" << i << endl;
		std::this_thread::sleep_for(std::chrono::microseconds(dice()));
	}
}

void Consum(LockFreeQueue<int> & queue)
{
	int i = 0; 
	vector<int> vec;
	int value = 1000000;
	while(i < 1000)
	{
		if(queue.Dequeue(value))
		{
			++i;
			cout << "dequeue value:" << value << endl;
			// vec.push_back(value);
		}
	}

	// for(int j = 0; j < vec.size(); ++j)
	// {
	// 	cout << vec[j] << "   ";
	// 	if(j % 10 == 0)
	// 		cout << endl;
	// }
	// cout << endl;
}

int main()
{
	LockFreeQueue<int> queue;
	thread consumer(Consum,std::ref(queue));
	thread producer(Produce,std::ref(queue));
	producer.join();
	consumer.join();
	return 0;
}