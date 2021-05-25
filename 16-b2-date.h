/* 2051995 ���� ���� */
#ifndef _hehepig_16_b2_date_
#define _hehepig_16_b2_date_


#include <iostream>
#include "16-b2-date.h"
using namespace std;

#define YEAR_MIN (1900)
#define YEAR_MAX (2099)
#define YEAR_DEF (2000)
#define CD_MIN (1)
#define CD_MAX (73049)
#define CD_HA ( CD_MAX - CD_MIN +1 )

/* ��ȫDate��Ķ��壬���г�Ա����������ʵ�֣���Ҫ�ڴ˴�����ʵ�� */
class Date {
protected:
    int year;
    int month;
    int day;
    /* ������������ݳ�Ա */

    //�������򷵻�1������0
    bool runNian(int x) const;

    //�Ƿ��·��򷵻�-1
    int maxDay(int m, int y) const;

    //1..y ��������
    int cntRun(int y) const;

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


    //0Ҳ�ǷǷ�ֵ
    //������˳���жϣ����ַǷ�����һ����ΪĬ��ֵ
    void set(const int _year, const int _month = 1, const int _day = 1);

    //ȡ����ǰ����
    void get(int &_year, int &_month, int &_day) const;

    //����2020��1��1��
    void show()const;

    //ת������
    //��:int(Date(1900,1,1)):	1
    //��:int(Date(2021,4,17)):  44302
    operator int() const;

    //��������

    friend Date operator+(long long LHS, const Date &RHS);
    friend Date operator+(int LHS, const Date &RHS);

    friend Date operator+(const Date &LHS, long long RHS);
    friend Date operator+(const Date &LHS, int RHS);

    friend int operator-(const Date &LHS, const Date &RHS);

    friend Date operator-(const Date &LHS, long long RHS);
    friend Date operator-(const Date &LHS, int RHS);

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






#endif