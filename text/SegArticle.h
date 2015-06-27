#include "pubhead.h"
#include <fstream>
#include <cstdlib>
#include <io.h>
#include "Model.h"

//class Model;
//用于提取文章中信息到数据库中
class SegArticle
{
private:
	string dicname = "data";
	Model mode;
public:
	SegArticle();
	vector<string> stopwords; //存放停用词表
	void Diclist(string path);
	string  DealFile(string filename);
	void GetStopWords();  
};
