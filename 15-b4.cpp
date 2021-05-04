/* 2051995 朱泽凯 济美 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;

#define TOK_L -1
#define TOK_N -2
#define TOK_T -3
#define TOK_NUMBER 0
#define TOK_UNKNOWN -100
#define TOK_ILLEGAL -200

#define L_MIN 64
#define L_MAX 64000
#define L_DEF 128
#define N_MIN 1
#define N_MAX 10000
#define N_DEF 100

struct ping_data {
    int l;
    int n;
    bool t;
    unsigned int IP[4];

    ping_data()
    {
        l = L_DEF;
        n = N_DEF;
        t = 0;
        IP[0] = IP[1] = IP[2] = IP[3] = 0;
    }

};

//正常则返回0
int get_ip(ping_data *p, const char *str)
{
    const char *cp = str;
    int cnt = 0, lst = -1;
    unsigned int a[4] = { 0 };

    for (; *cp != '\0'; cp++) {
        if (isdigit(*cp)) {
            a[cnt] = (a[cnt] << 1) + (a[cnt] << 3) + *cp - '0';
        }
        else if (*cp == '.' && (cp - str) != lst + 1) {
            lst = cp - str;
            cnt++;
            continue;
        }
        else {
            cout << "IP地址错误" << endl;
            return -1;
        }
    }

    if (cnt < 3) {
        cout << "IP地址错误" << endl;
        return -1;
    }

    for (int i = 0; i < 4; i++) {
        if (a[i] > 255) {
            cout << "IP地址错误" << endl;
            return -1;
        }
    }

    for (int i = 0; i < 4; i++) {
        p->IP[i] = a[i];
    }

    return 0;
}

int get_tok(const char *str)
{
    if (str[0] != '-')
        return TOK_ILLEGAL;

    if (strcmp(str, "-l") == 0)
        return TOK_L;
    if (strcmp(str, "-n") == 0)
        return TOK_N;
    if (strcmp(str, "-t") == 0)
        return TOK_T;

    return TOK_UNKNOWN;
}

//不是正整数则返回-1
int get_posi_int(const char *str)
{
    int ret = 0;
    while (*str != '\0') {
        if (isdigit(*str))
            ret = (ret << 1) + (ret << 3) + *str - '0';
        else
            return -1;
        str++;
    }
    return ret;
}

int pd_modify(ping_data *p, const char **argv, int &pt, const int ptmax)
{
    int opr = get_tok(argv[pt]);
    int tmp;

    if (opr == TOK_ILLEGAL) {
        cout << "参数" << argv[pt] << "不是以-开头的合法参数" << endl;
        return -1;
    }
    if (opr == TOK_UNKNOWN) {
        cout << "参数" << argv[pt] << "不存在";
        return -1;
    }

    if (opr == TOK_L) {
        if (pt==ptmax || argv[pt + 1][0] == '-') {   //参数不足或下一个又是一个参数命令
            cout << "参数" << argv[pt] << "没有后续参数" << endl;
            return -1;
        }
        pt++;
        tmp = get_posi_int(argv[pt]);
        p->l = (tmp >= L_MIN && tmp <= L_MAX) ? tmp : L_DEF;
    }

    if (opr == TOK_N) {
        if (pt == ptmax || argv[pt+1][0] == '-') {   //参数不足或下一个又是一个参数命令
            cout << "参数" << argv[pt] << "没有后续参数" << endl;
            return -1;
        }
        pt++;
        tmp = get_posi_int(argv[pt]);
        p->n = (tmp >= N_MIN && tmp <= N_MAX) ? tmp : N_DEF;
    }

    if (opr == TOK_T) {
        p->t = 1;
    }

    return 0;
}

int main(int argc, const char **argv)
{
    if (argc == 1) {
        cout << "Usage: _std [-l 大小] [-n 数量] [-t] IP地址" << endl;
        return 0;
    }

    ping_data pd;

    if (get_ip(&pd, argv[argc - 1]) != 0) {
        return -1;
    }

    for (int i = 1, mi = argc - 2; i <= mi; i++) {
        if (pd_modify(&pd, argv, i, mi) != 0) {
            return 1;
        }
    }

    cout << "参数检查通过" << endl;
    cout << "-n参数：" << pd.n << endl;
    cout << "-l参数：" << pd.l << endl;
    cout << "-t参数：" << pd.t << endl;
    cout << "IP地址：" << pd.IP[0] << '.' << pd.IP[1] << '.' << pd.IP[2] << '.' << pd.IP[3] << endl;

    return 0;
}