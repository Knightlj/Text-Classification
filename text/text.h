#include "SegArticle.h"
#include "Model.h"
//使用朴素贝叶斯方法求解
class Classify
{
private:
	int sumcounts;
	map<string, int> cats;
	Model model;
	map <string, map<string, Feature>> featable;
public:
	Classify();
	void run();
	string Bayes(string content);
};
Classify::Classify()
{
	cout << "正在准备数据.." << endl;
	cout << "正在获取文章总数.." << endl;
	this->sumcounts=this->model.countposts();
	cout << "文章总数获取完成!!" << endl;
	cout << "正在获取各分类数.." << endl;
	cats = this->model.catcounts();
	cout << "各分类数获取完成!!" << endl;
	cout << "正在获取各分类特征总表.." << endl;
	featable = this->model.getallfeature();
	cout << "各分类总表获取完成!!" << endl;
	cout << "数据准备完成!!" << endl;
}

void Classify::run()
{
	auto itcat = cats.cbegin();
	int total = 0;
	int right = 0;
	while (itcat != cats.cend())
	{
		//按照分类为组依次取出测试集的文章
		auto testdatas = this->model.postsbycat(itcat->first, false);
		auto ittest = testdatas.begin();
		while (ittest != testdatas.end())
		{
			++total;
			auto newcat=Bayes(ittest->second);
			cout << ittest->first << " " << newcat.c_str() << " " << itcat->first << endl;
			if (newcat == itcat->first)
			{
				++right;
			}
			++ittest;
		}
		++itcat;
		break;
	}
	cout << "数据正确率为" << double(right)/double(total)<<endl;
}
string  Classify::Bayes(string content)
{
	
	vector<PAIR> result;
	auto itcat = cats.cbegin();
	while (itcat != cats.cend())
	{
		double catprob = double(cats[itcat->first]) / double(sumcounts);
		auto feasofcat = featable[itcat->first];
		string word;
		stringstream contentstream(content);
		double temp = 0;
		while (contentstream >> word)
		{
			if (feasofcat.count(word))
				temp += feasofcat[word].weight*catprob;
		}
		result.push_back(make_pair(itcat->first,temp));
		++itcat;
	}
	sort(result.begin(), result.end(),
		  [](const PAIR &x, const PAIR &y)
	           { return (x.second > y.second); });
	return result[0].first;
}