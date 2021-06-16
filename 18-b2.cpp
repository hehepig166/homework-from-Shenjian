/* 朱泽凯 2051995 济美 */
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

double definite_integration(double (*fun)(double), double low, double high, int n)
{
    double ret = 0, eps = (high - low) / n;
    while (n--) {
        ret += fun(low += eps) * eps;
    }
    return ret;
}


class integral {
protected:
    int n;
    double low;
    double high;
    double (*fun)(double);
    string funName;

public:
    friend istream &operator>>(istream &in, integral &X)
    {
        cout << "请输入" << X.funName << "dx的下限、上限及区间划分数量" << endl;
        in >> X.low >> X.high >> X.n;
        return in;
    }
    double value()
    {
        cout << funName << "dx[" << low << "~" << high << "/n=" << n << "] : ";
        return definite_integration(fun, low, high, n);
    }
};

void fun_integral(integral &fRef)
{
    cin >> fRef;
    cout << fRef.value() << endl;
    return;
}


class integral_sin :public integral {
public:
    integral_sin()
    {
        funName = "sin";
        fun = sin;
    }
};

class integral_cos :public integral {
public:
    integral_cos()
    {
        funName = "cos";
        fun = cos;
    }
};

class integral_exp :public integral {
public:
    integral_exp()
    {
        funName = "e^x";
        fun = exp;
    }
};

int main()
{
    integral_sin s1;
    integral_cos s2;
    integral_exp s3;

    fun_integral(s1);
    fun_integral(s2);
    fun_integral(s3);

    return 0;
}