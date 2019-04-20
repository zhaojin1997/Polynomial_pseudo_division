#ifndef COMPUTECPU_H
#define COMPUTECPU_H
#include "struct.h"
#include "support.h"

node* multcpu(node *p, node *u, int n);
void bit_reverse_swap(C* a, int n);
void FFT(C* a, int n, int t);
int trans(int x);
vector<float> multicpu(vector<float> s, vector<float> t);
node* sumcpu(node *p, node *u, int n);
node *subcpu(node *p, node *u, int n);
vector<float> addIntegercpu(vector<float> a, vector<float> b);
vector<float> addmaincpu(vector<float> a, vector<float> b);
void addcpu(const float* a, const float*b, float* c, int N);
vector<float> submaincpu(vector<float> a, vector<float> b);
void subVectorcpu(const float* a, const float*b, float* c, int N);
node* devicecpu(node *p, node *u, int num, char name[]);
#endif
