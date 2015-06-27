#include "pubhead.h"
#include <map>
#include <numeric>
#include <cmath>
#ifndef _MODEL_H_     //���û�ж��������
#define _MODEL_H_     //���������
//ʹ��ADO��ʽ�������ݿ�
class Model
{
public:
	//���һ��ָ��Connection�����ָ��
	_ConnectionPtr m_pConnection;
	//���һ��ָ��Recordset�����ָ��
	_RecordsetPtr m_pRecordset;
	int trainpercent = 67;
public:
	Model();
	virtual ~Model();
	void addinfo(string name, string kind, string content);
	map<string, int> catcounts();  //����ÿ�����������
	map<string, string> postsbycat(string cat, bool is_train=true); //��ָ����𷵻������е�����������������
	void AddFeasCat(map<string, int> feaofpost, string catname, int threshold);
	map<string, int> feasbycat(string cat, string table = "fc");
	int countA(string cat, string feature);
	int  countB(string cat, string feature);
	int countC(string cat, string feature);
	int countD(string cat, string feature);
	int countposts();
	void addChi(vector<PAIR> chiwords, string catname);
	int afeacatcount(string catname, string feature, string table = "fc");
	double GetTf(string catname, string feature);
	int feasumcat(string catname, string table = "fc");
	double GetIdf(string feature, string table = "fc");
	double GetTfIdf(string catname, string feature);
	map <string, map<string, Feature>> getallfeature();
private:
	//��ʼ��--�������ݿ�
	void OnInitADOConn();
	//ִ�в�ѯ
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);
	//ִ��SQL���
	BOOL ExecuteSQL(_bstr_t bstrSQL);
	//�Ͽ����ݿ�����
	void ExitConnect();
};
#endif
