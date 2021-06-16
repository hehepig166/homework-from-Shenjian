/* 2051995 朱泽凯 济美 */

//c++ -Wall -lncurses -o 17-b5 17-b5-1.cpp

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;


//别让屏幕输入输出的时候读到奇怪的东西
ostream &operator<<(ostream &out, const unsigned char &x)
{
    out << int(x);
    return out;
}
istream &operator>>(istream &in, unsigned char &x)
{
    int tmp;
    in >> tmp;
    x = (unsigned char)tmp;
    return in;
}


class TYPE_NAME {
public:
    char data[16] = { 0 };
    //要是读到长度大于15的，这样处理： data[0]=0，然后昵称那里长度最小是1，就能不通过了
    TYPE_NAME() {}
    TYPE_NAME(int len)
    {
        data[len] = 0;
        while (len--)
            data[len] = '#';
    }
    friend istream &operator>>(istream &in, TYPE_NAME &X)
    {
        string tmp;
        in >> tmp;
        if (tmp.size() > 15)
            X.data[0] = 0;
        else
            strcpy(X.data, tmp.c_str());
        return in;
    }
    friend ostream &operator<<(ostream &out, TYPE_NAME &X)
    {
        out << X.data;
        return out;
    }
    operator int() const
    {
        return strlen(data);
    }
};

//start_addr 从 0 开始计数
class ITEM_ALL {
public:
    int start_addr;
    string name;

    ITEM_ALL(int start_addr_, const string &name_) :start_addr(start_addr_), name(name_) {}
    virtual void print(const void *from) const = 0;
    virtual void modify(const void *from, const void *to) const = 0;
    virtual void ask_modify(void *target) const = 0;
};


template<typename T>
class ITEM :public ITEM_ALL {
public:
    T Min, Max;

    ITEM(int start_addr_, const string &name_, T Min_, T Max_) : ITEM_ALL(start_addr_, name_), Min(Min_), Max(Max_) {}

    virtual void print(const void *from) const
    {
        cout << *(T *)from;
    }
    virtual void modify(const void *from, const void *to) const
    {
        *(T *)to = *(const T *)from;
    }
    virtual void ask_modify(void *target) const
    {
        T tmp;
        cout << name << "，当前值= ";
        print(target);
        cout << "范围[" << Min << ".." << Max << "]，请输入新值" << endl;
        putchar('>');
        putchar(' ');
        cin >> tmp;

        while (!cin.good() || tmp<Min || tmp>Max) {
            cin.clear();
            cin.ignore(1926, '\n');
            cout << "非法的" << name << "，好好看看范围，再次输入" << endl;
            putchar('>');
            putchar(' ');
            cin >> tmp;
        }

        modify(&tmp, target);   //读取成功，更新数据
    }
};



#if 1
static const ITEM<TYPE_NAME> _tn[] = { {0,"玩家昵称",1,15} };
static const ITEM<unsigned char> _uc[] = {
    {44,"移动速度",0,100}, {45,"攻击速度",0,100}, {46,"攻击范围",0,100}, {52,"敏捷度",0,100}, {53,"智力",0,100},
    {54,"经验",0,100}, {55,"等级",0,100}, {58,"消耗魔法值",0,100}, {59,"魔法伤害力",0,100}, {60,"命中率",0,100},
    {61,"魔法防御力",0,100}, {62,"暴击率",0,100}, {63,"耐力",0,100} };
static const ITEM<unsigned short> _us[] = {
    {16,"生命值",0,10000}, {18,"力量值",0,10000}, {20,"体质",0,8192}, {22,"灵巧",0,1024}, {48,"攻击力",0,2000},
    {50,"防御力",0,2000}, {56,"魔法值",0,10000} };
static const ITEM<unsigned int> _uint[] = { {24,"金钱数量",0ll,100000000}, {28,"名声值",0,1000000} , {32,"魅力值",0,1000000} };
static const ITEM<unsigned long long> _ull[] = { {36,"游戏累计时间",0,10000000000000000ll} };
static const ITEM_ALL *item_p[] = {
    &_tn[0],  &_us[0],  &_us[1], &_us[2], &_us[3],
    &_uint[0],&_uint[1], &_uint[2], &_ull[0], &_uc[0],
    &_uc[1], &_uc[2], &_us[4], &_us[5], &_uc[3],
    &_uc[4], &_uc[5], &_uc[6], &_us[6], &_uc[7],
    &_uc[8], &_uc[9], &_uc[10], &_uc[11], &_uc[12]
};
static const int item_cnt = sizeof(item_p) / sizeof(void *);
#endif


//此处为示例，允许修改结构体名称，允许修改结构体中的成员内容，要求sizeof必须是64
struct demo {
    char data[64];
};


#if 1

void demo_show(const demo &D)
{
    for (int i = 0; i < item_cnt; i++) {
        cout << setw(20) << item_p[i]->name << "：";
        item_p[i]->print(D.data + item_p[i]->start_addr);
        cout << endl;
    }
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：整个函数，只允许出现一次open、一次read（因为包含错误处理，允许多次close）
***************************************************************************/
int read()
{
    /* 本函数中只允许定义一个 ifstream流对象，不再允许定义任何形式的fstream/ifstream/ofstream流对象，也不允许使用C方式的文件处理 */
    ifstream gfile;

    /* 文件打开，具体要求为：
       1、要求以读方式打开，打开方式***自行指定
       2、除本次open外，本函数其它地方不允许再出现open  */
    gfile.open("game.dat", ios::binary | ios::in);

    /* 进行后续操作，包括错误处理、读文件、显示各游戏项的值、关闭文件等，允许调用函数
       其中：只允许用一次性读取64字节的方法将game.dat的内容读入***（缓冲区名称、结构体名称自行指定）
                 gfile.read(***, sizeof(demo));
    */
    if (!gfile.is_open()) {
        puts("打开文件 game.dat 失败");
        return -1;
    }
    gfile.seekg(0, ios::end);
    if (gfile.tellg() != 64) {
        cout << "data.dat 大小异常，应该是64byte，实际是" << gfile.tellg() << "byte" << endl;
        return -1;
    }
    gfile.seekg(0, ios::beg);


    demo D;
    gfile.read((char *)&D, sizeof(demo));

    demo_show(D);

    gfile.close();
    return 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：整个函数，只允许出现一次open、一次read、一次write（因为包含错误处理，允许多次close）
***************************************************************************/
int modify()
{
    /* 本函数中只允许定义一个 fstream流对象，不再允许定义任何形式的fstream/ifstream/ofstream流对象，也不允许使用C方式的文件处理 */
    fstream gfile;

    /* 文件打开，具体要求为：
       1、要求以读写方式打开，打开方式***自行指定
       2、除本次open外，本函数其它地方不允许再出现open  */
    gfile.open("game.dat", ios::binary | ios::in | ios::out);

    /* 进行后续操作，包括错误处理、读文件、显示各游戏项的值、关闭文件等，允许调用函数
       其中：只允许用一次性读取64字节的方法将game.dat的内容读入***（缓冲区名称、结构体名称自行指定）
                 gfile.read(***, sizeof(demo));
             只允许用一次性写入64字节的方法将***的内容写入game.dat中（缓冲区名称、结构体名称自行指定）
                 gfile.write(***, sizeof(demo));
    */
    if (!gfile.is_open()) {
        puts("打开文件 game.dat 失败");
        return -1;
    }
    gfile.seekg(0, ios::end);
    if (gfile.tellg() != 64) {
        cout << "data.dat 大小异常，应该是64byte，实际是" << gfile.tellg() << "byte" << endl;
        return -1;
    }
    gfile.seekg(0, ios::beg);

    demo D;
    gfile.read(D.data, sizeof(demo));


    //------------------------------
    //存档修改模块
    //------------------------------
    int opr;
    while (1) {
        puts("");
        puts("--------------------------------------");
        puts("  游戏存档文件修改工具");
        puts("--------------------------------------");
        for (int i = 0; i < item_cnt; i++) {
            cout << "  " << (char)(i + 'a') << '.' << left << setw(16) << item_p[i]->name << right;
            item_p[i]->print(D.data + item_p[i]->start_addr);
            cout << endl;
        }
        puts("--------------------------------------");
        puts("  0.放弃修改");
        puts("  1.存盘退出");
        puts("--------------------------------------");
        puts("请选择[a..y, 0..1]");
        putchar('>');
        putchar(' ');
        do {
            opr = cin.get();
        } while (opr <= ' ' || opr > '~');
        cin.ignore(1926, '\n');
        puts("");

        if (opr == '0') {
            break;
        }
        else if (opr == '1') {
            gfile.seekp(0, ios::beg);
            gfile.write(D.data, sizeof(demo));
            break;
        }
        else if (isalpha(opr) && ((opr = (tolower(opr) - 'a')) < item_cnt)) {
            item_p[opr]->ask_modify(D.data + item_p[opr]->start_addr);
        }
    }
    //------------------------------

    gfile.close();
    return 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：main函数允许带参数，不允许进行文件读写
***************************************************************************/
int main(int argc, const char *argv[])
{

    if (argc == 2 && strcmp(argv[1], "--read") == 0)
        read();
    else if (argc == 2 && strcmp(argv[1], "--modify") == 0)
        modify();
    else
        puts("usage : 17-b5-demo --modify | --read");

    return 0;
}


#endif