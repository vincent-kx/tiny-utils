#include <iostream>
#include <string>
#include <vector>
#include <inttypes.h>
#include <time.h>
using namespace std;
size_t utf8_to_charset(const std::string &input, std::vector<std::string> &output)
{
    std::string ch;
    for(size_t i = 0, len = 0; i != input.length(); i += len) {
        unsigned char byte = (unsigned)input[i];
        if(byte >= 0xFC)
            len = 6;
        else if(byte >= 0xF8)
            len = 5;
        else if(byte >= 0xF0)
            len = 4;
        else if(byte >= 0xE0)
            len = 3;
        else if(byte >= 0xC0)
            len = 2;
        else
            len = 1;
        ch = input.substr(i,len);
        output.push_back(ch);
    }
    return output.size();
}
string utf8_substr(const std::string &input,size_t pos,size_t length)
{
    string returnStr;
    size_t added_len = 0;
    size_t cur_pos = 0;
    string ch;
    if(length == 0) return returnStr;
    for(size_t i = 0, len = 0; i != input.length(); i += len) {
        unsigned char byte = (unsigned)input[i];
        if(byte >= 0xFC) len = 6 ;
        else if(byte >= 0xF8) len = 5;
        else if(byte >= 0xF0) len = 4;
        else if(byte >= 0xE0) len = 3;
        else if(byte >= 0xC0) len = 2;
        else len = 1;
        ++cur_pos;
        if(cur_pos < pos) continue;
        else {
            returnStr.append(input.substr(i,len));
            added_len++;
            if(added_len == length)
                break;
        }
    }
    return returnStr;
}
int main(int argc,char* argv[])
{
    string s = "UTF-8字符串截取，123456,这是一个测试字符串，长度需要大于17个字符";
    cout << "utf8_substr(s,0,0) ===> " << utf8_substr(s,0,0) << endl;
    cout << "utf8_substr(s,0,1) ===> " << utf8_substr(s,0,1) << endl;
    cout << "utf8_substr(s,0,17) ===> " << utf8_substr(s,0,17) << endl;
    cout << "utf8_substr(s,0,50) ===> " << utf8_substr(s,0,50) << endl;
    cout << "utf8_substr(s,10,0) ===> " << utf8_substr(s,10,0) << endl;
    cout << "utf8_substr(s,10,1) ===> " << utf8_substr(s,10,1) << endl;
    cout << "utf8_substr(s,10,10)===> " << utf8_substr(s,10,10) << endl;
    cout << "utf8_substr(s,10,30) ===> " << utf8_substr(s,10,30) << endl;
    return 0;
}