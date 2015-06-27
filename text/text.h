#include "SegArticle.h"
#include "Model.h"
//ʹ�����ر�Ҷ˹�������
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
	cout << "����׼������.." << endl;
	cout << "���ڻ�ȡ��������.." << endl;
	this->sumcounts=this->model.countposts();
	cout << "����������ȡ���!!" << endl;
	cout << "���ڻ�ȡ��������.." << endl;
	cats = this->model.catcounts();
	cout << "����������ȡ���!!" << endl;
	cout << "���ڻ�ȡ�����������ܱ�.." << endl;
	featable = this->model.getallfeature();
	cout << "�������ܱ��ȡ���!!" << endl;
	cout << "����׼�����!!" << endl;
}

void Classify::run()
{
	auto itcat = cats.cbegin();
	int total = 0;
	int right = 0;
	while (itcat != cats.cend())
	{
		//���շ���Ϊ������ȡ�����Լ�������
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
	cout << "������ȷ��Ϊ" << double(right)/double(total)<<endl;
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