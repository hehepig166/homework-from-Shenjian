/* 2051995 ���� ���� */

#ifndef _hehepig166_14_b5_
#define _hehepig166_14_b5_

#include <iostream>
using namespace std;

/* ��ȫTString��Ķ��壬���г�Ա����������ʵ�֣���Ҫ�ڴ˴�����ʵ�� */
class TString {
private:
    char *content;
    int   len;

    /* ������Ҫ������������ݳ�Ա����Ա��������Ԫ������ */

    TString(char *str, const int _len);

    inline void release();  //�ͷſռ䣬len����
    inline void init(const char *str);  //�ú����������ڴ��ͷţ�������str�����޸�content��len�����ڹ��캯����
    inline void rplus(const char *str);
    inline void lplus(const char *str);
    inline int find_baoli(const char *T, const int Tlen);
    inline int find_KMP(const char *T, const int Tlen, int *nxt);
    inline void del(const char *str);   //ɾ����һ��ƥ�䵽�ģ��±���С����û���򲻲���
    inline char* multiply(const int n);     //����
    void exitERR(const char *hint);

public:
    /* ������Ҫ������������ݳ�Ա����Ա��������Ԫ������ */

    TString();
    TString(const char *str);
    TString(const TString &str);
    TString(TString &&str);

    ~TString();

    void rotate(const int k);     //�����ַ�Ԫ������������ k λ����״�������Ļص���ǰ�ˣ�

    int find(const char *T, int Tlen);  //�ҵ��򷵻ص�һ�����±꣨��Ӧ T[0]����û�ҵ��򷵻�-1

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

/* ���������ȫ�ֺ�����Ҫ������д�ڴ˴� */
int TStringLen(const TString &str);


/* �������Ҫ�ĺ궨�塢ֻ��ȫ�ֱ����ȣ�д�ڴ˴� */

#endif