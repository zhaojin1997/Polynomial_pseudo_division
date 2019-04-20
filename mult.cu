#include "mult.h"
node* mult(node *p, node *u, int n)
{
	node *a = p->next, *b = u->next, *d;
	node *t = new node;
	t->next = NULL;
	d = t;
	node *y = new node;

	/*node *r = new node;
	r->zhi.clear();
	r->xi.clear();
	for (int i = 0; i < n; i++) {
	r->zhi.insert(r->zhi.end(), 0);
	}
	r->next = NULL;
	y->next = r;*/
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
			c->xi = multi(a->xi, b->xi);
			c->next = NULL;
			d->next = c;
			d = c;
			b = b->next;
		}
		y = sum(y, t, n);
		a = a->next;
	}
	return y;
}
__global__ void ConvertToInt(cufftReal *a, int size)
{
	const int numThreads = blockDim.x * gridDim.x;
	const int threadID = blockIdx.x * blockDim.x + threadIdx.x;
	int* b = (int*)a;
	for (int i = threadID; i < size; i += numThreads)
		b[i] = static_cast<int>(round(a[i]));
}
vector<float> multi(vector<float> aa, vector<float> bb) {
	int lena = aa.size();
	int lenb = bb.size();
	float aflag = aa[0];
	float bflag = bb[0];
	//	cout << "aflag:" << aflag << ",bflag:" << bflag << endl;
	if (aflag < 0)aa[0] = -aa[0];
	if (bflag < 0)bb[0] = -bb[0];
	vector<float> a(lenb);
	vector<float> b(lena);
	//在vector首部执行大量insert操作会严重影响性能，因此转换为在尾部执行
	a.insert(a.end(), aa.begin(), aa.end());
	b.insert(b.end(), bb.begin(), bb.end());
	//printf(".....................do multiply............................\n");
	//设置进制
	vector<int> c = multiply(a, b);
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

	//去掉多余的零
	c.pop_back();
	int i = 0;
	if (c[0] == 0) { i++; }
	if ((aflag*bflag) < 0)c[0] = -c[0];
	vector<float> result;
	result.insert(result.end(), c.begin(), c.end());
	//输出最终结果，改了进制需要改这里的输出方式，比如百进制是"%02d",千进制是"%03d"
	return result;
}
vector<int> multiply(const vector<float> &a, const vector<float> &b)
{
	//时域转换频域后进行点乘
	const int NX = a.size();
	cufftHandle plan_a, plan_b, plan_c;
	cufftComplex *data_a, *data_b;
	vector<int> c(a.size());

	//分配显卡内存并初始化，这里假设sizeof(int)==sizeof(float), sizeof(cufftComplex)==2*sizeof(float)
	cudaMalloc((void**)&data_a, sizeof(cufftComplex) * (NX / 2 + 1) * BATCH);
	cudaMalloc((void**)&data_b, sizeof(cufftComplex) * (NX / 2 + 1) * BATCH);
	cudaMemcpy(data_a, a.data(), sizeof(float) * a.size(), cudaMemcpyHostToDevice);
	cudaMemcpy(data_b, b.data(), sizeof(float) * b.size(), cudaMemcpyHostToDevice);
	if (cudaGetLastError() != cudaSuccess) { fprintf(stderr, "Cuda error: Failed to allocate\n"); return c; }

	if (cufftPlan1d(&plan_a, NX, CUFFT_R2C, BATCH) != CUFFT_SUCCESS) { fprintf(stderr, "CUFFT error: Plan creation failed"); return c; }
	if (cufftPlan1d(&plan_b, NX, CUFFT_R2C, BATCH) != CUFFT_SUCCESS) { fprintf(stderr, "CUFFT error: Plan creation failed"); return c; }
	if (cufftPlan1d(&plan_c, NX, CUFFT_C2R, BATCH) != CUFFT_SUCCESS) { fprintf(stderr, "CUFFT error: Plan creation failed"); return c; }

	//把A(x)转换到频域
	if (cufftExecR2C(plan_a, (cufftReal*)data_a, data_a) != CUFFT_SUCCESS)
	{
		fprintf(stderr, "CUFFT error: ExecR2C Forward failed");
		return c;
	}

	//把B(x)转换到频域
	if (cufftExecR2C(plan_b, (cufftReal*)data_b, data_b) != CUFFT_SUCCESS)
	{
		fprintf(stderr, "CUFFT error: ExecR2C Forward failed");
		return c;
	}

	//点乘
	ComplexPointwiseMulAndScale << <NX / 256 + 1, 256 >> >(data_a, data_b, NX);

	//把C(x)转换回时域
	if (cufftExecC2R(plan_c, data_b, (cufftReal*)data_b) != CUFFT_SUCCESS)
	{
		fprintf(stderr, "CUFFT error: ExecC2R Forward failed");
		return c;
	}

	//将浮点数的结果转换为整数
	ConvertToInt << <NX / 256 + 1, 256 >> >((cufftReal*)data_b, NX);

	if (cudaDeviceSynchronize() != cudaSuccess)
	{
		fprintf(stderr, "Cuda error: Failed to synchronize\n");
		return c;
	}

	cudaMemcpy(&c[0], data_b, sizeof(float) * b.size(), cudaMemcpyDeviceToHost);

	cufftDestroy(plan_a);
	cufftDestroy(plan_b);
	cufftDestroy(plan_c);
	cudaFree(data_a);
	cudaFree(data_b);
	return c;
}
__global__ void ComplexPointwiseMulAndScale(cufftComplex *a, cufftComplex *b, int size)
{
	const int numThreads = blockDim.x * gridDim.x;
	const int threadID = blockIdx.x * blockDim.x + threadIdx.x;
	float scale = 1.0f / (float)size;
	cufftComplex c;
	for (int i = threadID; i < size; i += numThreads)
	{
		c = cuCmulf(a[i], b[i]);//a*b
		b[i] = make_cuFloatComplex(scale*cuCrealf(c), scale*cuCimagf(c));//分别对复数的实部和虚部乘以系数
	}
}

