#include "stdafx.h" 
#include "pubhead.h"
// string转为char数组  。。坑爹的C++
char* str2ch(string src)
{
	char *dst = new char[255];
	int i;
	for (i = 0; i <= src.length(); i++)
		dst[i] = src[i];
	dst[i] = '\0';
	return dst;
}
// string 转为LPCWSTR  。。
LPCWSTR str2lpcw(string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	std::wstring wstrResult(wcstring);
	free(wcstring);
	return wstrResult.c_str();
}
bool is_asc(string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i]<1||str[i]>255)
			return false;
	}
	return true;
}
