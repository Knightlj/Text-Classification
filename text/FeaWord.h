#include "pubhead.h"
#include "Model.h"
//�������Ҫ������ȡ�������Լ�������Ȩ�صļ���
//������ά
//�����ʳ�ȡ����������
//Ȩ�ؼ��㣺TF-IDF
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

