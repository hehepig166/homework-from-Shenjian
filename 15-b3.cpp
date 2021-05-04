/* ���� 2051995 ���� */
#include <iostream>
#include <cstring>
using namespace std;

#define STU_ERR_NOT_DIG -1
#define STU_ERR_NOT_7 -2
#define STU_ALL -3

#define NUMSIM_MIN 60
#define NUMSIM_MAX 100
#define NUMSIM_DEFAULT 80

int usage(const char *procname)
{
    cout << "Usage: " << procname << " Ҫ����ѧ��/all ƥ��ѧ��/all Դ������/all ���ƶȷ�ֵ(60-100) ���(filename/screen)" << endl;
    cout << "               2059999          2059998      all          80                screen" << endl;
    cout << "               2059999          all          7-b2.cpp     75                result.txt" << endl;
    cout << "               all              all          7-b2.cpp     80                check.dat" << endl;
    cout << "               all              all          all          85                screen" << endl;

    return 0;
}

int trans_stu(const char *str)
{
    if (strcmp(str, "all") == 0)
        return STU_ALL;

    if (strlen(str) != 7) {
        return STU_ERR_NOT_7;
    }

    int ret = 0;
    while (*str != '\0') {
        if (!isdigit(*str))
            return STU_ERR_NOT_DIG;
        ret = (ret << 1) + (ret << 3) + *str - '0';
        str++;
    }
    return ret;
}

int check_stu(const char *str_jiancha, const char *str_pipei, int &stuJ, int &stuP)
{
    stuJ = trans_stu(str_jiancha);
    if (stuJ == STU_ERR_NOT_DIG) {
        puts("Ҫ����ѧ�Ų���7λ����");
        return -1;
    }
    if (stuJ == STU_ERR_NOT_7) {
        puts("Ҫ����ѧ�Ų���7λ");
        return -1;
    }
    stuP = trans_stu(str_pipei);
    if (stuP == STU_ERR_NOT_DIG) {
        puts("Ҫƥ���ѧ�Ų���7λ����");
        return -1;
    }
    if (stuP == STU_ERR_NOT_7) {
        puts("Ҫƥ���ѧ�Ų���7λ");
        return -1;
    }
    if (stuJ == STU_ALL && stuP != STU_ALL) {
        puts("���ѧ����all��ƥ��ѧ�ű�����all");
        return -1;
    }
    return 0;
}

int check_f(const char *str)
{
    if (strlen(str) > 32) {
        return -1;
    }
    return 0;
}

int trans_sim(const char *str)
{
    int ret;
    for (ret = 0; *str != '\0'; str++) {
        if (*str < '0' || *str>'9')
            return NUMSIM_DEFAULT;
        ret = (ret << 1) + (ret << 3) + *str - '0';
    }

    if (ret<NUMSIM_MIN || ret>NUMSIM_MAX)
        return NUMSIM_DEFAULT;

    return ret;
}

int main(int argc, char **argv)
{
    int stuJ, stuP;
    //char strf[35];
    int numSim;


    if (argc != 6) {
        usage(argv[0]);
        return 1;
    }

    if (check_stu(argv[1], argv[2], stuJ, stuP) != 0) {
        return 1;
    }

    if (check_f(argv[3]) != 0) {
        puts("Դ�����ļ���������32�ֽ�");
        return 1;
    }

    numSim = trans_sim(argv[4]);

    if (check_f(argv[5]) != 0) {
        puts("�������ļ���������32�ֽ�");
        return 1;
    }

    puts("�������ͨ��");
    cout << "���ѧ�ţ�" << argv[1] << endl;
    cout << "ƥ��ѧ�ţ�" << argv[2] << endl;
    cout << "Դ�ļ�����" << argv[3] << endl;
    cout << "ƥ����ֵ��" << numSim << endl;
    cout << "���Ŀ�꣺" << argv[5] << endl;
    return 0;
}