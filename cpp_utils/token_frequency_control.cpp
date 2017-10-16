#include <iostream>
#include <time.h>

class TokenFrequencyCtrl
{
public:
	TokenFrequencyCtrl(int capacity,int token_rate) :capacity_(capacity),token_rate_(token_rate),token_num_(0)
	{
		gettimeofday(last_access_time_,NULL);
	}
	~TokenFrequencyCtrl();
	bool Grant()
	{
		struct timeval tv;
		gettimeofday(&tv,NULL);
		//计算上一次请求到现在的间隔时长
		int time_elapse = 1000 * (tv.tv_sec - last_access_time_.tv_sec) + (tv.tv_usec - last_access_time_.tv_usec)/1000;
		//这一段流逝的时间内桶内应该积累多少令牌
		int tmp_tokens = token_num_ + token_rate_ * time_elapse;
		//如果积累的令牌数量超过容量，则当前令牌数量为桶的容量
		token_num_ = ( capacity_ > tmp_tokens ? tmp_tokens : capacity_);
		
		//设置最近一次请求的时间
		last_access_time_.tv_sec = tv.tv_sec;
		last_access_time_.tv_usec = tv.tv_usec;

		//如果桶内当前没有令牌了则拒绝访问
		if(token_num_ < 1)
			return false;
		else//否则给请求发一个通过令牌，允许访问
		{
			token_num_--;
			return true;
		}
	}

public:

private:
	int token_num_;			//当前桶内令牌数量
	int token_rate_;		//往桶里放令牌的速度，单位个/ms
	int capacity_;			//桶的容量
	struct timeval last_access_time_;
};