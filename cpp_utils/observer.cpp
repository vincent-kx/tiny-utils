#include <iostream>
#include <algorithm>
#include <list>

using namespace std;

class BaseObserverI
{
public:
	BaseObserverI() = default;
	virtual ~BaseObserverI() = default;
	virtual void Update() {}
};


class BaseSubject
{
protected:
	BaseSubject(std::size_t max_observer_size)
	{
		max_observer_size_ = max_observer_size;
		observers_.clear();
	}

public:
	virtual ~BaseSubject()
	{
		while(!observers_.empty())
		{
			BaseObserverI * ptmp_observer = observers_.back();
			observers_.pop_back();
			delete ptmp_observer;
			ptmp_observer = nullptr;
		}
		observers_.clear();
	}
	virtual bool AddObserver(BaseObserverI * p_observer)
	{
		if(observers_.size() >= max_observer_size_)
			return false;
		observers_.push_back(p_observer);
		return true;
	}

	virtual bool RemoveObserverIfExists(BaseObserverI * p_observer)
	{
		if(observers_.size() == 0)
			return false;
		if(std::find(observers_.begin(),observers_.end(),p_observer) == observers_.end())
			return false;
		observers_.remove(p_observer);
		return true;
	}

	virtual void RemoveObserver(BaseObserverI * p_observer)
	{
		observers_.remove(p_observer);
	}
	
	virtual bool NotifyOne(BaseObserverI * p_observer)
	{
		if(observers_.size() == 0)
			return false;
		auto p_tmp_observer = std::find(observers_.begin(),observers_.end(),p_observer);
		if(p_tmp_observer == observers_.end())
			return false;
		
		(*p_tmp_observer)->Update();
		return true;
	}

	virtual void NotifyAll()
	{
		for(auto p_observer : observers_)
			p_observer->Update();
	}

protected:
	list<BaseObserverI *> observers_;
	std::size_t max_observer_size_;
};

#if 1

class CarAccident : public BaseSubject
{
public:
	CarAccident(std::size_t max_observer_size) : BaseSubject(max_observer_size)
	{
		cout << "CarAccident happen !!!" << endl;
	}
	~CarAccident()
	{
		cout << "CarAccident settled" << endl;
	}
};

class Hospital : public BaseObserverI
{
public:
	Hospital() = default;
	~Hospital()
	{
		cout << "the wounded are well cured" << endl;
	}
public:
	void Update()
	{
		cout << "ambulance is on the way" << endl;
	}
};

class PoliceOffice : public BaseObserverI
{
public:
	PoliceOffice() = default;
	~PoliceOffice()
	{
		cout << "policemen are back" << endl;
	}

public:
	void Update()
	{
		cout << "110 policemen are on the way" << endl;
	}
};

int main(int argc, char const *argv[])
{
	CarAccident car_accident(10);
	PoliceOffice * p_police_office = new PoliceOffice;
	Hospital * p_hospital = new Hospital;
	car_accident.AddObserver(p_police_office);
	car_accident.AddObserver(p_hospital);
	car_accident.NotifyAll();
	return 0;
}
#endif