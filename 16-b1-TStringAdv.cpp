/* 2051995 ÷Ï‘Ûø≠ º√√¿ */
#include <cstdlib>
#include <cstring>
#include "16-b1-TString.h"
#include "16-b1-TStringAdv.h"
using namespace std;



void TStringAdv::ins(const char *str, int pos)
{
    if (pos<1 || pos>len + 1) {
        return;
    }
    if (str == NULL || *str == 0) {
        return;
    }

    int dlen = strlen(str);
    int newlen = len + dlen;

    char *buf = (char *)malloc((newlen + 1) * sizeof(char));

    if (buf == NULL) {  //…Í«Î ß∞‹
        return;
    }

    for (int i = pos - 2; i >= 0; i--)
        buf[i] = content[i];
    for (int i = pos - 1, j = 0; j < dlen; i++, j++)
        buf[i] = str[j];
    for (int i = pos + dlen - 1, j = pos - 1; i < newlen; i++, j++)
        buf[i] = content[j];


    buf[newlen] = '\0'; //÷ÿ“™

    release();  //÷ÿ“™

    content = buf;
    len = newlen;
}



TStringAdv &TStringAdv::assign(const TStringAdv &ts2)
{
    init(ts2.content);
    return *this;

}
TStringAdv &TStringAdv::assign(const TString &ts2)
{
    init(ts2.c_str());
    return *this;
}
TStringAdv &TStringAdv::assign(const char *s)
{
    init(s);
    return *this;
}
TStringAdv &TStringAdv::assign(const char &c)
{
    char tmp[2] = { c, 0 };
    init(tmp);
    return *this;
}




TStringAdv &TStringAdv::append(const TStringAdv &ts2)
{
    rplus(ts2.content);
    return *this;
}
TStringAdv &TStringAdv::append(const TString &ts2)
{
    rplus(ts2.c_str());
    return *this;
}
TStringAdv &TStringAdv::append(const char *s)
{
    rplus(s);
    return *this;
}
TStringAdv &TStringAdv::append(const char &c)
{
    char tmp[2] = { c, 0 };
    rplus(tmp);
    return *this;
}




TStringAdv &TStringAdv::insert(const TStringAdv &ts2, int pos)
{
    ins(ts2.content, pos);
    return *this;
}
TStringAdv &TStringAdv::insert(const TString &ts2, int pos)
{
    ins(ts2.c_str(), pos);
    return *this;
}
TStringAdv &TStringAdv::insert(const char *s, int pos)
{
    ins(s, pos);
    return *this;
}
TStringAdv &TStringAdv::insert(const char &c, int pos)
{
    char tmp[2] = { c, 0 };
    ins(tmp, pos);
    return *this;
}



TStringAdv &TStringAdv::erase(const TStringAdv &ts2)
{
    *this -= ts2;
    return *this;
}
TStringAdv &TStringAdv::erase(const TString &ts2)
{
    *this -= ts2;
    return *this;
}
TStringAdv &TStringAdv::erase(const char *s)
{
    *this -= s;
    return *this;
}
TStringAdv &TStringAdv::erase(const char &c)
{
    *this -= c;
    return *this;
}
TStringAdv TStringAdv::substr(const int pos, const int len_) const
{

    if (pos<=0 || (len_!=-19260817 && len_<=0) || pos>this->len)
        return TStringAdv();

    int cnt = len_ == -19260817 ? len : len_;
    TString ret;
    const char *p = this->content;
    p += (pos - 1);
    while (cnt-- && *p) {
        ret += *p;
        p++;
    }
    return ret;
}

char &TStringAdv::at(const int n)
{
    return this->content[n];
}














TStringAdv TStringAdv::operator=(const char *str)
{
    init(str);
    return *this;
}
TStringAdv TStringAdv::operator=(const TString &str)
{
    init(str.c_str());
    return *this;
}
TStringAdv TStringAdv::operator=(const TStringAdv &str)
{
    init(str.content);
    return *this;
}

TStringAdv &TStringAdv::operator+=(const char *str)
{
    rplus(str);
    return *this;
}
TStringAdv &TStringAdv::operator+=(const TString &str)
{
    rplus(str.c_str());
    return *this;
}
TStringAdv &TStringAdv::operator += (const char ch)
{
    char tmp[2] = { ch, '\0' };
    rplus(tmp);
    return *this;
}
TStringAdv &TStringAdv::operator+=(const TStringAdv &str)
{
    rplus(str.content);
    return *this;
}

TStringAdv operator+(const TStringAdv &LHS, const TStringAdv &RHS)
{
    TStringAdv ret(LHS);
    ret.rplus(RHS.content);
    return ret;
}
TStringAdv operator+(const TStringAdv &LHS, const char *RHS)
{
    TStringAdv ret(LHS);
    ret.rplus(RHS);
    return ret;
}
TStringAdv operator+(const TStringAdv &LHS, const char RHS)
{
    char tmp[2] = { RHS, '\0' };
    TStringAdv ret(LHS);
    ret.rplus(tmp);
    return ret;
}
TStringAdv operator+(const char *LHS, const TStringAdv &RHS)
{
    TStringAdv ret(RHS);
    ret.lplus(LHS);
    return ret;
}
TStringAdv operator+(const char LHS, const TStringAdv &RHS)
{
    char tmp[2] = { LHS, '\0' };
    TStringAdv ret(RHS);
    ret.lplus(tmp);
    return ret;
}
 TStringAdv operator+(const TString LHS, const TStringAdv &RHS)
{
    TStringAdv ret(LHS);
    ret.rplus(RHS.content);
    return ret;
}
 TStringAdv operator+(const TStringAdv &LHS, const TString RHS)
{
     TStringAdv ret(LHS);
     ret.rplus(RHS.c_str());
     return ret;
}


TStringAdv &TStringAdv::operator-=(const char *str)
{
    del(str);
    return *this;
}
TStringAdv &TStringAdv::operator-=(const TString &str)
{
    del(str.c_str());
    return *this;
}
TStringAdv &TStringAdv::operator-=(const char ch)
{
    char tmp[2] = { ch, '\0' };
    del(tmp);
    return *this;
}
TStringAdv &TStringAdv::operator-=(const TStringAdv &str)
{
    del(str.content);
    return *this;
}

TStringAdv operator-(const TStringAdv &LHS, const TStringAdv &RHS)
{
    TStringAdv ret(LHS);
    ret.del(RHS.content);
    return ret;
}
TStringAdv operator-(const TStringAdv &LHS, const char *RHS)
{
    TStringAdv ret(LHS);
    ret.del(RHS);
    return ret;
}
TStringAdv operator-(const TStringAdv &LHS, const char RHS)
{
    char tmp[2] = { RHS, '\0' };
    TStringAdv ret(LHS);
    ret.del(tmp);
    return ret;
}
TStringAdv operator-(const char *LHS, const TStringAdv &RHS)
{
    TStringAdv ret(LHS);
    ret.del(RHS.content);
    return ret;
}
TStringAdv operator-(const char LHS, const TStringAdv &RHS)
{
    char tmp[2] = { LHS, '\0' };
    TStringAdv ret(tmp);
    ret.del(RHS.content);
    return ret;
}
 TStringAdv operator-(const TString LHS, const TStringAdv &RHS)
{
    TStringAdv ret(LHS);
    ret.del(RHS.content);
    return ret;
}
 TStringAdv operator-(const TStringAdv &LHS, const TString RHS)
 {
     TStringAdv ret(LHS);
     ret.del(RHS.c_str());
     return ret;
 }

TStringAdv TStringAdv::operator*(const int n) const
{
    TStringAdv ret;
    char *p = this->multiply(n);
    if (p == NULL) {
        ret.content = NULL;
        ret.len = 0;
        return ret;
    }
    else {
        ret.content = p;
        ret.len = this->len * n;
        return ret;
    }
}
TStringAdv &TStringAdv::operator*=(const int n)
{
    char *p = this->multiply(n);
    if (p != NULL) {
        int newlen = len * n;
        release();
        content = p;
        len = newlen;
    }
    return *this;
}
const TStringAdv TStringAdv::operator!() const
{
    char *p = (char *)malloc((len + 1) * sizeof(char));

    if (p == NULL) {
        return TStringAdv();   //…Í«Î ß∞‹
    }

    for (int l = 0, r = len - 1; l <= r; l++, r--) {
        p[l] = content[r];
        p[r] = content[l];
    }
    p[len] = '\0';

    TStringAdv ret;
    ret.release();
    ret.content = p;
    ret.len = this->len;

    return ret;
}