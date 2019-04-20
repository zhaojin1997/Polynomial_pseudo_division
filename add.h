#ifndef ADD_H
#define ADD_H
#include "struct.h"
#include "support.h"

vector<float> addInteger(vector<float> a, vector<float> b);
vector<float> addmain(vector<float> a, vector<float> b);
__global__ void add(const float* a, const float*b, float* c);
vector<float> submain(vector<float> a, vector<float> b);
__global__ void subVector(const float* a, const float*b, float* c);
node* sum(node *p, node *u, int n);
node *sub(node *p, node *u, int n);
#endif
