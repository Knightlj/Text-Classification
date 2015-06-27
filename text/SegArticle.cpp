#include "stdafx.h" 
#include "SegArticle.h"
SegArticle::SegArticle()
{
	GetStopWords();
}
//递归遍历所有的文件
void SegArticle::Diclist(string path)
{
	_finddata_t filedata;
	long lf;
	cout << path.c_str() << "\n" << endl;
	string newpath = path + "*.*";
	if ((lf = _findfirst(newpath.c_str(), &filedata)) != -1)
	{
		while (_findnext(lf, &filedata) == 0)
		{
			//啊。。。filedata.name是char*类型的，并不能用==
			if (!strcmp(filedata.name, ".."))
				continue;
			if (filedata.attrib == _A_SUBDIR)
			{//当是目录的时候就递归遍历

				this->Diclist(path + filedata.name + "\\");
			}
			else
			{//当是文件的时候，就对文件进行处理
				//strcat(str2ch(path), filedata.name)
				string content = this->DealFile(path+filedata.name);
				string temppath = path;
				this->mode.addinfo(filedata.name, temppath.erase(temppath.size() - 1, 1).substr(5), content);
			}
		}
	}
}
//是目录的话就返回true 文件的话返回 false
bool DicOrFile(string filename)
{
	auto attr = GetFileAttributes(str2lpcw(filename));
	cout << attr << endl;
	switch (attr)
	{
	case FILE_ATTRIBUTE_ARCHIVE:
		return false;
	case FILE_ATTRIBUTE_DIRECTORY:
		return true;
	}
}
//处理文件
string SegArticle::DealFile(string filename)
{
	cout << "  " << filename.c_str() << endl;
	ifstream ifile(filename);
	if (!ifile)
	{
		cout << "文件不存在！" << endl;
		return "";
	}
	string filecontent;
	char ch;
	//将文件所有内容都读进字符串
	while (ifile.get(ch))
	{
		filecontent += ch;
	}
	ifile.close();
	//获取从Lines之后的所有字符串
	filecontent = filecontent.substr(filecontent.find_first_of("Lines:"));
	filecontent = filecontent.substr(filecontent.find_first_of("\n"));
	//将中间的单引号,双引号,连字符,回车，< <<全部变为空格 
	std::regex vowel_re("[[:punct:]]");
	filecontent=std::regex_replace(filecontent, vowel_re, " ");
	std::regex space_re("[[:space:]]+");
	filecontent = std::regex_replace(filecontent, space_re, " ");
	//TODO:  
	//去掉文章中所有停用词
	string temp,word;
	stringstream sstr(filecontent);
	vector<string>::iterator vsiter;
	while (sstr >> word)
	{
		
		std::transform(word.begin(), word.end(), word.begin(), std::tolower);
		//直接消除停用词,同时去除纯数字
		regex num_re("^[[:digit:]]+$");
		smatch result;
		if ((vsiter = find(stopwords.begin(), stopwords.end(), word)) != stopwords.end() || regex_search(word,result,num_re) )
			continue;
		temp += " " + word;
	}
	return temp;
}
//从文件中读取停用词表
void  SegArticle::GetStopWords()
{
	ifstream stopfile("stopwords.txt");
	string item;
	const int size = 100;
	char temp[size];
	while (stopfile.getline(temp, size))
	{
		item = temp;
		stopwords.push_back(item);
	}
	stopfile.close();
}

