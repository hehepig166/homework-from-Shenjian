/* 2051995 朱泽凯 济美 */

#pragma once

#include <iostream>
using namespace std;

/* 如果有其它全局函数需要声明，写于此处 */

//是闰年则返回1，否则0
bool runNian(int x);

//非法月份则返回-1
int maxDay(int m, int y);

//1..y 中闰年数
int cntRun(int y);


/* 如果有需要的宏定义、只读全局变量等，写于此处 */
#define YEAR_MIN 1900
#define YEAR_MAX 2099
#define YEAR_DEF 2000
#define CD_MIN 1
#define CD_MAX 73049

/* 补全Date类的定义，所有成员函数均体外实现，不要在此处体内实现 */
class Date {
private:
	int year;
	int month;
	int day;
	/* 不允许添加数据成员 */
public:
	/* 根据需要定义所需的成员函数、友元函数等(不允许添加数据成员) */

	//构造函数
	Date();	//默认2020.1.1
	Date(const Date &date);
	Date(int _year, int _month, int _day);

	//countDay<=1:		1900.1.1
	//countDay>=73049:	2099.12.31
	//例：Date(44302):	2021.4.17
	Date(int countDay);


	//若是 0 则不变
	//年月日顺序判断，出现非法，那一项设为默认值
	void set(const int _year, const int _month = 1, const int _day = 1);

	//取对象当前日期
	void get(int &_year, int &_month, int &_day);

	//例：2020年1月1日
	void show();

	//转换函数
	//例:int(Date(1900,1,1)):	1
	//例:int(Date(2021,4,17)):  44302
	operator int() const;

	//重载运算

	friend Date operator+(const int &LHS, const Date &RHS);
	friend Date operator+(const Date &LHS, const int &RHS);
	friend int operator-(const Date &LHS, const Date &RHS);
	friend Date operator-(const Date &LHS, const int &RHS);

	//++d
	Date &operator++();

	//--d
	Date &operator--();

	//d++
	Date operator++(int);

	//d--
	Date operator--(int);

	friend istream &operator>>(istream &in, Date &date);
	friend ostream &operator<<(ostream &out, const Date &date);

};

