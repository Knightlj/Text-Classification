//����һЩ������ͷ�ļ�

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
#ifndef _ARTICLE_H_     //���û�ж��������
#define _ARTICLE_H_     //���������
struct Feature
{
	int count;
	double weight;
};
#endif
LPCWSTR str2lpcw(string orig);
char* str2ch(string src);
bool is_asc(string str);
