#include "pubhead.h"
#include "Model.h"
//这个类主要用于提取特征词以及特征词权重的计算
//特征降维
//特征词抽取：卡方检验
//权重计算：TF-IDF
class FeaWord
{
private:
	Model model;
public:
	void GetSVM();
	
	map<string, int> DealFile(string);
	void ChiSquare();
	void SelectWord(string catname, vector<PAIR> &chiwords, int percent);
	void TfIdf();
};

