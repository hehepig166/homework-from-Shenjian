/* 朱泽凯 2051995 济美 */
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
    cout << "Usage: " << procname << " 要检查的学号/all 匹配学号/all 源程序名/all 相似度阀值(60-100) 输出(filename/screen)" << endl;
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
        puts("要检查的学号不是7位数字");
        return -1;
    }
    if (stuJ == STU_ERR_NOT_7) {
        puts("要检查的学号不是7位");
        return -1;
    }
    stuP = trans_stu(str_pipei);
    if (stuP == STU_ERR_NOT_DIG) {
        puts("要匹配的学号不是7位数字");
        return -1;
    }
    if (stuP == STU_ERR_NOT_7) {
        puts("要匹配的学号不是7位");
        return -1;
    }
    if (stuJ == STU_ALL && stuP != STU_ALL) {
        puts("检查学号是all，匹配学号必须是all");
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
        puts("源程序文件名超过了32字节");
        return 1;
    }

    numSim = trans_sim(argv[4]);

    if (check_f(argv[5]) != 0) {
        puts("输出结果文件名超过了32字节");
        return 1;
    }

    puts("参数检查通过");
    cout << "检查学号：" << argv[1] << endl;
    cout << "匹配学号：" << argv[2] << endl;
    cout << "源文件名：" << argv[3] << endl;
    cout << "匹配阈值：" << numSim << endl;
    cout << "输出目标：" << argv[5] << endl;
    return 0;
}