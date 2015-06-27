#include "pubhead.h"
#include <map>
#include <numeric>
#include <cmath>
#ifndef _MODEL_H_     //如果没有定义这个宏
#define _MODEL_H_     //定义这个宏
//使用ADO方式连接数据库
class Model
{
public:
	//添加一个指向Connection对象的指针
	_ConnectionPtr m_pConnection;
	//添加一个指向Recordset对象的指针
	_RecordsetPtr m_pRecordset;
	int trainpercent = 67;
public:
	Model();
	virtual ~Model();
	void addinfo(string name, string kind, string content);
	map<string, int> catcounts();  //返回每个种类的数量
	map<string, string> postsbycat(string cat, bool is_train=true); //按指定类别返回其所有的文章名和文章内容
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
	//初始化--连接数据库
	void OnInitADOConn();
	//执行查询
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);
	//执行SQL语句
	BOOL ExecuteSQL(_bstr_t bstrSQL);
	//断开数据库连接
	void ExitConnect();
};
#endif
