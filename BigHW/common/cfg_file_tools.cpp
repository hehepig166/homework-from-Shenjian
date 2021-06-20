/* 朱泽凯 2051995 济美 */
#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <io.h>
#include "../include/cfg_file_tools.h"
using namespace std;

/* ---------------------------------------------------------------
     允许加入其它需要static函数（内部工具用）
   ---------------------------------------------------------------- */


   /***************************************************************************
     函数名称：
     功    能：
     输入参数：
     返 回 值：
     说    明：如有必要，按需加入其它内容
   ***************************************************************************/
CFT::CFT()
{
    filename = "";
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：如有必要，按需加入其它内容
***************************************************************************/
CFT::~CFT()
{
    this->close();
    is_open_flag = false;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
CFT::CFT(const char *cfgname, int opt)
{
    open(cfgname, opt);
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void CFT::open(const char *cfgname, int opt)
{
    this->close();  //先把前面打开过的关掉

    is_open_flag = false;

    filename = (cfgname == NULL) ? "" : cfgname;

    if (!reopen_opt(opt)) {
        cout << "打开文件 [" << filename << "] 失败" << endl;
        return;
    }

    is_open_flag = true;

    to_win();   //先把原来的配置文件保证为 Windows 格式

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：打开成功返回true，否则返回false
***************************************************************************/
bool CFT::is_open(void)
{
    return is_open_flag && fp.is_open();
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void CFT::close()
{
    if (fp.is_open())
        fp.close();
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：C++17中有filesystem::resize_file直接可以修改文件大小，之前的版本无
                   因此本函数扔沿用C的旧方法
        已实现，不要动
***************************************************************************/
int CFT::file_resize(int newsize)
{
    /* 如果大小是负数则直接返回-1 */
    if (newsize < 0)
        return -1;

    if (!fp.good())
        fp.clear();

    /* 取fp当前指针位置 */
    int now_pos = int(fp.tellg());

    /* 首先：C++方式关闭 */
    this->close();

    /* 其次：C方式打开并改变文件大小 */
    FILE *fp_cstyle;
    fp_cstyle = fopen(filename.c_str(), "rb+"); //rw方式打开
    if (fp_cstyle) {
        _chsize(_fileno(fp_cstyle), newsize);
        fclose(fp_cstyle);
    }
    else
        cout << "open [" << filename << "] for resize failed" << endl;

    /* 再次：C++方式重新打开 */
    fp.open(filename, ios::in | ios::out | ios::binary);
    if (fp.is_open() == 0) {
        cout << "reopen [" << filename << "] after resize failed" << endl;
        return -1;
    }

    /* 文件指针移动到原位置 */
    fp.seekg(now_pos < newsize ? now_pos : newsize, ios::beg);

    return 0;
}

/***************************************************************************
    函数名称：
    功    能：
    输入参数：
    返 回 值：成功返回1，否则返回0
    说    明：若组已存在，不重复增加，直接返回0
***************************************************************************/
int CFT::group_add(const char *group_name)
{
    reopen_opt(open_opt);

    if (!is_open() || open_opt != OPEN_OPT_RDWR) {  //状态不对的对象不进行操作
        return 0;
    }

    int pos = this->group_find(group_name, 0, 0);  //在配置文件中找是否已经有该组

    if (pos == FAIL) {  //若没有该组，则在文件最后添加一个 "\n[group_name]\n"
        if (fp.seekp(0, ios::end).tellp() == 0) {
            add(0, string("[").append(group_name).append("]\n"));
        }
        else {
            add((int)fp.tellp(), string("\n[").append(group_name).append("]\n"));
        }
    }
    else {
        return 0;
    }

    return 1;   //跑到这里说明成功了
}

/***************************************************************************
  函数名称：
  功    能：删除组 group_name 以及其下内容直到没有名字为此的组
  输入参数：
  返 回 值：删除的组数（有可能有许多组同名）
  说    明：
***************************************************************************/
int CFT::group_del(const char *group_name)
{
    reopen_opt(open_opt);

    if (!is_open() || open_opt != OPEN_OPT_RDWR) {
        return 0;
    }

    int ret(0);
    int pos_beg, pos_end;

    while ((pos_beg = group_find(group_name, 0, 0)) != FAIL) {
        pos_end = group_find(group_name, 2, pos_beg);   //一般来说在这里不会出错了

        del(pos_beg, pos_end - pos_beg);    //删除该组（把文件后面直到结尾的东西往前挪）

        ret++;
    }

    return ret;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_add(const char *group_name, const char *item_name, const int item_value)
{
    ostringstream os;
    os << item_value;

    return item_add(group_name, item_name, os.str());
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_add(const char *group_name, const char *item_name, const double item_value)
{
    ostringstream os;
    os << item_value;

    return item_add(group_name, item_name, os.str());
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_add(const char *group_name, const char *item_name, const char *item_value)
{
    ostringstream os;
    os << item_value;

    return item_add(group_name, item_name, os.str());
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：成功返回1，否则返回0
  说    明：组存在，若项不存在，则在最前面的组的最前面位置加，项存在、组不存在都直接返回0
***************************************************************************/
int CFT::item_add(const char *group_name, const char *item_name, const string &item_value)
{
    reopen_opt(open_opt);
    if (!is_open() || open_opt != OPEN_OPT_RDWR || item_name == NULL) {
        return 0;
    }

    int pos_first_group_begin = group_find(group_name, 1, 0);

    if (pos_first_group_begin == FAIL) {    //组不存在
        return 0;
    }

    int pos_item_begin = item_find(group_name, item_name, 0);
    if (pos_item_begin != FAIL) {   //组存在，项存在
        return 0;
    }

    //到这说明组存在，项不存在，需要在第一个匹配的组的第一行添加
    add(pos_first_group_begin, string(item_name).append(" = ").append(item_value).append("\n"));

    return 1;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_add(const char *group_name, const char *item_name, const char item_value)
{
    ostringstream os;
    os << item_value;

    return item_add(group_name, item_name, os.str());
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_add(const char *group_name, const char *item_name)
{
    return item_add(group_name, item_name, "");
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：返回成功删除的项数（因为可能有多个同组同名项）
  说    明：
***************************************************************************/
int CFT::item_del(const char *group_name, const char *item_name)
{
    if (!is_open() || open_opt != OPEN_OPT_RDWR || item_name == NULL) {
        return 0;
    }

    int ret = 0, tmp;
    int pos = item_find(group_name, item_name, 0, 0);

    while (pos != FAIL) {
        tmp = item_find(group_name, item_name, 1, 0) - pos;
        del(pos, tmp);
        ret++;
        pos = item_find(group_name, item_name, 0, 0);
    }

    return ret;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：成功则返回1，否则返回0
  说    明：若不存在该项，则在可以放的最前面创建一个这个项，若存在，则修改最前面那个，且返回1
***************************************************************************/
int CFT::item_update(const char *group_name, const char *item_name, const int item_value)
{
    ostringstream os;
    os << item_value;
    return item_update(group_name, item_name, os.str());
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_update(const char *group_name, const char *item_name, const double item_value)
{
    ostringstream os;
    os << item_value;
    return item_update(group_name, item_name, os.str());
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_update(const char *group_name, const char *item_name, const char *item_value)
{
    ostringstream os;
    os << item_value;
    return item_update(group_name, item_name, os.str());
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：成功则返回1，否则返回0
  说    明：若不存在该项，则在可以放的最前面创建一个这个项，若存在，则修改最前面那个，且返回1
***************************************************************************/
int CFT::item_update(const char *group_name, const char *item_name, const string &item_value)
{
    if (!is_open() || open_opt != OPEN_OPT_RDWR || item_name == NULL) {
        return 0;
    }

    int pos = item_find(group_name, item_name);
    if (pos == FAIL) {  //不存在
        return item_add(group_name, item_name, item_value);
    }
    else {  //存在，修改第一个
        item_del(group_name, item_name);
        add(pos, string(item_name).append(" = ").append(item_value).append("\n"));
        return 1;
    }
    return 0; //此句根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_update(const char *group_name, const char *item_name, const char item_value)
{
    ostringstream os;
    os << item_value;
    return item_update(group_name, item_name, os.str());
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_update(const char *group_name, const char *item_name)
{
    return item_update(group_name, item_name, "");
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：取值成功返回1，否则返回0
  说    明：
***************************************************************************/
int CFT::item_get_value(const char *group_name, const char *item_name, int &item_value)
{
    string tmpstr;
    int f = item_get_value(group_name, item_name, tmpstr);
    istringstream is(tmpstr);
    return (f && !(is >> item_value).fail());
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_get_value(const char *group_name, const char *item_name, double &item_value)
{
    string tmpstr;
    int f = item_get_value(group_name, item_name, tmpstr);
    istringstream is(tmpstr);
    return (f && !(is >> item_value).fail());
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_get_value(const char *group_name, const char *item_name, char *item_value)
{
    string tmpstr;
    int f = item_get_value(group_name, item_name, tmpstr);
    istringstream is(tmpstr);
    strcpy(item_value, tmpstr.c_str());
    return 1;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_get_value(const char *group_name, const char *item_name, string &item_value)
{
    if (!is_open()) {
        return 0;
    }

    int tmp = item_find(group_name, item_name, 0);

    if (tmp == FAIL) {
        return 0;
    }

    string strline, tmpstr_n, tmpstr_v;
    if (!getline(fp.seekg(tmp), strline, '\n').good()) {
        fp.clear();     //重要
        return 0;
    }

    return item_analyze(strline, tmpstr_n, item_value);
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_get_value(const char *group_name, const char *item_name, char &item_value)
{
    string tmpstr;
    int f = item_get_value(group_name, item_name, tmpstr);
    istringstream is(tmpstr);
    return (f && !(is >> item_value).fail());
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CFT::item_get_value(const char *group_name, const char *item_name)
{
    string tmpstr;
    int f = item_get_value(group_name, item_name, tmpstr);
    return f;
}











/* 以下是工具函数 */

bool is_a_group_name(const string strline)
{
    int L, R;
    if ((L = strline.find_first_not_of("\n\r \t")) == strline.npos)
        return false;
    if ((R = strline.find_last_not_of("\n\r \t")) == strline.npos)
        return false;

    return strline[L] == '[' && strline[R] == ']';
}


bool group_name_analyze(const string &strline, string &name)
{
    if (!is_a_group_name(strline))
        return false;

    int p_beg(strline.find_first_not_of("\n\r \t") + 1), p_end(strline.find_last_not_of("\n\r \t") - 1);

    while (strline[p_beg] == ' ')
        p_beg++;
    while (strline[p_end] == ' ')
        p_end--;

    if (p_end < p_beg) {
        name = "";
    }
    else {
        name = "";
        while (p_beg <= p_end) {
            name += strline[p_beg];
            p_beg++;
        }
    }

    return true;
}




bool item_analyze(const string &strline, string &name, string &value)
{
    int pos_div = strline.find('='), tmp, sz = strline.size();

    if (pos_div == strline.npos)
        return false;

    name = "";
    for (tmp = pos_div - 1; pos_div >= 0 && (strline[tmp] == ' ' || strline[tmp] == '\t' || strline[tmp] == '\r'); )
        tmp--;
    for (int i = 0; i <= tmp; i++)
        name += strline[i];

    value = "";
    for (tmp = pos_div + 1; tmp < sz && (strline[tmp] == ' ' || strline[tmp] == '\t' || strline[tmp] == '\r'); )
        tmp++;
    while (tmp < sz && strline[tmp] != '#' && !(strline[tmp] == ' ' || strline[tmp] == '\t' || strline[tmp] == '\r'))     // # 还有后面的都是注释
        value += strline[tmp++];

    return true;
}




/* 以下是 class CFT 中 protected 的函数 */


bool CFT::reopen_opt(int opt)
{
    if (!fp.is_open()) {
        open_opt = opt;
        switch (opt) {
            case OPEN_OPT_RDONLY:
                fp.open(filename.c_str(), ios::in | ios::binary);
                break;
            case OPEN_OPT_RDWR:
                fp.open(filename.c_str(), ios::in | ios::out | ios::binary);
                if (!fp.is_open())
                    fp.open(filename.c_str(), ios::out | ios::binary);
                fp.close();
                fp.open(filename.c_str(), ios::in | ios::out | ios::binary);
                break;
            default:
                cout << "未知的打开方式 [" << opt << "]" << endl;
                return false;
        }
    }
    else {
        int size = int(fp.seekg(0, ios::end).tellg());
        file_resize(size);
    }

    return fp.is_open();
}



void CFT::del(int pos_start, int size)
{
    if (open_opt != OPEN_OPT_RDWR)
        return;
    reopen_opt(OPEN_OPT_RDWR);
    if (!fp.is_open())
        return;


    int sz_old;

    fp.seekp(0, ios::end);
    sz_old = int(fp.tellg());    //获取文件大小
    fp.seekp(0, ios::beg);

    char buf;
    for (int i = pos_start, j = pos_start + size; ; i++, j++) {
        fp.seekp(j, ios::beg);
        fp.read(&buf, 1);
        if (buf == 0x1A || !fp.good())      //注意一下 0x1A
            break;
#if DEBUG_CFG_HEHEPIG
        cout << hex << int(buf) << endl;
#endif
        fp.seekp(i, ios::beg);
        fp.write(&buf, 1);
    }

    file_resize(sz_old - size);   //修改大小，把后面无意义的东西去掉
    reopen_opt(open_opt);

    to_win();   //重要!
}



void CFT::add(int pos_start, string str)
{
    if (!is_open() || open_opt != OPEN_OPT_RDWR)
        return;
    if (str.empty())
        return;

    int n = str.size();

    int size = int(fp.seekg(0, ios::end).tellg());

    if (pos_start == size) {
        fp.seekp(0, ios::end);
        fp.write(str.c_str(), n);
        to_win();   //重要
        return;
    }
    else if (pos_start < 0 || pos_start > size) {
        return;
    }

    file_resize(size + n);

    int cnt = size + n - pos_start;
    char buf;

    for (int i = 0, k = 0; cnt--; k = (++i) % n) {
        buf = str[k % n];
        fp.seekg(pos_start + i, ios::beg);
        fp.read(&str[k], 1);

        fp.seekp(pos_start + i, ios::beg);
        fp.write(&buf, 1);

        reopen_opt(OPEN_OPT_RDWR);
    }

    to_win();   //重要
}



//查找该组，返回读完该行之后得到的 tell() 值
int CFT::group_find(const char *group_name, int mode, int start_pos)
{
    if (!is_open() || group_name == NULL) {
        return FAIL;
    }

    fp.seekg(start_pos, ios::beg);
    if (!fp.good()) {
        fp.clear();
        return FAIL;
    }

    unsigned long long lst_beg(start_pos), lst_end;
    string strline, name;
    bool flag(false);

    while (getline(fp, strline, '\n').good()) {

        lst_end = fp.tellg();
        if (group_name_analyze(strline, name)) {
            if (name == group_name) {
                switch (mode) {
                    case 0:
                        return (int)lst_beg;
                        break;
                    case 1:
                        return (int)lst_end;
                        break;
                    case 2:
                        flag = true;
                        break;
                }
            }
            else if (flag) {
                return (int)lst_beg;
            }
        }
        lst_beg = fp.tellg();
    }

    fp.clear();     //重要!!!!!!!!

    if (flag) { //说明是匹配到最后一组了
        fp.seekg(0, ios::end);
        int ret = (int)fp.tellg();
        fp.seekg(0, ios::beg);
        return ret;
    }

    return FAIL;
}




int CFT::item_find(const char *group_name, const char *item_name, int mode, int start_pos)
{
    if (mode != 0 && mode != 1) {   //未知的模式
        return FAIL;
    }

    int pos_group_begin = group_find(group_name, 1, start_pos);  //匹配的组的第一行

    if (pos_group_begin == FAIL) {
        return FAIL;
    }

    int pos_cur = pos_group_begin;    //当前行

    while (pos_group_begin != FAIL) {   //以防多组重名

        fp.seekp(pos_group_begin, ios::beg);

        string strline;
        string tmp_item_name, tmp_item_value;

        while (getline(fp, strline, '\n').good() && !is_a_group_name(strline)) {    //遍历当前组中连续的行
            int kkkk, ffff;
            if ((kkkk = item_analyze(strline, tmp_item_name, tmp_item_value)) && (ffff = tmp_item_name == item_name)) {   //项存在
                switch (mode) {
                    case 0:
                        return pos_cur;
                        break;
                    case 1:
                        return pos_cur = int(fp.tellp());
                        break;
                    default:
                        return FAIL;
                }
            }

            pos_cur = (int)fp.tellg();
        }
        fp.clear();             //重要!!!
        pos_group_begin = group_find(group_name, pos_cur);
    }

    fp.clear(); //重要!!
    return FAIL;    //到这说明有组，但没找到项
}


int CFT::to_win(int start_pos)
{
    int pos = 0, lstpos = 0;
    char cur, lst = 0;
    string tt = "\r";


    fp.seekp(pos, ios::beg);
    if (!is_open() || !fp.good() || open_opt != OPEN_OPT_RDWR) {
        fp.clear();
        return 0;
    }

    while (fp.read(&cur, 1).good()) {

        if (cur == 0xA && lst != 0xD) {
            add(pos, tt);
            fp.seekp((++pos) + 1, ios::beg);
        }

        pos++;

        lst = cur;
    }

    fp.clear();         //重要!!!
    return 1;
}