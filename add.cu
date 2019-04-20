#include "add.h"
vector<float> addInteger(vector<float> a, vector<float> b) {
	int lena = a.size();
	int lenb = b.size();
	int aflag = a[0];
	int bflag = b[0];
	if (aflag < 0) {
		a[0] = -a[0];
	}if (bflag < 0) {
		b[0] = -b[0];
	}
	int lendev;
	vector<float> tmp;
	if (lena > lenb) {
		lendev = lena - lenb;
		for (int i = 0; i < lendev; i++) {
			tmp.insert(tmp.end(), 0);
		}
		tmp.insert(tmp.end(), b.begin(), b.end());
		b = tmp;
	}
	else {
		lendev = lenb - lena;
		for (int i = 0; i < lendev; i++) {
			tmp.insert(tmp.end(), 0);
		}
		tmp.insert(tmp.end(), a.begin(), a.end());
		a = tmp;
	}
	vector<float> tmpresult(a.size() + 1);
	bool changeflag = false;
	if (aflag > 0 && bflag > 0) {
		tmpresult = addmain(a, b);
	}
	else if (aflag< 0 && bflag>0) {
		if (lena > lenb) {
			tmpresult = submain(a, b);
			//tmpresult[0] = -tmpresult[0];
			changeflag = true;
		}
		else if (lena<lenb) {
			tmpresult = submain(b, a);
		}
		else if (lena == lenb) {
			bool flag = false;
			for (int i = 0; i < lena; i++) {
				if (a[i] > b[i]) {
					flag = true;
					break;
				}
			}
			if (flag == true) {
				tmpresult = submain(a, b);
				changeflag = true;
				//tmpresult[0] = -tmpresult[0];
			}
			else {
				tmpresult = submain(b, a);
			}
		}

	}
	else if (aflag > 0 && bflag < 0) {
		if (lena > lenb) {
			tmpresult = submain(a, b);
		}
		else if (lena < lenb) {
			tmpresult = submain(b, a);
			changeflag = true;
			//tmpresult[0] = -tmpresult[0];
		}
		else if (lena == lenb) {
			bool flag = false;
			for (int i = 0; i < lena; i++) {
				if (a[i] > b[i]) {
					flag = true;
					break;
				}
			}
			if (flag == true) {
				tmpresult = submain(a, b);
			}
			else {
				tmpresult = submain(b, a);
				changeflag = true;
				//tmpresult[0] = -tmpresult[0];
			}
		}
	}
	else if (aflag < 0 && bflag < 0) {
		tmpresult = addmain(a, b);
		changeflag = true;
		//tmpresult[0] = -tmpresult[0];
	}
	vector<int> c;
	for (int i = 0; i <tmpresult.size(); i++) {
		int a = int(tmpresult[i]);
		c.insert(c.end(), a);
	}
	const int base = 10;
	//处理进位
	for (int j = c.size() - 1; j > 0; j--)
	{
		if (c[j] >= base)
		{
			c[j - 1] += c[j] / base;
			c[j] %= base;
		}
	}
	for (int j = c.size() - 1; j > 0; j--)
	{
		if (c[j] < 0) {
			c[j] = 10 + c[j];
			c[j - 1] = c[j - 1] - 1;
		}
	}

	//去掉多余的零
	c.pop_back();
	int i = 0;
	if (c[0] == 0) { i++; }
	if (changeflag == true) {
		c[0] = -c[0];
	}
	vector<float> result;
	result.insert(result.end(), c.begin(), c.end());
	//输出最终结果，改了进制需要改这里的输出方式，比如百进制是"%02d",千进制是"%03d"
	return result;
}
vector<float> addmain(vector<float> a, vector<float> b)
{
	int N = a.size();
	vector<float> c(N + 1);
	float *dev_a, *dev_b, *dev_c;
	cudaMalloc((void **)&dev_a, N * sizeof(float));
	cudaMalloc((void **)&dev_b, N * sizeof(float));
	cudaMalloc((void **)&dev_c, (N + 1) * sizeof(float));
	cudaMemcpy(dev_a, a.data(), N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_b, b.data(), N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_c, c.data(), (N + 1) * sizeof(float), cudaMemcpyHostToDevice);
	add << <N, 1 >> >(dev_a, dev_b, dev_c);//<N,1>,第一个参数N代表block的数量，第二个参数1代表每个block中thread的数量
	cudaMemcpy(&c[0], dev_c, (N + 1) * sizeof(int), cudaMemcpyDeviceToHost);
	return c;
}
__global__ void add(const float* a, const float*b, float* c)
{
	int tid = blockIdx.x;//blockIdx是一个内置变量，blockIdx.x代表这是一个2维索引
	c[tid] = a[tid] + b[tid];
}
vector<float> submain(vector<float> a, vector<float> b)
{
	int N = a.size();
	vector<float> c(N + 1);
	float *dev_a, *dev_b, *dev_c;
	cudaMalloc((void **)&dev_a, N * sizeof(float));
	cudaMalloc((void **)&dev_b, N * sizeof(float));
	cudaMalloc((void **)&dev_c, (N + 1) * sizeof(float));
	cudaMemcpy(dev_a, a.data(), N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_b, b.data(), N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_c, c.data(), (N + 1) * sizeof(float), cudaMemcpyHostToDevice);
	subVector << <N, 1 >> >(dev_a, dev_b, dev_c);//<N,1>,第一个参数N代表block的数量，第二个参数1代表每个block中thread的数量
	cudaMemcpy(&c[0], dev_c, (N + 1) * sizeof(int), cudaMemcpyDeviceToHost);
	return c;
}
__global__ void subVector(const float* a, const float*b, float* c)
{
	int tid = blockIdx.x;//blockIdx是一个内置变量，blockIdx.x代表这是一个2维索引
	c[tid] = a[tid] - b[tid];
}

node* sum(node *p, node *u, int n)
{
	node *a = new node;
	node *b = new node;
	node *l = new node;
	node *h = new node;
	h->next = NULL;
	l = h;
	a = p->next;
	b = u->next;
	while (a != NULL&&b != NULL)
	{
		if (a->zhi>b->zhi)
		{
			node *m = new node;
			m->zhi = a->zhi;
			m->xi = a->xi;
			m->next = NULL;
			l->next = m;
			l = m;
			a = a->next;
		}
		else if (a->zhi<b->zhi)
		{
			node *m = new node;
			m->zhi = b->zhi;
			m->xi = b->xi;
			m->next = NULL;
			l->next = m;
			l = m;
			b = b->next;
		}
		else
		{
			if ((a->xi.size() + b->xi.size()) != 0)
			{
				node *m = new node;
				m->xi = addInteger(a->xi, b->xi);
				m->zhi = a->zhi;
				m->next = NULL;
				l->next = m;
				l = m;
				b = b->next;
				a = a->next;
			}
			else
			{
				b = b->next;
				a = a->next;
			}
		}
	}
	if (a == NULL&&b != NULL)
	{
		while (b != NULL)
		{
			node *m = new node;
			m->xi = b->xi;
			m->zhi = b->zhi;
			m->next = NULL;
			l->next = m;
			l = m;
			b = b->next;
		}
	}
	else if (a != NULL&&b == NULL)
	{
		while (a != NULL)
		{
			node *m = new node;
			m->xi = a->xi;
			m->zhi = a->zhi;
			m->next = NULL;
			l->next = m;
			l = m;
			a = a->next;
		}
	}
	return h;
}
node *sub(node *p, node *u, int n)
{
	node *a = new node;
	node *b = new node;
	node *l = new node;
	node *h = new node;
	h->next = NULL;
	l = h;
	a = p->next;
	b = u->next;
	while (a != NULL&&b != NULL)
	{
		if (a->zhi>b->zhi)
		{
			node *m = new node;
			m->zhi = a->zhi;
			m->xi = a->xi;
			m->next = NULL;
			l->next = m;
			l = m;
			a = a->next;
		}
		else if (a->zhi<b->zhi)
		{
			node *m = new node;
			//b->xi[0]=-b->xi[0];
			m->zhi = b->zhi;
			m->xi = b->xi;
			m->xi[0] = -m->xi[0];
			m->next = NULL;
			l->next = m;
			l = m;
			b = b->next;
		}
		else
		{
			if ((a->xi.size() + b->xi.size()) != 0)
			{
				node *m = new node;
				//b->xi[0]=-b->xi[0];
				m->xi = b->xi;
				m->xi[0] = -m->xi[0];
				m->xi = addInteger(a->xi, m->xi);
				m->zhi = a->zhi;
				m->next = NULL;
				l->next = m;
				l = m;
				b = b->next;
				a = a->next;
			}
			else
			{
				b = b->next;
				a = a->next;
			}
		}
	}
	if (a == NULL&&b != NULL)
	{
		while (b != NULL)
		{
			node *m = new node;
			m->xi = b->xi;
			m->xi[0] = -m->xi[0];
			m->zhi = b->zhi;
			m->next = NULL;
			l->next = m;
			l = m;
			b = b->next;
		}
	}
	else if (a != NULL&&b == NULL)
	{
		while (a != NULL)
		{
			node *m = new node;
			m->xi = a->xi;
			m->zhi = a->zhi;
			m->next = NULL;
			l->next = m;
			l = m;
			a = a->next;
		}
	}
	return h;
}

