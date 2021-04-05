#define _CRT_SECURE_NO_WARNINGS
//#include <bits/stdc++.h>
#include <cstdio>
using namespace std;

int n;
//int i1,j1,i2,j2;
int i1, i2, j1, j2;
int f1, f2;

void solve()
{
	int a;
	scanf("%d", &a);

	if (f1 == -1 && i1 + j1 <= n + n) 	{
		f1 = 3 - (f2 + a) % 3;
		printf("%d %d %d\n", f1, i1, j1);
		i1++;
		j1--;
		if (j1<1 || i1>n) 		{
			j1 = i1 + j1 + 2;
			i1 = 1;
		}
		return;
	}
	else if (f2 == -1 && i2 + j2 <= n + n) 	{
		f2 = 3 - (f1 + a) % 3;
		printf("%d %d %d\n", f2, i2, j2);
		i2++;
		j2--;
		if (j2<1 || i2>n) 		{
			j2 = i2 + j2 + 2;
			i2 = 1;
		}
		return;
	}
	else if (a == f1) 	{
		if (i2 + j2 <= n + n) 		{
			printf("%d %d %d\n", f2, i2, j2);
			i2++;
			j2--;
			if (j2<1 || i2>n) 			{
				j2 = i2 + j2 + 2;
				i2 = 1;
			}
			return;
		}
		else 		{
			printf("%d %d %d\n", f1, i1, j1);
			i1++;
			j1--;
			if (j1<1 || i1>n) 			{
				j1 = i1 + j1 + 2;
				i1 = 1;
			}
			return;
		}
	}
	else if (a == f2) 	{
		if (i1 + j1 <= n + n) 		{
			printf("%d %d %d\n", f1, i1, j1);
			i1++;
			j1--;
			if (j1<1 || i1>n) 			{
				j1 = i1 + j1 + 2;
				i1 = 1;
			}
			return;
		}
		else 		{
			printf("%d %d %d\n", f2, i2, j2);
			i2++;
			j2--;
			if (j2<1 || i2>n) 			{
				j2 = i2 + j2 + 2;
				i2 = 1;
			}
			return;
		}
	}
	else if (i1 + j1 <= n + n) 	{
		printf("%d %d %d\n", f1, i1, j1);
		i1++;
		j1--;
		if (j1<1 || i1>n) 		{
			j1 = i1 + j1 + 2;
			i1 = 1;
		}
		return;
	}
	else if (i2 + j2 <= n + n) 	{
		printf("%d %d %d\n", f2, i2, j2);
		i2++;
		j2--;
		if (j2<1 || i2>n) 		{
			j2 = i2 + j2 + 2;
			i2 = 1;
		}
		return;
	}
}

int main()
{
	i1 = 1, j1 = 1, i2 = 2, j2 = 1;
	f1 = -1, f2 = -1;
	scanf("%d", &n);
	int kk = n * n;
	while (kk--) 	{
		solve();
	}
	return 0;
}
