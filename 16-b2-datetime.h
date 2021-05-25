/* 2051995 朱泽凯 济美 */
#ifndef _hehepig_16_b2_datetime_
#define _hehepig_16_b2_datetime_

#include <iostream>

#include "16-b2-date.h"
#include "16-b2-time.h"
#include "16-b2-datetime.h"

using namespace std;


#define MAX_S ((CD_MAX)*(MAX_SSS))


class DateTime :public Date, public Time {

protected:
    long long toS() const;  //返回以秒为单位
    bool check_avalible(long long year_, long long month_, long long day_, long long hour_, long long minute_, long long second_) const;


public:
    DateTime() :Date(), Time() {}
    DateTime(const Date &date) :Date(date), Time(){}
    DateTime(const Time &time) :Date(), Time(time){}
    DateTime(const Date &date, const Time &time) :Date(date), Time(time) {}
    DateTime(const DateTime &dt) :Date(dt.year, dt.month, dt.day), Time(dt.hour, dt.minute, dt.second) {}
    DateTime(long long s_);
    DateTime(long long year_, long long month_, long long day_, long long hour_, long long minute_, long long second_);


    void set(long long year_ = 0, long long month_ = 0, long long day_ = 0, long long hour_ = 0, long long minute_ = 0, long long second_ = 0);

    void get(int &year_, int &month_, int &day_, int &hour_, int &minute_, int &second_) const;

    operator long long() const;


    friend const DateTime operator+(long long LHS, const DateTime &RHS);
    friend const DateTime operator+(int LHS, const DateTime &RHS);

    friend const DateTime operator+(const DateTime &LHS, long long RHS);
    friend const DateTime operator+(const DateTime &LHS, int RHS);


    friend const long long operator-(const DateTime &LHS, const DateTime &RHS);
    friend const long long operator-(const DateTime &LHS, const Date &RHS);
    friend const long long operator-(const DateTime &LHS, const Time &RHS);
    friend const long long operator-(const Date &LHS, const DateTime &RHS);
    friend const long long operator-(const Time &LHS, const DateTime &RHS);

    friend const DateTime operator-(const DateTime &LHS, long long RHS);
    friend const DateTime operator-(const DateTime &LHS, int RHS);

    friend const long long operator-(const DateTime &LHS, const DateTime &RHS);
    friend const long long operator-(const DateTime &LHS, const Date &RHS);
    friend const long long operator-(const DateTime &LHS, const Time &RHS);
    friend const long long operator-(const Date &LHS, const DateTime &RHS);
    friend const long long operator-(const Time &LHS, const DateTime &RHS);



    //++dt
    DateTime &operator++();

    //--dt
    DateTime &operator--();

    //dt++
    DateTime operator++(int);

    //dt--
    DateTime operator--(int);

    friend istream &operator>>(istream &in, DateTime &dt);
    friend ostream &operator<<(ostream &out, const DateTime &dt);
};





#endif

