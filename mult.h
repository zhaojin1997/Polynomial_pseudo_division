#ifndef MULT_H
#define MULT_H
#include "struct.h"
#include "support.h"
#include "add.h"
__global__ void ConvertToInt(cufftReal *a, int size);
vector<float> multi(vector<float> aa, vector<float> bb);
vector<int> multiply(const vector<float> &a, const vector<float> &b);
__global__ void ComplexPointwiseMulAndScale(cufftComplex *a, cufftComplex *b, int size);
node* mult(node *p, node *u, int n);
#endif
