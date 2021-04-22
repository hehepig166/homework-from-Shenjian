/* 2051995 ���� ���� */

#pragma once

#include <iostream>
using namespace std;

/* ���������ȫ�ֺ�����Ҫ������д�ڴ˴� */

//�������򷵻�1������0
bool runNian(int x);

//�Ƿ��·��򷵻�-1
int maxDay(int m, int y);

//1..y ��������
int cntRun(int y);


/* �������Ҫ�ĺ궨�塢ֻ��ȫ�ֱ����ȣ�д�ڴ˴� */
#define YEAR_MIN 1900
#define YEAR_MAX 2099
#define YEAR_DEF 2000
#define CD_MIN 1
#define CD_MAX 73049

/* ��ȫDate��Ķ��壬���г�Ա����������ʵ�֣���Ҫ�ڴ˴�����ʵ�� */
class Date {
private:
	int year;
	int month;
	int day;
	/* ������������ݳ�Ա */
public:
	/* ������Ҫ��������ĳ�Ա��������Ԫ������(������������ݳ�Ա) */

	//���캯��
	Date();	//Ĭ��2020.1.1
	Date(const Date &date);
	Date(int _year, int _month, int _day);

	//countDay<=1:		1900.1.1
	//countDay>=73049:	2099.12.31
	//����Date(44302):	2021.4.17
	Date(int countDay);


	//���� 0 �򲻱�
	//������˳���жϣ����ַǷ�����һ����ΪĬ��ֵ
	void set(const int _year, const int _month = 1, const int _day = 1);

	//ȡ����ǰ����
	void get(int &_year, int &_month, int &_day);

	//����2020��1��1��
	void show();

	//ת������
	//��:int(Date(1900,1,1)):	1
	//��:int(Date(2021,4,17)):  44302
	operator int() const;

	//��������

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

