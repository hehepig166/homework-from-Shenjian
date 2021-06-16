/* 2051995 朱泽凯 济美 */

#include <iostream>
#include <cmath>
using namespace std;

#define PI (3.14159265358)

class Shape {
protected:

public:
    virtual void ShapeName() = 0;
    virtual void area() = 0;
};


class Circle : public Shape {
protected:
    double Radius;

public:

    Circle() :Radius(0) {}
    Circle(double r) :Radius(r)
    {
        if (r < 0)
            *this = Circle();
    }

    virtual void ShapeName()
    {
        cout << "圆" << endl;
    }
    virtual void area()
    {
        cout << (PI * Radius * Radius) << endl;
    }
};


class Square : public Shape {
protected:
    double SideLen;

public:

    Square() :SideLen(0) {}
    Square(double s) :SideLen(s)
    {
        if (s <= 0)
            *this = Square();
    }

    virtual void ShapeName()
    {
        cout << "正方形" << endl;
    }
    virtual void area()
    {
        cout << (SideLen * SideLen) << endl;
    }
};


class Rectangle : public Shape {
protected:
    double WidLen;
    double HeiLen;

public:

    Rectangle() :WidLen(0), HeiLen(0) {}
    Rectangle(double w, double h) :WidLen(w), HeiLen(h)
    {
        if (w <= 0 || h <= 0)
            *this = Rectangle();
    }

    virtual void ShapeName()
    {
        cout << "长方形" << endl;
    }
    virtual void area()
    {
        cout << (WidLen * HeiLen) << endl;
    }
};


class Trapezoid :public Shape {
protected:
    double UpperLen;
    double LowerLen;
    double HeiLen;
public:

    Trapezoid() :UpperLen(0), LowerLen(0), HeiLen(0) {}
    Trapezoid(double upper, double lower, double h) :UpperLen(upper), LowerLen(lower), HeiLen(h)
    {
        if (upper <= 0 || lower <= 0 || h <= 0)
            *this = Trapezoid();
    }

    virtual void ShapeName()
    {
        cout << "梯形" << endl;
    }
    virtual void area()
    {
        cout << ((UpperLen + LowerLen) * HeiLen / 2.0) << endl;
    }
};


class Triangle :public Shape {
protected:
    double SideLen[3];
public:

    Triangle() {
        for (int i = 0; i < 3; i++)
            SideLen[i] = 0;
    }
    Triangle(double a, double b, double c)
    {
        if (a <= 0 || b <= 0 || c <= 0) {
            for (int i = 0; i < 3; i++)
                SideLen[i] = 0;
        }
        else {
            SideLen[0] = a;
            SideLen[1] = b;
            SideLen[2] = c;
        }
    }

    virtual void ShapeName()
    {
        cout << "三角形" << endl;
    }
    virtual void area()
    {
        double &a(SideLen[0]), &b(SideLen[1]), &c(SideLen[2]);
        double p = (a + b + c) / 2.0;
        cout << (sqrt(p * (p - a) * (p - b) * (p - c))) << endl;
    }
};


int main()
{
    Circle c1(5.2);
    Square s1(5.2);
    Rectangle r1(5.2, 3.7);
    Trapezoid t1(2.3, 4.4, 3.8);
    Triangle t2(3, 4, 5);
    Shape *s[5] = { &c1,&s1,&r1,&t1,&t2 };

    int i;
    for (i = 0; i < 5; i++) {
        s[i]->ShapeName();
        s[i]->area();
    }

    return 0;
}
