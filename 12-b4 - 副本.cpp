/* ���� 2051995 ���� */
#include <iostream>
#include <cmath>
using namespace std;

double definite_integration(double (*fun)(double), double low, double high, int n)
{
    double ret = 0, eps = (high-low) / n;
    while (n--)
    {
        ret += fun(low += eps)*eps;
    }
    return ret;
}

int main()
{
    int n;
    double low, high, value;
    
    cout << "������sinxdx�����ޡ����޼����仮������" << endl;
    cin >> low >> high >> n;
    value = definite_integration(sin, low, high, n);
    cout << "sinxdx[" << low << "~" << high << "/n=" << n << "] : " << value << endl;

    cout << "������cosxdx�����ޡ����޼����仮������" << endl;
    cin >> low >> high >> n;
    value = definite_integration(cos, low, high, n);
    cout << "cosxdx[" << low << "~" << high << "/n=" << n << "] : " << value << endl;

    cout << "������e^xdx�����ޡ����޼����仮������" << endl;
    cin >> low >> high >> n;
    value = definite_integration(exp, low, high, n);
    cout << "e^xdx[" << low << "~" << high << "/n=" << n << "] : " << value << endl;

    return 0;
}