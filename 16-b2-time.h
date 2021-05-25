/* 2051995 ���� ���� */
#ifndef _hehepig_16_b2_time_
#define _hehepig_16_b2_time_


#include <iostream>
#include "16-b2-time.h"
using namespace std;

//Ĭ��ֵΪ 00:00:00
#define DEF_SSS (0)
#define MAX_SSS (24ll*60*60)


class Time {

protected:
    int hour;
    int minute;
    int second;

    long long toSSS() const;    //��������ĵ�λ
    bool check_avalible(const long long hh_, const long long mm_, const long long ss_) const;

public:
    Time();
    Time(const Time &tm);
    Time(long long sss_);     //����Ϊ��λ��0Ϊ00:00:00��MAX_SSS��������Ϊ0
    Time(long long hour_, long long minute_, long long second_);    //�κ�һ����������Ϊ0

    //�Ƿ���Ϊ 00:00:00
    void set(const long long hh_ = 0, const long long mm_ = 0, const long long ss_ = 0);

    void get(int &hh_, int &mm_, int &ss_) const;

    //����12ʱ15��8��
    void show() const;

    //operator int() const;
    operator long long() const;

    template<typename T_INT>
    friend Time operator+(T_INT LHS, const Time &RHS);

    template<typename T_INT>
    friend Time operator+(const Time &LHS, T_INT RHS);

    friend long long operator- (const Time &LHS, const Time &RHS);

    template<typename T_INT>
    friend Time operator-(const Time &LHS, T_INT RHS);


    //++t
    Time &operator++();

    //--t
    Time &operator--();

    //t++
    Time operator++(int);

    //t--
    Time operator--(int);


    friend istream &operator>>(istream &in, Time &time);
    friend ostream &operator<<(ostream &out, const Time &time);
};



#endif