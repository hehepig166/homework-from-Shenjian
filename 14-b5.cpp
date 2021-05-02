/* 2051995 ���� ���� */

/* ���������Ҫ��ͷ�ļ����궨��� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <cstdlib>
#include "14-b5.h"
using namespace std;

#define TMPSIZE 32

/* ���� TString ������г�Ա����������ʵ�� */



//4�ֹ��캯��============================================
TString::TString() :content(NULL), len(0)
{
    //printf("[%d] TString()",int(this));
}

TString::TString(const char *str) : content(NULL), len(0)
{
    //printf("[%d] TString(const char*) %s\n", int(this),str);
    init(str);
}

TString::TString(const TString &str) : content(NULL), len(0)
{
    //printf("[%d] TString(const TString &) %s\n", this, str.content);
    init(str.content);
}

TString::TString(TString &&str) : content(NULL), len(0)
{
    //printf("[%d] TString(TString &&) %s\n", this, str.content);
    //������ֵ���ù��죬������д������ˣ�ֱ�Ӱ�str�Ĵ���������Ȼ���ж�str���Ǹ�������ϵ����
    memcpy(this, &str, sizeof(TString));
    str.content = NULL;
}



//��������===============================================
TString::~TString()
{
    //printf("[%d] ~TString %d\n",this,(int)content);
    release();
}


//���ܺ���===============================================

static int gcd(int x, int y)
{
    return y ? gcd(y, x % y) : x;
}

void TString::rotate(const int _k)
{
    if (len <= 1) {
        return;
    }
    int k = _k % len;
    for (int i = gcd(k, len) - 1; i >= 0; i--) {
        int cur = i, nxt;
        char tmp = content[i];
        do {
            nxt = (cur + k) % len;
            swap(content[nxt], tmp);
            cur = nxt;
        } while (cur != i);
    }
}

int TString::find(const char *T, int Tlen)
{
    if (Tlen > len) {
        return -1;
    }

    if (Tlen <= 5) {    //�����Ƚ�С��ֱ�ӱ���
        return find_baoli(T, Tlen);
    }
    else {      //����̫��ʱ����KMP
        int *p = (int *)malloc((Tlen + 3) * sizeof(Tlen));
        if (p == NULL) {
            return find_baoli(T, Tlen);
        }
        else {
            int ret = find_KMP(T, Tlen, p);
            free(p);
            return ret;
        }
    }
}

int TString::length() const
{
    return len;
}

char *TString::c_str() const
{
    return content;
}



/*���������==============================================*/

char &TString::operator[](int index)
{
    return content[index];
}


//��ֵ
TString TString::operator=(const char *str)
{
    //puts("=*");
    init(str);
    //puts("end =*");
    return *this;
}

TString TString::operator=(const TString &str)
{
    //puts("=&");
    init(str.content);
    //puts("end =&");
    return *this;
}

TString TString::operator=(TString &&str)
{
    //puts("=&&");

    release();  //��Ҫ����

    memcpy(this, &str, sizeof(TString));
    str.content = NULL;
    //puts("end =&&");
    return *this;
}


//������
TString &TString::operator+=(const char *str)
{
    //puts("+=*");
    rplus(str);
    return *this;
}

TString &TString::operator+=(const TString &str)
{
    //puts("+=&");
    rplus(str.content);
    return *this;
}

TString &TString::operator+=(const char ch)
{
    //puts("+=");
    char tmp[2] = { ch, '\0' };
    rplus(tmp);
    return *this;
}


//����
TString operator+(const TString &LHS, const TString &RHS)
{
    //puts("&+&");
    TString ret(LHS);
    ret.rplus(RHS.content);
    return ret;
}

TString operator+(const TString &LHS, const char *RHS)
{
    //puts("&+*");
    TString ret(LHS);
    ret.rplus(RHS);
    return ret;
}

TString operator+(const TString &LHS, const char RHS)
{
    //puts("&+");
    TString ret(LHS);
    char tmp[2] = { RHS, '\0' };
    ret.rplus(tmp);
    return ret;
}

TString operator+(const char *LHS, const TString &RHS)
{
    //puts("*+&");
    TString ret(RHS);
    ret.lplus(LHS);
    return ret;

}

TString operator+(const char LHS, const TString &RHS)
{
    //puts("+&");
    TString ret(RHS);
    char tmp[2] = { LHS, '\0' };
    ret.lplus(tmp);
    return ret;
}

TString operator+(TString &&LHS, const TString &RHS)
{
    //puts("&&+&");
    LHS.rplus(RHS.content);
    return LHS;
}

TString operator+(TString &&LHS, const char *RHS)
{
    //puts("&&+*");
    LHS.rplus(RHS);
    return LHS;
}

TString operator+(TString &&LHS, const char RHS)
{
    //puts("&+");
    char tmp[2] = { RHS,'\0' };
    LHS.rplus(tmp);
    return LHS;
}


//ɾ��
TString &TString::operator-=(const char *str)
{
    del(str);
    return *this;
}

TString &TString::operator-=(const TString &str)
{
    del(str.content);
    return *this;
}

TString &TString::operator-=(const char ch)
{
    char tmp[2] = { ch,'\0' };
    del(tmp);
    return *this;
}

TString operator-(const TString &LHS, const TString &RHS)
{
    TString ret(LHS);
    ret.del(RHS.content);
    return ret;
}

TString operator-(const TString &LHS, const char *RHS)
{
    TString ret(LHS);
    ret.del(RHS);
    return ret;
}

TString operator-(const TString &LHS, const char RHS)
{
    TString ret(LHS);
    char tmp[2] = { RHS,'\0' };
    ret.del(tmp);
    return ret;
}

TString operator-(TString &&LHS, const TString &RHS)
{
    LHS.del(RHS.content);
    return LHS;
}

TString operator-(TString &&LHS, const char *RHS)
{
    LHS.del(RHS);
    return LHS;
}

TString operator-(TString &&LHS, const char RHS)
{
    char tmp[2] = { RHS,'\0' };
    LHS.del(tmp);
    return LHS;
}

//���ظ�
TString &TString::operator*=(const int n)
{
    char *p=this->multiply(n);
    if (p!=NULL) {
        free(content);
        content = p;
        len *= n;
    }
    return *this;
}

//�ظ�
TString TString::operator*(const int n)
{
    char *p = this->multiply(n);
    if (p == NULL) {
        return TString(p, 0);
    }
    else {
        return TString(p, len * n);
    }
}

//��ת
TString TString::operator!()
{
    char *p = (char*)malloc((len + 1) * sizeof(char));

    if (p == NULL) {
        return TString();   //����ʧ��
    }

    for (int l = 0, r = len - 1; l <= r; l++, r--) {
        p[l] = content[r];
        p[r] = content[l];
    }
    p[len] = '\0';
    
    return TString(p, len);
}


//�Ƚ�
bool operator>(const TString &LHS, const TString &RHS)
{
    if (LHS.len == 0)
        return 0;
    if (RHS.len == 0)
        return 1;
    return strcmp(LHS.content, RHS.content) > 0;
}
bool operator>(const TString &LHS, const char *RHS)
{
    if (LHS.len == 0)
        return 0;
    if (RHS == NULL || *RHS=='\0')
        return 1;
    return strcmp(LHS.content, RHS) > 0;
}
bool operator>(const char *LHS, const TString &RHS)
{
    if (LHS == NULL || *LHS=='\0')
        return 0;
    if (RHS.len == 0)
        return 1;
    return strcmp(LHS, RHS.content) > 0;
}

bool operator<(const TString &LHS, const TString &RHS)
{
    if (RHS.len == 0)
        return 0;
    if (LHS.len == 0)
        return 1;
    return strcmp(LHS.content, RHS.content) < 0;
}
bool operator<(const TString &LHS, const char *RHS)
{
    if (RHS== NULL || *RHS=='\0')
        return 0;
    if (LHS.len == 0)
        return 1;
    return strcmp(LHS.content, RHS) < 0;
}
bool operator<(const char *LHS, const TString &RHS)
{
    if (RHS.len == 0)
        return 0;
    if (LHS== NULL || *LHS=='\0')
        return 1;
    return strcmp(LHS, RHS.content) < 0;
}

bool operator>=(const TString &LHS, const TString &RHS)
{
    return !(LHS < RHS);
}
bool operator>=(const TString &LHS, const char *RHS)
{
    return !(LHS < RHS);
}
bool operator>=(const char *LHS, const TString &RHS)
{
    return !(LHS < RHS);
}

bool operator<=(const TString &LHS, const TString &RHS)
{
    return !(LHS > RHS);
}
bool operator<=(const TString &LHS, const char *RHS)
{
    return !(LHS > RHS);
}
bool operator<=(const char *LHS, const TString &RHS)
{
    return !(LHS > RHS);
}

bool operator==(const TString &LHS, const TString &RHS)
{
    if (LHS.len == 0)
        return RHS.len == 0;
    if (RHS.len == 0)
        return 0;
    return strcmp(LHS.content, RHS.content) == 0;
}
bool operator==(const TString &LHS, const char *RHS)
{
    if (LHS.len == 0)
        return (RHS== NULL);
    if (RHS == NULL)
        return 0;
    return strcmp(LHS.content, RHS) == 0;
}
bool operator==(const char *LHS, const TString &RHS)
{
    if (LHS == NULL)
        return RHS.len == 0;
    if (RHS.len == 0)
        return 0;
    return strcmp(LHS, RHS.content) == 0;
}

bool operator!=(const TString &LHS, const TString &RHS)
{
    return !(LHS == RHS);
}
bool operator!=(const TString &LHS, const char *RHS)
{
    if (LHS.len == 0) {
        return 0;
    }
    return !(LHS == RHS);
}
bool operator!=(const char *LHS, const TString &RHS)
{
    return !(LHS == RHS);
}



istream &operator>>(istream &in, TString &str)
{
    str.release();  //�Ƚ�str���

    static char tmp[TMPSIZE];
    int p, mp = TMPSIZE - 1;
    char ch;

    for (ch = in.get(); in.good() && isspace(ch); ch = in.get())
        continue;

    tmp[TMPSIZE - 1] = '\0';
    for (p = 0; in.good() && !isspace(ch); ch = in.get()) {
        tmp[p] = ch;
        if (++p == mp) {
            str.rplus(tmp);
            p = 0;
        }
    }

    if (p > 0) {
        tmp[p] = '\0';
        str.rplus(tmp);
    }

    return in;
}


ostream &operator<<(ostream &out, const TString &str)
{
    if (str.len > 0)
        out << str.content;
    else
        out << "<NULL>";
    return out;
}




//˽�к���
//======================================================================

TString::TString(char *str, const int _len)
{
    content = str;
    len = _len;
}

inline void TString::release()
{
    if (content != NULL) {
        //printf("  release %s {%d}\n", content,content);
        free(content);
        content = NULL;
    }
    len = 0;
}

inline void TString::init(const char *str)
{
    if (str == content) {
        return;  //��ԭ����ʼ������������ֱ���˳��ú���
    }

    release();

    //������Чstr�����մ�
    if (str == NULL || *str == '\0') {
        content = NULL;
        return;
    }

    //����ռ䣨����ʧ�ܱ�ɿմ���
    len = strlen(str);
    if ((content = (char *)malloc((len + 1) * sizeof(char))) == NULL) {
        content = NULL;
        len = 0;
        //exitERR("TString(const char *) ������ռ�ʧ��");
        return;
    }
    //printf("  malloc {%d}\n", content);

    memcpy(content, str, (len + 1) * sizeof(char));
}

inline void TString::rplus(const char *str)
{
    //��Ч��str�������в���������ԭ����
    if (str == NULL || *str == '\0') {
        return;
    }

    int dlen = strlen(str);
    char *np = (char *)realloc(content, (len + dlen + 1) * sizeof(char));  //���·���ռ�

    //����ʧ�ܣ������в���������ԭ����
    if (np == NULL) {
        return;
    }

    //printf("  realloc {%d} -> {%d}\n", content, np);

    content = np;
    memcpy(content + len, str, (dlen + 1) * sizeof(char));
    len += dlen;

}

inline void TString::lplus(const char *str)
{
    int old_len = len, dlen;
    rplus(str);
    dlen = len - old_len;
    rotate(dlen);
}

inline int TString::find_baoli(const char *T, const int Tlen)
{
    for (int i = 0, mi = len - Tlen; i <= mi; i++) {
        bool fail = 0;
        for (int j = 0; j < Tlen; j++)
            if (content[i + j] != T[j] && (fail = 1))
                break;
        if (!fail)
            return i;
    }
    return -1;
}

inline int TString::find_KMP(const char *T, const int Tlen, int *nxt)
{
    //calc_nxt
    nxt[0] = -1;
    for (int i = 1; i < Tlen; i++) {
        int k = nxt[i - 1];
        while (k >= 0 && T[k + 1] != T[i])
            k = nxt[k];
        if (T[k + 1] == T[i])
            k++;
        nxt[i] = k;
    }

    //kmp
    int k = 0;
    for (int i = 0; i < len; i++) {
        while (k >= 0 && content[i] != T[k + 1])
            k = nxt[k];
        if (content[i] == T[k + 1])
            k++;
        if (k == Tlen - 1) {
            return i - k;
        }
    }
    return -1;
}

inline void TString::del(const char *str)
{
    //��Ч��
    if (str == NULL || *str == '\0') {
        return;
    }

    int dlen = strlen(str);
    int pos = this->find(str, dlen);

    //û�ҵ�
    if (pos < 0) {
        return;
    }

    //�ҵ��ˣ�����ɾ��
    this->rotate(len - (pos + dlen));
    char *np = (char *)realloc(content, (len - dlen + 1) * sizeof(char));
    if (np == NULL) {
        return;   //��С�������벻���ռ�???
    }
    content = np;
    len -= dlen;
    np[len] = '\0';
    this->rotate(pos);
}

inline char *TString::multiply(const int n)
{
    char *np = (char *)malloc((len * n + 1) * sizeof(char));
    if (np == NULL) {
        return NULL; //����ռ�ʧ�ܣ����ؿմ�
    }

    for (int i = 0, ii = 0; i < n; i++, ii += len) {
        memcpy(np + ii, content, len * sizeof(char));
    }
    np[len * n] = '\0';

    return np;
}

void TString::exitERR(const char *hint)
{
    puts(hint);
    getchar();
    exit(-1);
}




/* �������Ҫ������ȫ�ֺ�����ʵ�֣�����д�ڴ˴� */

int TStringLen(const TString &str)
{
    return str.length();
}