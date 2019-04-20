#include "ComputeCpu.h"
node* multcpu(node *p, node *u, int n)
{
	node *a = p->next, *b = u->next, *d;
	node *t = new node;
	t->next = NULL;
	d = t;
	node *y = new node;
	//node *r = new node;
	//r->zhi.clear();
	//r->xi.clear();
	//for (int i = 0; i < n; i++) {
	//	r->zhi.insert(r->zhi.end(), 0);
	//}
	//r->next = NULL;
	y->next = NULL;
	while (a != NULL)
	{
		b = u->next;
		d = t;
		while (b != NULL)
		{
			node *c = new node;
			c->zhi.clear();
			for (int i = 0; i<n; i++) {
				c->zhi.insert(c->zhi.end(), (a->zhi[i] + b->zhi[i]));
			}
			c->xi = multicpu(a->xi, b->xi);
			c->next = NULL;
			d->next = c;
			d = c;
			b = b->next;
		}
		y = sumcpu(y, t, n);
		a = a->next;
	}
	return y;
}
void bit_reverse_swap(C* a, int n) {
	for (int i = 1, j = n >> 1, k; i < n - 1; ++i) {
		if (i < j) swap(a[i], a[j]);
		// tricky
		for (k = n >> 1; j >= k; j -= k, k >>= 1)  // inspect the highest "1"
			;
		j += k;
	}
}

void FFT(C* a, int n, int t) {
	bit_reverse_swap(a, n);
	for (int i = 2; i <= n; i <<= 1) {
		C wi(cos(2.0 * t * PI / i), sin(2.0 * t * PI / i));
		for (int j = 0; j < n; j += i) {
			C w(1);
			for (int k = j, h = i >> 1; k < j + h; ++k) {
				C t = w * a[k + h], u = a[k];
				a[k] = u + t;
				a[k + h] = u - t;
				w *= wi;
			}
		}
	}
	if (t == -1) {
		for (int i = 0; i < n; ++i) {
			a[i] /= n;
		}
	}
}

int trans(int x) {
	return 1 << int(ceil(log(x) / log(2) - 1e-9));  // math.h/log() 以e为底
}

vector<float> multicpu(vector<float> s, vector<float> t) {
	vector<C> a, b;
	int n, m, l;
	n = s.size();
	m = t.size();
	l = trans(n + m - 1);  // n次*m次不超过n+m-1次
	float sflag = s[0];
	float tflag = t[0];
	if (sflag < 0)s[0] = -s[0];
	if (tflag < 0)t[0] = -t[0];
	vector<int> ans(l + 1);
	for (int i = 0; i < n; ++i) {
		a.insert(a.end(), C(s[n - 1 - i]));
	}
	for (int i = n; i < l; ++i) {
		a.insert(a.end(), C(0));
	}
	for (int i = 0; i < m; ++i) {
		b.insert(b.end(), C(t[m - 1 - i]));
	}
	for (int i = m; i < l; ++i) {
		b.insert(b.end(), C(0));
	}
	C *data_a, *data_b;
	data_a = (C*)malloc(sizeof(C) * a.size());
	data_b = (C*)malloc(sizeof(C) * b.size());
	memcpy(data_a, a.data(), sizeof(C) * a.size());
	memcpy(data_b, b.data(), sizeof(C) * b.size());
	////---以上main函数代码没有问题------------------
	FFT(data_a, l, 1);  //把A和B换成点值表达
	FFT(data_b, l, 1);
	for (int i = 0; i < l; ++i)  //点值做乘法
		data_a[i] *= data_b[i];
	FFT(data_a, l, -1);  //逆DFT  此处不一样


	for (int i = 0; i < l; i++) {
		ans[i] = (int)(data_a[i].real() + 0.5);
		//ans.insert(ans.end(),(int)(data_a[i].real() + 0.5));
	}
	ans[l] = 0;  // error-prone :'l' -> '1'

	for (int i = 0; i < l; ++i) {
		ans[i + 1] += ans[i] / 10;
		ans[i] %= 10;
	}
	int p = l;
	vector<float> res;
	for (; p && !ans[p]; --p);

	float tmp;
	char tmpc;
	for (; ~p; p--) {
		//putchar(ans[p] + '0');
		res.insert(res.end(), ans[p]);

	}
	if ((sflag*tflag) < 0)res[0] = -res[0];
	return  res;
}

node* sumcpu(node *p, node *u, int n)
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
				m->xi = addIntegercpu(a->xi, b->xi);
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

node *subcpu(node *p, node *u, int n)
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
				m->xi = addIntegercpu(a->xi, m->xi);
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

vector<float> addIntegercpu(vector<float> a, vector<float> b) {
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
		tmpresult = addmaincpu(a, b);
	}
	else if (aflag< 0 && bflag>0) {
		if (lena > lenb) {
			tmpresult = submaincpu(a, b);
			//tmpresult[0] = -tmpresult[0];
			changeflag = true;
		}
		else if (lena<lenb) {
			tmpresult = submaincpu(b, a);
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
				tmpresult = submaincpu(a, b);
				changeflag = true;
				//tmpresult[0] = -tmpresult[0];
			}
			else {
				tmpresult = submaincpu(b, a);
			}
		}

	}
	else if (aflag > 0 && bflag < 0) {
		if (lena > lenb) {
			tmpresult = submaincpu(a, b);
		}
		else if (lena < lenb) {
			tmpresult = submaincpu(b, a);
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
				tmpresult = submaincpu(a, b);
			}
			else {
				tmpresult = submaincpu(b, a);
				changeflag = true;
				//tmpresult[0] = -tmpresult[0];
			}
		}
	}
	else if (aflag < 0 && bflag < 0) {
		tmpresult = addmaincpu(a, b);
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

vector<float> addmaincpu(vector<float> a, vector<float> b)
{

	int N = a.size();
	vector<float> c(N + 1);
	float *dev_a, *dev_b, *dev_c;
	dev_a = (float*)malloc(sizeof(float) * N);
	dev_b = (float*)malloc(sizeof(float) * N);
	dev_c = (float*)malloc(sizeof(float) * (N + 1));
	memcpy(dev_a, a.data(), sizeof(float) * N);
	memcpy(dev_b, b.data(), sizeof(float) * N);
	memcpy(dev_c, c.data(), sizeof(float) * (N + 1));
	addcpu(dev_a, dev_b, dev_c, N);//<N,1>,第一个参数N代表block的数量，第二个参数1代表每个block中thread的数量
	memcpy(&c[0], dev_c, sizeof(float) * (N + 1));
	return c;
}

void addcpu(const float* a, const float*b, float* c, int N)
{
	for (int i = 0; i < N; i++) {
		c[i] = a[i] + b[i];
	}
}

vector<float> submaincpu(vector<float> a, vector<float> b)
{

	int N = a.size();
	vector<float> c(N + 1);
	float *dev_a, *dev_b, *dev_c;
	dev_a = (float*)malloc(sizeof(float) * N);
	dev_b = (float*)malloc(sizeof(float) * N);
	dev_c = (float*)malloc(sizeof(float) * (N + 1));
	memcpy(dev_a, a.data(), sizeof(float) * N);
	memcpy(dev_b, b.data(), sizeof(float) * N);
	memcpy(dev_c, c.data(), sizeof(float) * (N + 1));
	subVectorcpu(dev_a, dev_b, dev_c, N);//<N,1>,第一个参数N代表block的数量，第二个参数1代表每个block中thread的数量
	memcpy(&c[0], dev_c, sizeof(float) * (N + 1));
	return c;
}

void subVectorcpu(const float* a, const float*b, float* c, int N)
{
	for (int i = 0; i < N; i++) {
		c[i] = a[i] - b[i];
	}
}

node* devicecpu(node *p, node *u, int num, char name[])
{
	node *r0 = new node;//被除多项式节点
	node *r1 = new node;//除多项式节点
	r0 = p;
	r1 = u;
	//-------------------pquo------------------------
	node *headPquo = new node;
	node *pquo = new node;
	pquo->xi.clear();
	pquo->xi.insert(pquo->xi.end(), 0);
	pquo->zhi.clear();
	for (int i = 0; i<num; i++)
	{
		pquo->zhi.insert(pquo->zhi.end(), 0);
	}
	pquo->next = NULL;
	headPquo->next = pquo;
	//----------------------pquo-----------------------
	//----------------------x^(m-n)-----------------------
	node *tmp = new node;
	node *tmp2 = new node;
	tmp2->xi.clear();
	tmp2->xi.insert(tmp2->xi.end(), 1);
	tmp2->zhi.clear();
	for (int i = 0; i<num; i++)
	{
		tmp2->zhi.insert(tmp2->zhi.end(), 0);
	}
	tmp2->next = NULL;
	tmp->next = tmp2;
	//----------------------x^(m-n)-----------------------
	//----------------------lcoeff1------------------------

	int m, n;
	m = r0->next->zhi[0];
	n = r1->next->zhi[0];

	node *hlcoeff0 = new node;
	node *lcoeff0 = new node;
	lcoeff0->xi = r0->next->xi;
	lcoeff0->zhi.clear();
	lcoeff0->zhi = r0->next->zhi;
	lcoeff0->zhi[0] = 0;
	lcoeff0->next = NULL;
	hlcoeff0->next = lcoeff0;

	node *hlcoeff1 = new node;
	node *lcoeff1 = new node;
	lcoeff1->xi = r1->next->xi;
	lcoeff1->zhi.clear();
	lcoeff1->zhi = r1->next->zhi;
	lcoeff1->zhi[0] = 0;
	lcoeff1->next = NULL;
	hlcoeff1->next = lcoeff1;
	//----------------------lcoeff1------------------------
	while (true)
	{
		if (m<n || n == 0)
		{
			break;
		}
		else
		{
			node *r = new node;
			r = r1;
			tmp2->zhi[0] = m - n;
			headPquo = sumcpu(multcpu(hlcoeff1, headPquo, num), multcpu(hlcoeff0, tmp, num), num);
			r1 = subcpu(multcpu(hlcoeff1, r0, num), multcpu(multcpu(hlcoeff0, r1, num), tmp, num), num);
			r1 = r1->next;
			r0 = r1;
			r1 = r;
			if (r0->next != NULL)
			{
				m--;
				lcoeff0->xi = r0->next->xi;
				lcoeff0->zhi = r0->next->zhi;
				lcoeff0->zhi[0] = 0;
			}
			else
			{
				m = 0;
				break;
			}
		}
	}
	cout << "\nprem:";
	print(r0, num, name, 'c');
	cout << "pquo:";
	print(headPquo, num, name,' d');
	return p;
}

