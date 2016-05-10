#include <iostream>
#include <string>
#include <time.h>
#include <memory.h>

using namespace std;

time_t StrDate2Time(const string date,const string format="%Y-%m-%d %H:%M:%S")
{
    struct tm tm;
    strptime(date.c_str(),format.c_str(), &tm) ;
    time_t ft=mktime(&tm);
    return ft;
}

string Time2DateStr(const time_t t,const string format="%Y-%m-%d %H:%M:%S")
{
    uint32_t buf_len = 24;
    char buf[buf_len];
    memset(buf,0,buf_len);
    strftime(buf,buf_len,"%Y-%m-%d %H:%M:%S",localtime(&t));
    return string(buf);
}

string GetLastDayBeginTimeStr()
{
    string begin_time = " 00:00:00";
    time_t now = time(NULL);
    time_t lastDayNow = now - 24 * 60 * 60;
    struct tm *pTime = localtime(&lastDayNow);
    uint32_t buflen = 32;
    char str_time[buflen];
    memset(str_time,0,buflen);
    strftime(str_time,buflen,"%Y-%m-%d",pTime);
    return string(str_time) + begin_time;
}

time_t GetTodayXColockTime(int xColock)
{
    if(xColock < 0 || xColock > 24)
    {
        return 0;
    }
    time_t nowTime = time(NULL);
    struct tm *pTime = localtime(&nowTime);
    pTime->tm_hour = xColock;
    pTime->tm_min = 0;
    pTime->tm_sec = 0;
    return mktime(pTime);
}

int main()
{
    string strTime = "2016-05-30 12:11:10";
    time_t timestamp = StrDate2Time(strTime);
    cout << timestamp << endl;
    cout << Time2DateStr(timestamp) << endl;
    return 0;
}
