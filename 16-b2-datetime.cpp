/* 2051995 ÷Ï‘Ûø≠ º√√¿ */

#include <iostream>

#include "16-b2-date.h"
#include "16-b2-time.h"
#include "16-b2-datetime.h"

using namespace std;


long long DateTime::toS() const
{
    long long dd(long long(Date(year, month, day)) - 1), tt(Time::toSSS());
    return dd * MAX_SSS + tt;
}

bool DateTime::check_avalible(long long year_, long long month_, long long day_, long long hour_, long long minute_, long long second_) const
{
    if (year_ > YEAR_MAX || year_ < YEAR_MIN) {
        return 0;
    }
    if (month_ > 12 || month_ < 1) {
        return 0;
    }
    if (day_ > maxDay(month_, year_) || day_ < 0) {
        return 0;
    }
    return Time::check_avalible(hour_, minute_, second_);
}


DateTime::DateTime(long long s_)
{
    if (s_ < 0 || s_ >= MAX_S)
        *this = DateTime();

    long long dd = s_ / MAX_SSS % CD_HA + 1, tt = s_ % MAX_SSS;
    *this = DateTime(Date(dd), Time(tt));
}

DateTime::DateTime(long long year_, long long month_, long long day_, long long hour_, long long minute_, long long second_)
{
    *this = DateTime(Date((int)year_, (int)month_, (int)day_), Time(hour_, minute_, second_));
}

void DateTime::set(long long year_, long long month_, long long day_, long long hour_, long long minute_, long long second_)
{
    if (check_avalible(year_, month_, day_, hour_, minute_, second_)) {
        year = year_;
        month = month_;
        day = day_;
        hour = hour_;
        minute = minute_;
        second = second_;
    }
}

void DateTime::get(int &year_, int &month_, int &day_, int &hour_, int &minute_, int &second_) const
{
    year_ = year;
    month_ = month;
    day_ = day;
    hour_ = hour;
    minute_ = minute;
    second_ = second;
}

DateTime::operator long long() const
{
    return toS();
}


const DateTime operator+(long long LHS, const DateTime &RHS)
{
    long long tmp = RHS.toS() + LHS;
    return DateTime(tmp % MAX_S);
}
const DateTime operator+(int LHS, const DateTime &RHS)
{
    return DateTime((RHS.toS() + LHS) % MAX_S);
}


const DateTime operator+(const DateTime &LHS, long long RHS)
{
    return DateTime((LHS.toS() + RHS) % MAX_S);
}
const DateTime operator+(const DateTime &LHS, int RHS)
{
    return DateTime((LHS.toS() + RHS) % MAX_S);
}


const long long operator-(const DateTime &LHS, const DateTime &RHS)
{
    return LHS.toS() - RHS.toS();
}
const long long operator-(const DateTime &LHS, const Date &RHS)
{
    return LHS.toS() - DateTime(RHS).toS();
}
const long long operator-(const DateTime &LHS, const Time &RHS)
{
    return LHS.toS() - DateTime(RHS).toS();
}
const long long operator-(const Date &LHS, const DateTime &RHS)
{
    return DateTime(LHS).toS(), RHS.toS();
}
const long long operator-(const Time &LHS, const DateTime &RHS)
{
    return DateTime(LHS).toS(), RHS.toS();
}

 const DateTime operator-(const DateTime &LHS, long long RHS)
{

}
 const DateTime operator-(const DateTime &LHS, int RHS);