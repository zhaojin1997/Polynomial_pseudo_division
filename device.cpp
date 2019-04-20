#include "device.h"
node* device(node *p, node *u, int num, char name[])
{
	node *r0 = new node;//被除多项式节点
	node *r1 = new node;//除多项式节点
	r0 = p;
	r1 = u;
	//-------------------pquo------------------------
	node *headPquo = new node;
	node *pquo = new node;
	pquo->xi.clear();
	pquo->xi.insert(pquo->xi.end(),0);
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
			headPquo = sum(mult(hlcoeff1, headPquo, num), mult(hlcoeff0, tmp, num), num);
			r1 = sub(mult(hlcoeff1, r0, num), mult(mult(hlcoeff0, r1, num), tmp, num), num);
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
	print(r0, num, name,'a');
	cout << "pquo:";
	print(headPquo, num, name,'b');
	return p;
}
