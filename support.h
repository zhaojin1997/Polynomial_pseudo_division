#ifndef SUPPORT_H
#define SUPPORT_H
#include "struct.h"
void input(node *p, int n, char temName[]);
vector<float> readMulNum(string paths);
vector<float> charTovector(char num[], int len);
void printNode(node *u, int n, char temName[]);
void print(node *p, int n, char temName[], int flag);
string write(vector<int> zhi, vector<float> c, char flag);

#endif
