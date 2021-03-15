#include <cstdio>
#include <iostream>
#include <cmath>
using namespace std;

template <typename T>
void printBits(T x)
{
	unsigned long long p, k, i, j;
	long long f, e, s;
	int flen, elen, len;
	if (sizeof(T) == 8)
	{
		cout << "> double( " << x << " )" << "\n\n";
		flen = 52, elen = 11, len = 64, p = *((unsigned long long*) & x);
	}
	else
	{
		cout << "> float( " << x << " )" << "\n\n";
		flen = 23, elen = 8, len = 32, p = *((unsigned int*)&x);
	}

	for (i = len / 8, k = 1ull << (len - 1); i; i--, printf(" "))
		for (j = 0; j < 8; j++, k >>= 1)
			printf("%c", p & k ? '1' : '0');

	printf("\n\nf= %17lld   (", f = p & ((1ull << flen) - 1));
	for (i = flen, k = 1ull << (flen - 1); i; i--, k >>= 1)
		printf("%c", p & k ? '1' : '0');
	printf(")\ne= %17lld   (", e = (p >> flen) & ((1ull << elen) - 1));
	for (i = elen, k = 1ull << (len - 2); i; i--, k >>= 1)
		printf("%c", p & k ? '1' : '0');
	printf(")\ns= %17lld\n\n", s = (p & (1ull << (len - 1)) ? 1 : 0));
	printf("%.6f = (-1)^s * (1+f*2^(-%d)) * 2^(e-%d)\n\n",
		pow(-1, s) * (1 + f * pow(2, -flen)) * pow(2, e - ((1ll << (elen - 1)) - 1)),
		flen, (1 << (elen - 1)) - 1);

	puts("\n\n");
	/*
	for (char* p = (char*)&x + sizeof(x) - 1; p >= (char*)&x; p--)
		dfs((unsigned char)*p, 8);
	puts("\n\n\n\n\n");
	*/

}

int main()
{
	printBits(float(123.456));
	printBits(float(-123.456));
	printBits(float(0.123e-3));
	printBits(float(-1.23e-4));

	printBits(double(123.456));
	printBits(double(-123.456));
	printBits(double(0.123e-3));
	printBits(double(-1.23e-4));
	return 0;
}