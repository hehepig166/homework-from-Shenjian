/* 2051995 朱泽凯 济美 */

#include "16-b2-date.h"
#include <iostream>
using namespace std;

static const int MD[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

//是闰年则返回1，否则0
bool Date::runNian(int x)const
{
    return ((x % 4 == 0 && x % 100 != 0) || x % 400 == 0);
}

int Date::maxDay(int m, int y)const
{
    if (m < 1 || m>12)
        return -1;
    else if (m == 2)
        return 28 + runNian(y);
    else
        return MD[m];
}

int Date::cntRun(int y)const
{
    return y / 4 - y / 100 + y / 400;
}


Date::Date() : year(YEAR_DEF), month(1), day(1) {}

Date::Date(const Date &date)
{
    *this = date;
}

Date::Date(int _year, int _month, int _day) : year(_year), month(_month), day(_day)
{
    if (year > YEAR_MAX || year < YEAR_MIN) {
        year = YEAR_DEF;
    }
    if (month > 12 || month < 1) {
        month = 1;
    }
    if (day > maxDay(month, year) || day < 0) {
        day = 1;
    }
}

//四年一闰，百年不闰，四百年再闰
Date::Date(int countDay)
{
    //边界与越界拎出来讨论
    if (countDay <= CD_MIN) {
        year = YEAR_MIN;
        month = 1;
        day = 1;
        return;
    }
    if (countDay >= CD_MAX) {
        year = YEAR_MAX;
        month = 12;
        day = 31;
        return;
    }

    countDay = max(countDay, CD_MIN);
    countDay = min(countDay, CD_MAX);

    int y = YEAR_MIN, m = 1, d, tmp = 0;    //tmp为补偿

    y += countDay / 365;
    countDay %= 365;

    while (countDay > maxDay(m, y)) {
        countDay -= maxDay(m, y);
        m++;
    }

    d = countDay;

    year = y;
    month = m;
    day = d;

    //补偿（不算当前年）
    tmp = cntRun(y - 1) - cntRun(YEAR_MIN - 1);
    for (int i = 1; i <= tmp; i++) {
        --(*this);
    }
}

void Date::set(const int _year, const int _month, const int _day)
{
    if (_year != 0) {
        year = (_year >= YEAR_MIN && _year <= YEAR_MAX) ? _year : YEAR_DEF;
    }

    if (_month != 0) {
        month = (_month >= 1 && _month <= 12) ? _month : 1;
    }

    if (_day != 0) {
        day = _day;
    }
    if (day<1 || day>maxDay(month, year)) {
        day = 1;
    }
}

void Date::get(int &_year, int &_month, int &_day) const
{
    _year = year;
    _month = month;
    _day = day;
}

void Date::show() const
{
    cout << year << "年" << month << "月" << day << "日" << endl;
}

Date::operator int() const
{
    int y = year, m = month, d = day;
    int ret = 0, tmp;

    ret += d;
    d = 1;
    for (int i = 1; i < m; i++)
        ret += maxDay(i, y);

    tmp = 365 * (y - YEAR_MIN) + cntRun(y - 1) - cntRun(YEAR_MIN - 1);
    ret += tmp;

    return ret;
}

Date operator+(long long LHS, const Date &RHS)
{
    return Date(((LHS + int(RHS) - CD_MIN) % CD_HA + CD_HA) % CD_HA + CD_MIN);
}
Date operator+(int LHS, const Date &RHS)
{
    return Date(((LHS + int(RHS) - CD_MIN) % CD_HA + CD_HA) % CD_HA + CD_MIN);
}

Date operator+(const Date &LHS, long long RHS)
{
    return Date(((RHS + int(LHS) - CD_MIN) % CD_HA + CD_HA) % CD_HA + CD_MIN);
}
Date operator+(const Date &LHS, int RHS)
{
    return Date(((RHS + int(LHS) - CD_MIN) % CD_HA + CD_HA) % CD_HA + CD_MIN);
}

int operator-(const Date &LHS, const Date &RHS)
{
    return int(LHS) - int(RHS);
}

Date operator-(const Date &LHS, long long RHS)
{
    return Date((((int(LHS) - RHS - CD_MIN) % CD_HA) + CD_HA) % CD_HA + CD_MIN);
}
Date operator-(const Date &LHS, int RHS)
{
    return Date((((int(LHS) - RHS - CD_MIN) % CD_HA) + CD_HA) % CD_HA + CD_MIN);
}


Date &Date::operator++()
{
    //等于上限，返回最小值（循环）
    if (year == YEAR_MAX && month == 12 && day == 31) {
        *this = Date(CD_MIN);
        return *this;
    }

    if (++day > maxDay(month, year)) {
        if (++month > 12) {
            year++;
            month = 1;
        }
        day = 1;
    }

    return *this;
}

Date &Date::operator--()
{
    //等于下限，返回最大值（循环）
    if (year == YEAR_MIN && month == 1 && day == 1) {
        *this = Date(CD_MAX);
        return *this;
    }

    if (--day <= 0) {
        if (--month <= 0) {
            year--;
            month = 12;
        }
        day = maxDay(month, year);
    }

    return *this;
}

Date Date::operator++(int)
{
    Date tmp = *this;
    ++ *this;
    return tmp;
}

Date Date::operator--(int)
{
    Date tmp = *this;
    -- *this;
    return tmp;
}

istream &operator>>(istream &in, Date &date)
{
    int y, m, d;
    in >> y >> m >> d;
    if (cin.good())
        date = Date(y, m, d);
    else
        date = Date();
    return in;
}

ostream &operator<<(ostream &out, const Date &date)
{
    out << date.year << "年" << date.month << "月" << date.day << "日";
    return out;
}