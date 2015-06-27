#include "stdafx.h"
#include "FeaWord.h"

//����VSMģ�͵����ݿ���
void FeaWord::GetSVM()
{
	map<string, int> catcounts = this->model.catcounts();
	auto itcat = catcounts.cbegin();
	int a = 0;
	while (itcat != catcounts.cend())
	{
		++a;
		//��ȡ�����Լ�
		map<string, string> posts = this->model.postsbycat(itcat->first);
		auto itpost = posts.cbegin();
		map<string, int> feaofcat;
		int b = 0;
		while (itpost != posts.cend())
		{
			++b;
			stringstream contentstream(itpost->second);
			string word;
			map<string, int> feaofpost;
			while (contentstream >> word)
			{

				if (!is_asc(word))
					continue;
				if (!feaofpost.count(word))
				{
					feaofpost.insert(pair<string, int>(word, 1));
					if (!feaofcat.count(word))
					{
						feaofcat[word] = 1;
					}
					else
					{
						++feaofcat[word];
					}
				}
				else
				{
					++feaofpost[word];
				}

			}
			cout << a << "." << b << endl;
			++itpost;
		}
		//������һ���࣬�Ͳ����ȥ
		model.AddFeasCat(feaofcat, itcat->first, 3);
		++itcat;
	}
}
//���ÿ��������������ѡ��ʵ�ֽ�ά
/*
 Chi-Square(t,c)=(A*D - B*C) / ((A + B)*(C + D));
 A: ����t���������c���ĵ�����
 B: ����t�Ҳ��������c���ĵ����� 
 C: ������t���������c���ĵ�����
 D: ������t�Ҳ��������c���ĵ�����
*/

void FeaWord::ChiSquare()
{
	map<string, int> catcounts = this->model.catcounts();
	auto itcat = catcounts.cbegin();
	int a = 0;
	while (itcat != catcounts.cend())
	{
		++a;
		auto feacat=this->model.feasbycat(itcat->first);
		auto itfc=feacat.begin();
		vector<PAIR> chiword;
		int b = 0;
		while (itfc != feacat.end())
		{
			++b;
			int A = this->model.countA(itcat->first, itfc->first);
			int B = this->model.countB(itcat->first, itfc->first);
			int C = itcat->second - A;
			int D = this->model.countposts() - (A + B + C);
			float chi = float((A*D - B*C) / ((A + B)*(C + D)));
			chiword.push_back(make_pair(itfc->first, chi));
			cout << a << "." << b << endl;
			++itfc;
		}
		this->SelectWord(itcat->first,chiword,30);
		++itcat;
	}
}
//��������ѡ����������
void  FeaWord::SelectWord(string catname, vector<PAIR> &chiwords, int percent)
{
	//ʹ��lambda����������,���ҽ�������
	stable_sort(chiwords.begin(), chiwords.end(),
		        [] (const PAIR &x, const PAIR &y)
	               { return (x.second > y.second); });
	chiwords.erase((chiwords.end() - int(percent*chiwords.size() / 100)), chiwords.end());
	this->model.addChi(chiwords,catname);
}
//ͨ��TF-IDF�㷨����Ȩ��
void FeaWord::TfIdf()
{
	map<string, int> catcounts = this->model.catcounts();
	auto itcat = catcounts.cbegin();
	int a = 0;
	while (itcat != catcounts.cend())
	{
		++a;
		auto feacat = this->model.feasbycat(itcat->first,"nfc");
		auto itfc = feacat.begin();
		int b = 0;
		while (itfc != feacat.end())
		{
			++b;
			this->model.GetTfIdf(itcat->first,itfc->first);
			cout << a << "." << b << endl;
			++itfc;
		}
		++itcat;
	}
}

