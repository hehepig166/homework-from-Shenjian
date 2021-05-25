/* 2051995 朱泽凯 济美 */

#include <iostream>

#include "16-b2-date.h"
#include "16-b2-time.h"
using namespace std;

#define LL long long

LL Time::toSSS() const
{
    LL ret = 0;
    ret += hour * 3600;
    ret += minute * 60;
    ret += second;
    return ret;
}

bool Time::check_avalible(const long long hh_, const long long mm_, const long long ss_) const
{
    return hh_ >= 0 && hh_ < 24 && mm_ >= 0 && mm_ < 60 && ss_ >= 0 && ss_ < 60;
}


Time::Time()
{
    hour = 0;
    minute = 0;
    second = 0;
}

Time::Time(const Time &tm)
{
    hour = tm.hour;
    minute = tm.minute;
    second = tm.second;
}

Time::Time(long long sss_)
{
    if (sss_ >= 0 && sss_ < MAX_SSS) {
        second = sss_ % 60;
        minute = (sss_ /= 60) % 60;
        hour = sss_ /= 60;
    }
    else {
        hour = 0;
        minute = 0;
        second = 0;
    }
}

Time::Time(long long hour_, long long minute_, long long second_)
{
    if (check_avalible(hour_, minute_, second_)) {
        hour = hour_;
        minute = minute_;
        second = second_;
    }
    else {
        hour = 0;
        minute = 0;
        second = 0;
    }
}


void Time::set(const long long hh_, const long long mm_, const long long ss_)
{
    if (check_avalible(hh_, mm_, ss_)) {
        hour = hh_;
        minute = mm_;
        second = ss_;
    }
    else {
        hour = 0;
        minute = 0;
        second = 0;
    }
}

void Time::get(int &hh_, int &mm_, int &ss_) const
{
    hh_ = hour;
    mm_ = minute;
    ss_ = second;
}

void Time::show() const
{
    cout << hour << "时" << minute << "分" << second << "秒";
}


/*
Time::operator int() const
{
    return int(toSSS());
}
*/

Time::operator long long() const
{
    return toSSS();
}


template<typename T_INT>
Time operator+(T_INT LHS, const Time &RHS)
{
    long long tmp = (long long)LHS + RHS.toSSS();
    tmp %= MAX_SSS;
    return Time(tmp);
}


template<typename T_INT>
Time operator+(const Time &LHS, T_INT RHS)
{
    long long tmp = LHS.toSSS() + (long long)RHS;
    tmp %= MAX_SSS;
    return Time(tmp);
}

long long operator-(const Time &LHS, const Time &RHS)
{
    return LHS.toSSS() - RHS.toSSS();
}

template<typename T_INT>
Time operator-(const Time &LHS, T_INT RHS)
{
    long long tmp = LHS.toSSS() - (long long)RHS;
    tmp = (tmp % MAX_SSS + MAX_SSS) % MAX_SSS;
    return Time(tmp);
}

Time &Time::operator++()
{
    *this = *this + 1;
    return *this;
}


Time &Time::operator--()
{
    *this = *this - 1;
    return *this;
}


Time Time::operator++(int)
{
    Time tmp(*this);
    *this = *this + 1;
    return tmp;
}


Time Time::operator--(int)
{
    Time tmp(*this);
    *this = *this - 1;
    return tmp;
}


istream &operator>>(istream &in, Time &time)
{
    long long h, m, s;
    in >> h >> m >> s;
    if (in.good()) {
        time.set(h, m, s);
    }
    else {
        time.set();
    }
    return in;
}

ostream &operator<<(ostream &out, const Time &time)
{
    out << time.hour << "时" << time.minute << "分" << time.second << "秒";
    return out;
}