/* 2051995 朱泽凯 济美 */

#ifndef  _hehepig166_16_b1_TStringAdv_
#define  _hehepig166_16_b1_TStringAdv_

#include "16-b1-TString.h"

class TStringAdv :public TString {

protected:
    void ins(const char *str, int pos);

public:

    TStringAdv() {}
    TStringAdv(const char *str) :TString(str) {}
    TStringAdv(const TString &str) :TString(str) {}



    //~TStringAdv();

    TStringAdv &assign(const TStringAdv &ts2);
    TStringAdv &assign(const TString &ts2);
    TStringAdv &assign(const char *s);
    TStringAdv &assign(const char &c);

    TStringAdv &append(const TStringAdv &ts2);
    TStringAdv &append(const TString &ts2);
    TStringAdv &append(const char *);
    TStringAdv &append(const char &c);

    TStringAdv &insert(const TStringAdv &ts2, int pos);
    TStringAdv &insert(const TString &ts2, int pos);
    TStringAdv &insert(const char *s, int pos);
    TStringAdv &insert(const char &c, int pos);

    TStringAdv &erase(const TStringAdv &ts2);
    TStringAdv &erase(const TString &ts2);
    TStringAdv &erase(const char *s);
    TStringAdv &erase(const char &c);

    TStringAdv substr(const int pos, const int len=-19260817) const;

    char &at(const int n);




    TStringAdv operator=(const char *str);
    TStringAdv operator=(const TString &str);
    TStringAdv operator=(const TStringAdv &str);

    TStringAdv &operator+=(const char *str);
    TStringAdv &operator+=(const TString &str);
    TStringAdv &operator+=(const char ch);
    TStringAdv &operator+=(const TStringAdv &str);

    friend TStringAdv operator+(const TStringAdv &LHS, const TStringAdv &RHS);
    friend TStringAdv operator+(const TStringAdv &LHS, const char *RHS);
    friend TStringAdv operator+(const TStringAdv &LHS, const char RHS);
    friend TStringAdv operator+(const char *LHS, const TStringAdv &RHS);
    friend TStringAdv operator+(const char LHS, const TStringAdv &RHS);
    friend TStringAdv operator+(const TString LHS, const TStringAdv &RHS);  //TString没有引用了
    friend TStringAdv operator+(const TStringAdv &LHS, const TString RHS);  //


    TStringAdv &operator-=(const char *str);
    TStringAdv &operator-=(const TString &str);
    TStringAdv &operator-=(const char ch);
    TStringAdv &operator-=(const TStringAdv &str);

    friend TStringAdv operator-(const TStringAdv &LHS, const TStringAdv &RHS);
    friend TStringAdv operator-(const TStringAdv &LHS, const char *RHS);
    friend TStringAdv operator-(const TStringAdv &LHS, const char RHS);
    friend TStringAdv operator-(const char *LHS, const TStringAdv &RHS);
    friend TStringAdv operator-(const char LHS, const TStringAdv &RHS);
    friend TStringAdv operator-(const TString LHS, const TStringAdv &RHS);  // TString没有引用了
    friend TStringAdv operator-(const TStringAdv &LHS, const TString RHS);  //

    TStringAdv operator*(const int n) const;
    TStringAdv &operator*=(const int n);
    const TStringAdv operator!() const;


};



#endif