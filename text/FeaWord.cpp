#include "stdafx.h"
#include "FeaWord.h"

//构建VSM模型到数据库里
void FeaWord::GetSVM()
{
	map<string, int> catcounts = this->model.catcounts();
	auto itcat = catcounts.cbegin();
	int a = 0;
	while (itcat != catcounts.cend())
	{
		++a;
		//提取出测试集
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
		//遍历完一个类，就插入进去
		model.AddFeasCat(feaofcat, itcat->first, 3);
		++itcat;
	}
}
//利用卡方检验进行特征选择，实现降维
/*
 Chi-Square(t,c)=(A*D - B*C) / ((A + B)*(C + D));
 A: 包含t且属于类别c的文档数量
 B: 包含t且不属于类别c的文档数量 
 C: 不包含t且属于类别c的文档数量
 D: 不包含t且不属于类别c的文档数量
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
//卡方检验选择特征向量
void  FeaWord::SelectWord(string catname, vector<PAIR> &chiwords, int percent)
{
	//使用lambda定义了排序,并且降序排列
	stable_sort(chiwords.begin(), chiwords.end(),
		        [] (const PAIR &x, const PAIR &y)
	               { return (x.second > y.second); });
	chiwords.erase((chiwords.end() - int(percent*chiwords.size() / 100)), chiwords.end());
	this->model.addChi(chiwords,catname);
}
//通过TF-IDF算法计算权重
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

