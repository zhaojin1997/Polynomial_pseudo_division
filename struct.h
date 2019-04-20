#ifndef STRUCT_H
#define STRUCT_H
#include<iostream>
#include <device_launch_parameters.h>
#include <cuda_runtime.h>
#include <cufft.h>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include<string>
#include <fstream>
#include <cstdio>
#include <complex>
#include <cstring>
#include <time.h>
using namespace std;
const int BATCH = 1;
const double PI(acos(-1.0));
typedef complex<double> C;
struct node
{
	vector<float> xi;//系数
	vector<int> zhi;
	node *next;
};
#endif
