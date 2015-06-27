//定义一些公共的头文件

#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cctype>
#include "windows.h"
using namespace std;
using PAIR = pair < string, double >;
#ifndef _ARTICLE_H_     //如果没有定义这个宏
#define _ARTICLE_H_     //定义这个宏
struct Feature
{
	int count;
	double weight;
};
#endif
LPCWSTR str2lpcw(string orig);
char* str2ch(string src);
bool is_asc(string str);
