/* 朱泽凯 2051995 济美 */
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
    
    cout << "请输入sinxdx的下限、上限及区间划分数量" << endl;
    cin >> low >> high >> n;
    value = definite_integration(sin, low, high, n);
    cout << "sinxdx[" << low << "~" << high << "/n=" << n << "] : " << value << endl;

    cout << "请输入cosxdx的下限、上限及区间划分数量" << endl;
    cin >> low >> high >> n;
    value = definite_integration(cos, low, high, n);
    cout << "cosxdx[" << low << "~" << high << "/n=" << n << "] : " << value << endl;

    cout << "请输入e^xdx的下限、上限及区间划分数量" << endl;
    cin >> low >> high >> n;
    value = definite_integration(exp, low, high, n);
    cout << "e^xdx[" << low << "~" << high << "/n=" << n << "] : " << value << endl;

    return 0;
}