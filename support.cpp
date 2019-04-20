#include "support.h"
void input(node *p, int n, char temName[])
{
	p->next = NULL;//p为多项式头节点
	node *u = new node;
	vector<int> temZhi;
	int temp;
	string path;
	vector<float> xi;
	while (true)
	{
		temZhi.clear();
		cout << "请输入系数路径(例：da1.txt):";
		cin >> path;
		if (path.length() <= 1) {
			break;
		}
		xi = readMulNum(path);

		u->xi = xi;
		char sym;
		cout << "请输入该多项式的符号：";
		cin >> sym;
		//u->symbol = sym;
		if (sym == '-') {
			u->xi[0] = -u->xi[0];
		}
		for (int i = 0; i<n; i++)
		{
			cout << "请输入" << temName[i] << "的指数:";
			cin >> temp;
			temZhi.insert(temZhi.end(), temp);
		}
		u->zhi = temZhi;
		p->next = u;
		u->next = NULL;
		p = u;
		cout << "u->xi:" << path << ",u->zhi:";
		printNode(u, n, temName);
		cout << endl;
		u = new node;
	}
}
vector<float> readMulNum(string paths) {
	char pathc[256];
	int i;
	for (i = 0; i<paths.length(); i++) {
		pathc[i] = paths[i];
	}
	pathc[i] = '\0';
	vector<float> v;
	char szTest[1000] = { 0 };
	int len = 0;
	//vector<float> v;
	vector<float> tmp;
	FILE *fp = fopen(pathc, "r");
	if (NULL == fp)
	{
		printf("failed to open file\n");
	}

	while (!feof(fp))
	{
		memset(szTest, 0, sizeof(szTest));
		fgets(szTest, sizeof(szTest) - 1, fp); // 包含了换行符
											   //printf("%s", szTest);
		int len = sizeof(szTest) / sizeof(szTest[0]);
		tmp = charTovector(szTest, len);
		v.insert(v.end(), tmp.begin(), tmp.end());
	}

	fclose(fp);
	if (v[0] > 9 || v[0] < 0) {
		v[1] = -v[1];
		v.erase(v.begin());
	}
	return v;
}
vector<float> charTovector(char num[], int len)
{
	vector<float> v2;
	for (int i = 0; i<len; i++)
	{
		float temp = num[i] - 48;
		//char类型转换为float类型后Null转化为-48
		if (temp != -48&&temp!=-38)
			v2.insert(v2.end(), temp);
	}
	return v2;
}
void printNode(node *u, int n, char temName[])
{
	for (int i = 0; i<n; i++)
	{
		if (u->zhi[i] != 0 && u->zhi[i] != 1)
		{
			cout << temName[i] << "^" << u->zhi[i];
		}
		else if (u->zhi[i] == 1)
		{
			cout << temName[i];
		}
	}
}
void print(node *p, int n, char temName[], int flag)
{
	node *u;
	string path = "";
	for (u = p->next; u != NULL; u = u->next)
	{
		path = write(u->zhi, u->xi, flag);
		if (u->xi.size() != 0 && u->zhi.size() != 0) {
			if (u->xi.size() <= 5) {
				for (int i = 0; i < u->xi.size(); i++) {
					cout << u->xi[i];
				}
			}
			else {
				cout << "(" << path << ")";
			}
			printNode(u, n, temName);
		}
		if (u->next != NULL&&u->next->xi.size() != 0) {
			cout << "+";
		}
	}
	cout << endl;
}
string write(vector<int> zhi, vector<float> c, char flag) {

	char patha[256], path[256];
	char pathtmp[256];
	memset(patha, '\0', 256);
	for (int i = 0; i<zhi.size(); i++) {
		sprintf(pathtmp, "%d", zhi[i]);
		sprintf(patha, "%.*s%.*s", sizeof(patha), patha, sizeof(pathtmp), pathtmp);
	}
	char pathb[] = { 'n','.','t','x','t','\0' };
	if (flag!=NULL) {
		pathb[0] =flag;
	}
	memset(path, '\0', 256);
	sprintf(path, "%.*s%.*s", sizeof(patha), patha, sizeof(pathb), pathb);
	ofstream in;
	in.open(path, ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
	for (int i = 0; i < c.size(); i++) {
		//printf("%d", c[i]);
		in << c[i];
	}
	in.close();
	string resultpath = path;
	return resultpath;
}
