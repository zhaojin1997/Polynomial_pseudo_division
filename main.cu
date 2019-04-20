#include "support.h"
#include "struct.h"
#include "add.h"
#include "mult.h"
#include "device.h"
#include "ComputeCpu.h"


int main(int argc, char *argv[])
{
printf("请输入变量个数:");
	int temNum = 0;
	cin >> temNum;
	printf("请输入变量名：");
	char* temName = (char*)malloc(temNum * sizeof(char));
	for (int i = 0; i<temNum; i++)
	{
		cin >> temName[i];
	}
	cout << "\n---------------------------------------------------" << endl;
	cout << "请按提示输入多项式A:" << endl;;
	struct node *a = new node;//多项式A
	input(a, temNum, temName);
	cout << "\n---------------------------------------------------" << endl;
	cout << "请按提示输入多项式B:" << endl;;
	struct node *b = new node;//多项式B
	input(b, temNum, temName);
	cout << "\n-----------------------DEVICE--------------------------" << endl;
	cudaEvent_t start2;
	cudaEventCreate(&start2);
	cudaEvent_t stop2;
	cudaEventCreate(&stop2);
	cudaEventRecord(start2, NULL);

	device(a, b, temNum, temName);

	cudaEventRecord(stop2, NULL);
	cudaEventSynchronize(stop2);
	float msecTotal2 = 0.0f;
	cudaEventElapsedTime(&msecTotal2, start2, stop2);
	printf("DEVICE CUDA Time:%f ms\n", msecTotal2);

	clock_t begin2, end2;
	begin2 = clock();

	devicecpu(a, b, temNum, temName);

	end2 = clock();
	float time2 = ((float)(end2 - begin2));
	printf("DEVICE CPU: %f ms\n", time2);
	printf("DEVICE Spped up:%f\n", time2 / msecTotal2);
	return 0;
}
