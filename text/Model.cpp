#include "stdafx.h" 
#include "Model.h"

Model::Model()
{
	OnInitADOConn();
}
Model::~Model()
{
	ExitConnect();
}
void Model::addinfo(string name, string kind, string content)
{
	string query = "insert into dataset (name,kind,content) values('"+name+"', '"+kind+"', '"+content+"')";
	this->ExecuteSQL(query.c_str());
}
void Model::OnInitADOConn()
{
	//初始化OLE/COM库环境
	::CoInitialize(NULL);
	try
	{
		//创建connection对象
		m_pConnection.CreateInstance("ADODB.Connection");
		//设置连接字符串
		_bstr_t strConnect = "Provider=SQLOLEDB.1;Password=root;Persist Security Info=True;User ID=sa;Initial Catalog=data;Data Source=CCODER-PC";

		//SERVER和UID,PWD的设置根据实际情况来设置
		m_pConnection->Open(strConnect, "sa", "root", adModeUnknown);

	}
	//捕捉异常
	catch (_com_error e)
	{
		//显示错误信息
		printf(e.Description());
	}
}
_RecordsetPtr& Model::GetRecordSet(_bstr_t bstrSQL)
{
	try
	{
		//连接数据库，如果connection对象为空，则重新连接数据库
		if (m_pConnection == NULL)
			OnInitADOConn();
		//创建记录集对象
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		//取得表中的记录
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch (_com_error e)
	{
		printf(e.Description());
	}
	//返回记录集
	return m_pRecordset;
}
BOOL  Model::ExecuteSQL(_bstr_t bstrSQL)
{
	_variant_t RecordsAffected;
	try
	{
		//是否已连接数据库
		if (m_pConnection == NULL)
			OnInitADOConn();
		m_pConnection->Execute(bstrSQL, NULL, adCmdText);
		return true;
	}
	catch (_com_error e)
	{
		printf(e.Description());
		return false;
	}
}
void Model::ExitConnect()
{
	//关闭记录集和连接
	if (m_pRecordset != NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
	//释放环境
	::CoUninitialize();
}
//获取每个种类的文章数目
map<string, int> Model::catcounts()
{
	string query = "select [kind],count(kind) as counts from dataset group by kind";
	m_pRecordset=this->GetRecordSet(query.c_str());
	map<string, int> result;
	while (!m_pRecordset->adoEOF)
	{
		string str = LPSTR(_bstr_t(m_pRecordset->GetCollect("kind")));
		result[str]=m_pRecordset->GetCollect("counts");
		m_pRecordset->MoveNext();
	}
	return result;
}
/*
param string cat :分类名
param string bool is_train:是否为训练集
return map<string,string> 返回对应的分类的文档内容。<文章名，内容>
*/
map<string, string> Model::postsbycat(string cat,bool is_train)
{
	stringstream querystream;
	
	if (!is_train)
	{
		querystream << "select name,content from dataset where id not in( select top " << this->trainpercent
			<< " id from dataset  where kind='" << cat << "') AND kind='" << cat << "'";
	}
	else
	{
		querystream << "select top " << this->trainpercent << " percent name,content from dataset where kind='" << cat << "'";
	}
	map<string, string> result;
	m_pRecordset = this->GetRecordSet(querystream.str().c_str());
	while (!m_pRecordset->adoEOF)
	{
		string name = LPSTR(_bstr_t(m_pRecordset->GetCollect("name")));
		result[name] = LPSTR(_bstr_t(m_pRecordset->GetCollect("content")));
		m_pRecordset->MoveNext();
	}
	return result;
}
/*
param string cat :分类名
param string table :表名
return map<string,int> 获取某个分类中所有特征名及其对应数量，通过table表指定新旧表。默认为旧表fc
*/
map<string, int> Model::feasbycat(string cat,string table)
{
	string query = "select feature,count from "+table +" where cat='" + cat + "'";
	map<string, int> result;
	m_pRecordset = this->GetRecordSet(query.c_str());
	while (!m_pRecordset->adoEOF)
	{
		string feature = LPSTR(_bstr_t(m_pRecordset->GetCollect("feature")));
		result[feature] = m_pRecordset->GetCollect("count");
		m_pRecordset->MoveNext();
	}
	return result;
}
/*
*/
void Model::AddFeasCat(map<string, int> feaofcat, string catname,int threshold)
{
	auto itfc = feaofcat.cbegin();
	int count = 1;
	while (itfc != feaofcat.cend())
	{
		if (itfc->second <= threshold)
		{
			++itfc;
			continue;
		}
		stringstream query;
		query<< "insert into fc (feature,cat,count) values ('" << itfc->first << "','" << catname << "'," << itfc->second << ")";
		this->ExecuteSQL(query.str().c_str());
		++itfc;
	}
}
//A: 包含t且属于类别c的文档数量
int Model::countA(string cat, string feature)
{
	string query = "select count from fc where feature='" + feature + "' " + "and cat='" + cat + "'";
	m_pRecordset = this->GetRecordSet(query.c_str());
	_variant_t var = m_pRecordset->GetCollect("count");
	if (var.vt == VT_NULL || var.vt == VT_EMPTY)
		return 0;
	return m_pRecordset->GetCollect("count");
}
//B: 包含t且不属于类别c的文档数量 
int Model::countB(string cat, string feature)
{
	string query = "select sum([count]) as allcounts from fc where feature='" + feature + "' " + "and cat!='" + cat + "'";
	m_pRecordset = this->GetRecordSet(query.c_str());
	_variant_t var = m_pRecordset->GetCollect("allcounts");
	if (var.vt == VT_NULL || var.vt == VT_EMPTY )
		return 0;
	return m_pRecordset->GetCollect("allcounts");
}
//C: 不包含t且属于类别c的文档数量
int Model::countC(string cat, string feature)
{
	auto allcounts = this->catcounts()[cat];
	return allcounts - this->countA(cat,feature);
}
// D: 不包含t且不属于类别c的文档数量
int Model::countD(string cat, string feature)
{
	return countposts() - countA(cat, feature) - countB(cat, feature) - countC(cat, feature);
}
//计算总的文章数目
int Model::countposts()
{
	auto catc = this->catcounts();
	int sum = 0;
	for (auto it = catc.begin(); it != catc.end(); ++it)
		sum += it->second;
	return sum;
}
void Model::addChi(vector<PAIR> chiwords,string catname)
{
	for (size_t i = 0; i < chiwords.size(); ++i)
	{
		stringstream querystream;
		querystream<< "insert into nfc (feature,cat,count) values ('" << chiwords[i].first << "','" << catname << "'," << this->afeacatcount(catname, chiwords[i].first) << ")";
		this->ExecuteSQL(querystream.str().c_str());
	}
}
/*
 param string catname :分类名
 param string feature :特征
 return 获取某一个单词在某分类下的出现的次数,通过table表指定新旧表。默认为旧表fc
*/
int Model::afeacatcount(string catname,string feature,string table)
{
	string query = "select count from " + table + " where cat='" + catname + "' AND feature='" + feature + "'";
	m_pRecordset = this->GetRecordSet(query.c_str());
	_variant_t var = m_pRecordset->GetCollect("count");
	if (var.vt == VT_NULL || var.vt == VT_EMPTY)
		return 0;
	return m_pRecordset->GetCollect("count");
}
/*
param string catname :分类名
param string feature :特征
return 在某分类下的特征的总次数,通过table表指定新旧表。默认为旧表fc
*/
int Model::feasumcat(string catname, string table)
{
	string query = "select sum(count) as featotals from " + table + " where cat='" + catname + "'";
	m_pRecordset = this->GetRecordSet(query.c_str());
	_variant_t var = m_pRecordset->GetCollect("featotals");
	if (var.vt == VT_NULL || var.vt == VT_EMPTY)
		return 0;
	return m_pRecordset->GetCollect("featotals");
}
/*
param string catname :分类名
param string feature :特征
return double 获取某一个单词在某分类下的TF值
*/
double Model::GetTf(string catname, string feature)
{
	int acount = this->afeacatcount(catname,feature,"nfc");
	int sumcounts = this->feasumcat(catname,"nfc");
	return double(double(acount)/double(sumcounts));
}
//获取某一个单词的IDF值
double Model::GetIdf(string feature,string table)
{
	int postsum = this->countposts();
	int contain;
	string query = "select sum(count) as containcounts from " + table + " where feature='" + feature + "'";
	m_pRecordset = this->GetRecordSet(query.c_str());
	_variant_t var = m_pRecordset->GetCollect("containcounts");
	if (var.vt == VT_NULL || var.vt == VT_EMPTY)
		contain=0;
	contain=m_pRecordset->GetCollect("containcounts");
	return log(double(postsum) / double(contain + 1));
}
//计算TF-IDF值，并存入数据库
double Model::GetTfIdf(string catname, string feature)
{
	double tfidf = GetTf(catname, feature)*GetIdf(feature, "nfc");
	stringstream querystream;
	querystream << "update nfc set weight = " << tfidf << " where cat='" << catname << "' AND feature='" << feature << "'";
	this->ExecuteSQL(querystream.str().c_str());
	return tfidf;
}

//选出一个权重，次数，分类，特征
map <string, map<string, Feature>> Model::getallfeature()
{
	auto cats=this->catcounts();
	auto itcat = cats.begin();
	map <string, map<string, Feature>> result;
	while (itcat != cats.end())
	{
		string query = "select feature,weight,count from nfc where cat='"+itcat->first+"'";
		m_pRecordset = this->GetRecordSet(query.c_str());
		map<string, Feature> tempacat;
		while (!m_pRecordset->adoEOF)
		{
			Feature fea;
			string feature = LPSTR(_bstr_t(m_pRecordset->GetCollect("feature")));
			double weight = m_pRecordset->GetCollect("weight");
			int count = m_pRecordset->GetCollect("count");
			fea.weight = weight;
			fea.count = count;
			tempacat.insert(make_pair(feature, fea));
			m_pRecordset->MoveNext();
		}
		result.insert(make_pair(itcat->first,tempacat));
		++itcat;
	}
	return result;
}
