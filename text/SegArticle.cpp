#include "stdafx.h" 
#include "SegArticle.h"
SegArticle::SegArticle()
{
	GetStopWords();
}
//�ݹ�������е��ļ�
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
			//��������filedata.name��char*���͵ģ���������==
			if (!strcmp(filedata.name, ".."))
				continue;
			if (filedata.attrib == _A_SUBDIR)
			{//����Ŀ¼��ʱ��͵ݹ����

				this->Diclist(path + filedata.name + "\\");
			}
			else
			{//�����ļ���ʱ�򣬾Ͷ��ļ����д���
				//strcat(str2ch(path), filedata.name)
				string content = this->DealFile(path+filedata.name);
				string temppath = path;
				this->mode.addinfo(filedata.name, temppath.erase(temppath.size() - 1, 1).substr(5), content);
			}
		}
	}
}
//��Ŀ¼�Ļ��ͷ���true �ļ��Ļ����� false
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
//�����ļ�
string SegArticle::DealFile(string filename)
{
	cout << "  " << filename.c_str() << endl;
	ifstream ifile(filename);
	if (!ifile)
	{
		cout << "�ļ������ڣ�" << endl;
		return "";
	}
	string filecontent;
	char ch;
	//���ļ��������ݶ������ַ���
	while (ifile.get(ch))
	{
		filecontent += ch;
	}
	ifile.close();
	//��ȡ��Lines֮��������ַ���
	filecontent = filecontent.substr(filecontent.find_first_of("Lines:"));
	filecontent = filecontent.substr(filecontent.find_first_of("\n"));
	//���м�ĵ�����,˫����,���ַ�,�س���< <<ȫ����Ϊ�ո� 
	std::regex vowel_re("[[:punct:]]");
	filecontent=std::regex_replace(filecontent, vowel_re, " ");
	std::regex space_re("[[:space:]]+");
	filecontent = std::regex_replace(filecontent, space_re, " ");
	//TODO:  
	//ȥ������������ͣ�ô�
	string temp,word;
	stringstream sstr(filecontent);
	vector<string>::iterator vsiter;
	while (sstr >> word)
	{
		
		std::transform(word.begin(), word.end(), word.begin(), std::tolower);
		//ֱ������ͣ�ô�,ͬʱȥ��������
		regex num_re("^[[:digit:]]+$");
		smatch result;
		if ((vsiter = find(stopwords.begin(), stopwords.end(), word)) != stopwords.end() || regex_search(word,result,num_re) )
			continue;
		temp += " " + word;
	}
	return temp;
}
//���ļ��ж�ȡͣ�ôʱ�
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

