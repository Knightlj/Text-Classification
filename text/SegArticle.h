#include "pubhead.h"
#include <fstream>
#include <cstdlib>
#include <io.h>
#include "Model.h"

//class Model;
//������ȡ��������Ϣ�����ݿ���
class SegArticle
{
private:
	string dicname = "data";
	Model mode;
public:
	SegArticle();
	vector<string> stopwords; //���ͣ�ôʱ�
	void Diclist(string path);
	string  DealFile(string filename);
	void GetStopWords();  
};
