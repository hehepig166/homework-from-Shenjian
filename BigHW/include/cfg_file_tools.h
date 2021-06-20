/* 朱泽凯 2051995 济美 */

#ifndef __HEHEPIG_CFG_FILE_TOOLS__
#define __HEHEPIG_CFG_FILE_TOOLS__

#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum class ITEM_TYPE {
    Int,		//为避免和int冲突，采用首字母大写形式
    Double,
    String,
    Character,
    Null
};

#define OPEN_OPT_RDONLY		0	//以只读方式打开（打不开则返回失败）
#define OPEN_OPT_RDWR		1	//以读写方式打开（打不开文件则创建）

#define FAIL -1

bool is_a_group_name(const string str);


#define DEBUG_CFG_HEHEPIG 0

/* ---------------------------------------------------------------
     允许加入其它需要的定义
     const全局变量/static全局变量(慎用)/define/class/struct/enum/enum class/union等
   ---------------------------------------------------------------- */


bool group_name_analyze(const string &strline, string &name);

bool item_analyze(const string &strline, string &name, string &value);


/* ---------------------------------------------------------------
     class CFT的定义，已有部分不要动，允许protected加入其它需要的定义
   ---------------------------------------------------------------- */
class CFT {
protected:
    string filename;
    fstream fp;

    int file_resize(int newsize);

    /* ---------------------------------------------------------------
         允许加入其它需要的定义（内部用）
       ---------------------------------------------------------------- */
    bool is_open_flag;
    int open_opt;

    bool reopen_opt(int opt); //成功返回 true

    //删除算上 pos_start 开始往后的 size 字节的东西
    //若超了，则删到不能删为止
    void del(int pos_start, int size);      //删除操作全用这个，为了保证调用了 to_win

    //从 pos_start （补算）往后插入 str
    //不检查 pos_start 的合理性
    void add(int pos_start, string str);    //插入操作全用这个（包括在结尾），为了保证调用了 to_win

    //从 start_pos 开始查找该组，根据 mode 返回读完对应的 tell() 值（默认 mode = 0）
    //0: [group_name] 这一行之前（seek之后读到第一行就是 [] 那一行）
    //1: [group_name] 这一行之后（读完 [] 那一行的seek）
    //2: [group_name] 这一组最后（seek之后读到第一行就是下一个组的 []）（若已经是最后一组，则返回最后）
    //若没找到则返回 FAIL (-1)
    int group_find(const char *group_name, int mode = 0, int start_pos = 0);

    //从 start_pos 开始查找该组的该项，根据 mode 返回读完对应的 tell() 值（默认 mode = 0）
    //0: [group_name] 这一行之前（seek之后读到第一行就是 [] 那一行）
    //1: [group_name] 这一行之后（读完 [] 那一行的seek）
    //若没找到则返回 FAIL (-1)
    int item_find(const char *group_name, const char *item_name, int mode = 0, int start_pos = 0);

    //转成 win 格式（0A -> 0D 0A）
    //成功变成 win 格式则返回 1，否则返回 0
    //不考虑效率问题了……每次有修改的操作都进行一次to_win
    int to_win(int start_pos = 0);

public:
    CFT();							//无参构造，置filename为""即可
    CFT(const char *cfgname, int opt = OPEN_OPT_RDONLY);	//两参构造，功能同open
    ~CFT();							//析构

    void open(const char *cfgname, int opt = OPEN_OPT_RDONLY);
    bool is_open();	//如果打开成功，则返回true，否则返回false
    void close();

    /* 组增加与删除 */
    int group_add(const char *group_name);
    int group_del(const char *group_name);

    /* 项增加，多类型重载 */
    int item_add(const char *group_name, const char *item_name, const int item_value);
    int item_add(const char *group_name, const char *item_name, const double item_value);
    int item_add(const char *group_name, const char *item_name, const char *item_value);
    int item_add(const char *group_name, const char *item_name, const string &item_value);
    int item_add(const char *group_name, const char *item_name, const char item_value);
    int item_add(const char *group_name, const char *item_name);	//NULL

    /* 项删除 */
    int item_del(const char *group_name, const char *item_name);

    /* 项更新，多类型重载 */
    int item_update(const char *group_name, const char *item_name, const int item_value);
    int item_update(const char *group_name, const char *item_name, const double item_value);
    int item_update(const char *group_name, const char *item_name, const char *item_value);
    int item_update(const char *group_name, const char *item_name, const string &item_value);
    int item_update(const char *group_name, const char *item_name, const char item_value);
    int item_update(const char *group_name, const char *item_name);	//NULL

    /* 取某项的值，多类型重载 */
    int item_get_value(const char *group_name, const char *item_name, int &item_value);
    int item_get_value(const char *group_name, const char *item_name, double &item_value);
    int item_get_value(const char *group_name, const char *item_name, char *item_value);
    int item_get_value(const char *group_name, const char *item_name, string &item_value);
    int item_get_value(const char *group_name, const char *item_name, char &item_value);
    int item_get_value(const char *group_name, const char *item_name);

    /* ---------------------------------------------------------------
         不允许加入其它需要的定义（即对外呈现的只有上述各项）
       ---------------------------------------------------------------- */
};

#endif