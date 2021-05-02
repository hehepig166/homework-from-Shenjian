/* 2051995 朱泽凯 济美 */

#ifndef _hehepig166_14_b5_
#define _hehepig166_14_b5_

#include <iostream>
using namespace std;

/* 补全TString类的定义，所有成员函数均体外实现，不要在此处体内实现 */
class TString {
private:
    char *content;
    int   len;

    /* 根据需要定义所需的数据成员、成员函数、友元函数等 */

    TString(char *str, const int _len);

    inline void release();  //释放空间，len置零
    inline void init(const char *str);  //该函数不考虑内存释放，仅根据str重新修改content和len（用于构造函数）
    inline void rplus(const char *str);
    inline void lplus(const char *str);
    inline int find_baoli(const char *T, const int Tlen);
    inline int find_KMP(const char *T, const int Tlen, int *nxt);
    inline void del(const char *str);   //删除第一个匹配到的（下标最小），没有则不操作
    inline char* multiply(const int n);     //复制
    void exitERR(const char *hint);

public:
    /* 根据需要定义所需的数据成员、成员函数、友元函数等 */

    TString();
    TString(const char *str);
    TString(const TString &str);
    TString(TString &&str);

    ~TString();

    void rotate(const int k);     //串中字符元素整体向右移 k 位（环状，超过的回到最前端）

    int find(const char *T, int Tlen);  //找到则返回第一个的下标（对应 T[0]），没找到则返回-1

    int length() const;

    char *c_str() const;

    char &operator[](int index);

    TString operator=(const char *str);
    TString operator=(const TString &str);
    TString operator=(TString &&str);

    TString &operator+=(const char *str);
    TString &operator+=(const TString &str);
    TString &operator+=(const char ch);

    friend TString operator+(const TString &LHS, const TString &RHS);
    friend TString operator+(const TString &LHS, const char *RHS);
    friend TString operator+(const TString &LHS, const char RHS);
    friend TString operator+(const char *LHS, const TString &RHS);
    friend TString operator+(const char LHS, const TString &RHS);
    friend TString operator+(TString &&LHS, const TString &RHS);
    friend TString operator+(TString &&LHS, const char *RHS);
    friend TString operator+(TString &&LHS, const char RHS);

    
    TString &operator-=(const char *str);
    TString &operator-=(const TString &str);
    TString &operator-=(const char ch);

    friend TString operator-(const TString &LHS, const TString &RHS);
    friend TString operator-(const TString &LHS, const char *RHS);
    friend TString operator-(const TString &LHS, const char RHS);
    friend TString operator-(TString &&LHS, const TString &RHS);
    friend TString operator-(TString &&LHS, const char *RHS);
    friend TString operator-(TString &&LHS, const char RHS);

    TString operator*(const int n);
    TString &operator*=(const int n);
    TString operator!();

    friend bool operator>(const TString &LHS, const TString &RHS);
    friend bool operator>(const TString &LHS, const char *RHS);
    friend bool operator>(const char *LHS, const TString &RHS);

    friend bool operator<(const TString &LHS, const TString &RHS);
    friend bool operator<(const TString &LHS, const char *RHS);
    friend bool operator<(const char *LHS, const TString &RHS);

    friend bool operator>=(const TString &LHS, const TString &RHS);
    friend bool operator>=(const TString &LHS, const char *RHS);
    friend bool operator>=(const char *LHS, const TString &RHS);

    friend bool operator<=(const TString &LHS, const TString &RHS);
    friend bool operator<=(const TString &LHS, const char *RHS);
    friend bool operator<=(const char *LHS, const TString &RHS);

    friend bool operator==(const TString &LHS, const TString &RHS);
    friend bool operator==(const TString &LHS, const char *RHS);
    friend bool operator==(const char *LHS, const TString &RHS);

    friend bool operator!=(const TString &LHS, const TString &RHS);
    friend bool operator!=(const TString &LHS, const char *RHS);
    friend bool operator!=(const char *LHS, const TString &RHS);

    
    friend istream &operator>>(istream &in, TString &str);
    friend ostream &operator<<(ostream &out, const TString &str);
};

/* 如果有其它全局函数需要声明，写于此处 */
int TStringLen(const TString &str);


/* 如果有需要的宏定义、只读全局变量等，写于此处 */

#endif