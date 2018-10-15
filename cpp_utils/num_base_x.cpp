#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdint.h>

using namespace std;

vector<char> num_convert_vec;
map<char,uint32_t> num_convert_map;

void InitNumConvertVec()
{
        int begin = '0';
        int end = '9';
        for(int i = begin; i <= end; ++i)
        {
                num_convert_vec.push_back(i);
        }
        begin = 'a';
        end = 'z';
        for(int i = begin; i <= end; ++i)
        {
                num_convert_vec.push_back(i);
        }
        for (unsigned i = 0; i < num_convert_vec.size(); ++i)
        {
                cout << num_convert_vec[i] << ' ';
        }
        cout << endl;
}

void InitNumConvertMap()
{
	for(int i = 0; i <= 9; ++i)
	{
		num_convert_map.insert(make_pair('0'+i,i));
	}
	for(int i = 10; i < 36; ++i)
	{
		num_convert_map.insert(make_pair('a'+i-10,i));
	}
	for (auto it = num_convert_map.begin(); it != num_convert_map.end(); ++it)
	{
		cout << it->first << "=" << it->second << endl;
	}
}

string Add(const string & num1,const string & num2)
{
	vector<char> vec_num1;
	vector<char> vec_num2;
	for(int i = num1.length()-1;i >= 0; --i)
	{
		vec_num1.push_back(num1.at(i));
	}
	for(int i = num2.length()-1;i >= 0; --i)
	{
		vec_num2.push_back(num2.at(i));
	}
	uint32_t tmp_sum = 0;
	vector<char> sum_num;
	uint32_t i = 0;
	for(; i < vec_num1.size() && vec_num2.size(); ++i)
	{
		uint32_t tmp_num = num_convert_map[vec_num1[i]] + num_convert_map[vec_num2[i]] + tmp_sum;
		tmp_sum = tmp_num > 35 ? 1 : 0;
		char c = tmp_num > 35 ? num_convert_vec[tmp_num%36] : num_convert_vec[tmp_num];
		sum_num.push_back(c);
		cout << vec_num1[i] << '+' << vec_num2[i] << '+' << tmp_sum << '=' << c << "->" << tmp_sum << endl;
		cout << '#' << num_convert_map[vec_num1[i]] << '+' << num_convert_map[vec_num2[i]] << '+' << tmp_sum << '=' << c << "->" << tmp_sum << endl;
	}

	if(i== vec_num2.size()-1 && i == vec_num1.size()-1)//两个数字一样长
	{
		if(tmp_sum == 1)//两个数相加进位值最多是1
		{
			sum_num.push_back('1');
		}
	}
	else if(i== vec_num1.size())
	{
		uint32_t tmp_num = num_convert_map[vec_num2[i]] + tmp_sum;
		tmp_sum = tmp_num > 35 ? 1 : 0;
		char c = tmp_num > 35 ? num_convert_vec[tmp_num%36] : num_convert_vec[tmp_num];
		sum_num.push_back(c);
		cout << vec_num2[i] << '+' << tmp_sum << '=' << c << "->" << tmp_sum << endl;
		if(tmp_sum == 1)//两个数相加进位值最多是1
		{
			sum_num.push_back('1');
		}
	}
	else
	{
		uint32_t tmp_num = num_convert_map[vec_num1[i]] + tmp_sum;
		tmp_sum = tmp_num > 35 ? 1 : 0;
		char c = tmp_num > 35 ? num_convert_vec[tmp_num%36] : num_convert_vec[tmp_num];
		sum_num.push_back(c);
		cout << vec_num1[i] << '+' << tmp_sum << '=' << c << "->" << tmp_sum << endl;
		if(tmp_sum == 1)//两个数相加进位值最多是1
		{
			sum_num.push_back('1');
		}
	}
	for(auto it = sum_num.rbegin(); it != sum_num.rend(); ++it)
	{
		cout << *it;
	}
	cout << endl;
	return string(sum_num.rbegin(),sum_num.rend());
}

int main(int argc, char const *argv[])
{
	InitNumConvertVec();
	InitNumConvertMap();
	string num1 = "zxyzxyz";
	string num2 = "xyzxyz";
	cout << num1 << '+' << num2 << '=' << Add(num1,num2) << endl;
	return 0;
}