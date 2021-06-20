/* ���� 2051995 ���� */
#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <io.h>
#include "../include/cfg_file_tools.h"
using namespace std;

/* ---------------------------------------------------------------
     �������������Ҫstatic�������ڲ������ã�
   ---------------------------------------------------------------- */


   /***************************************************************************
     �������ƣ�
     ��    �ܣ�
     ���������
     �� �� ֵ��
     ˵    �������б�Ҫ�����������������
   ***************************************************************************/
CFT::CFT()
{
    filename = "";
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �������б�Ҫ�����������������
***************************************************************************/
CFT::~CFT()
{
    this->close();
    is_open_flag = false;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
CFT::CFT(const char *cfgname, int opt)
{
    open(cfgname, opt);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void CFT::open(const char *cfgname, int opt)
{
    this->close();  //�Ȱ�ǰ��򿪹��Ĺص�

    is_open_flag = false;

    filename = (cfgname == NULL) ? "" : cfgname;

    if (!reopen_opt(opt)) {
        cout << "���ļ� [" << filename << "] ʧ��" << endl;
        return;
    }

    is_open_flag = true;

    to_win();   //�Ȱ�ԭ���������ļ���֤Ϊ Windows ��ʽ

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    �����򿪳ɹ�����true�����򷵻�false
***************************************************************************/
bool CFT::is_open(void)
{
    return is_open_flag && fp.is_open();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void CFT::close()
{
    if (fp.is_open())
        fp.close();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����C++17����filesystem::resize_fileֱ�ӿ����޸��ļ���С��֮ǰ�İ汾��
                   ��˱�����������C�ľɷ���
        ��ʵ�֣���Ҫ��
***************************************************************************/
int CFT::file_resize(int newsize)
{
    /* �����С�Ǹ�����ֱ�ӷ���-1 */
    if (newsize < 0)
        return -1;

    if (!fp.good())
        fp.clear();

    /* ȡfp��ǰָ��λ�� */
    int now_pos = int(fp.tellg());

    /* ���ȣ�C++��ʽ�ر� */
    this->close();

    /* ��Σ�C��ʽ�򿪲��ı��ļ���С */
    FILE *fp_cstyle;
    fp_cstyle = fopen(filename.c_str(), "rb+"); //rw��ʽ��
    if (fp_cstyle) {
        _chsize(_fileno(fp_cstyle), newsize);
        fclose(fp_cstyle);
    }
    else
        cout << "open [" << filename << "] for resize failed" << endl;

    /* �ٴΣ�C++��ʽ���´� */
    fp.open(filename, ios::in | ios::out | ios::binary);
    if (fp.is_open() == 0) {
        cout << "reopen [" << filename << "] after resize failed" << endl;
        return -1;
    }

    /* �ļ�ָ���ƶ���ԭλ�� */
    fp.seekg(now_pos < newsize ? now_pos : newsize, ios::beg);

    return 0;
}

/***************************************************************************
    �������ƣ�
    ��    �ܣ�
    ���������
    �� �� ֵ���ɹ�����1�����򷵻�0
    ˵    ���������Ѵ��ڣ����ظ����ӣ�ֱ�ӷ���0
***************************************************************************/
int CFT::group_add(const char *group_name)
{
    reopen_opt(open_opt);

    if (!is_open() || open_opt != OPEN_OPT_RDWR) {  //״̬���ԵĶ��󲻽��в���
        return 0;
    }

    int pos = this->group_find(group_name, 0, 0);  //�������ļ������Ƿ��Ѿ��и���

    if (pos == FAIL) {  //��û�и��飬�����ļ�������һ�� "\n[group_name]\n"
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

    return 1;   //�ܵ�����˵���ɹ���
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ɾ���� group_name �Լ���������ֱ��û������Ϊ�˵���
  ���������
  �� �� ֵ��ɾ�����������п����������ͬ����
  ˵    ����
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
        pos_end = group_find(group_name, 2, pos_beg);   //һ����˵�����ﲻ�������

        del(pos_beg, pos_end - pos_beg);    //ɾ�����飨���ļ�����ֱ����β�Ķ�����ǰŲ��

        ret++;
    }

    return ret;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_add(const char *group_name, const char *item_name, const int item_value)
{
    ostringstream os;
    os << item_value;

    return item_add(group_name, item_name, os.str());
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_add(const char *group_name, const char *item_name, const double item_value)
{
    ostringstream os;
    os << item_value;

    return item_add(group_name, item_name, os.str());
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_add(const char *group_name, const char *item_name, const char *item_value)
{
    ostringstream os;
    os << item_value;

    return item_add(group_name, item_name, os.str());
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ���ɹ�����1�����򷵻�0
  ˵    ��������ڣ�������ڣ�������ǰ��������ǰ��λ�üӣ�����ڡ��鲻���ڶ�ֱ�ӷ���0
***************************************************************************/
int CFT::item_add(const char *group_name, const char *item_name, const string &item_value)
{
    reopen_opt(open_opt);
    if (!is_open() || open_opt != OPEN_OPT_RDWR || item_name == NULL) {
        return 0;
    }

    int pos_first_group_begin = group_find(group_name, 1, 0);

    if (pos_first_group_begin == FAIL) {    //�鲻����
        return 0;
    }

    int pos_item_begin = item_find(group_name, item_name, 0);
    if (pos_item_begin != FAIL) {   //����ڣ������
        return 0;
    }

    //����˵������ڣ�����ڣ���Ҫ�ڵ�һ��ƥ�����ĵ�һ�����
    add(pos_first_group_begin, string(item_name).append(" = ").append(item_value).append("\n"));

    return 1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_add(const char *group_name, const char *item_name, const char item_value)
{
    ostringstream os;
    os << item_value;

    return item_add(group_name, item_name, os.str());
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_add(const char *group_name, const char *item_name)
{
    return item_add(group_name, item_name, "");
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ�����سɹ�ɾ������������Ϊ�����ж��ͬ��ͬ���
  ˵    ����
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ���ɹ��򷵻�1�����򷵻�0
  ˵    �����������ڸ�����ڿ��Էŵ���ǰ�洴��һ�����������ڣ����޸���ǰ���Ǹ����ҷ���1
***************************************************************************/
int CFT::item_update(const char *group_name, const char *item_name, const int item_value)
{
    ostringstream os;
    os << item_value;
    return item_update(group_name, item_name, os.str());
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_update(const char *group_name, const char *item_name, const double item_value)
{
    ostringstream os;
    os << item_value;
    return item_update(group_name, item_name, os.str());
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_update(const char *group_name, const char *item_name, const char *item_value)
{
    ostringstream os;
    os << item_value;
    return item_update(group_name, item_name, os.str());
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ���ɹ��򷵻�1�����򷵻�0
  ˵    �����������ڸ�����ڿ��Էŵ���ǰ�洴��һ�����������ڣ����޸���ǰ���Ǹ����ҷ���1
***************************************************************************/
int CFT::item_update(const char *group_name, const char *item_name, const string &item_value)
{
    if (!is_open() || open_opt != OPEN_OPT_RDWR || item_name == NULL) {
        return 0;
    }

    int pos = item_find(group_name, item_name);
    if (pos == FAIL) {  //������
        return item_add(group_name, item_name, item_value);
    }
    else {  //���ڣ��޸ĵ�һ��
        item_del(group_name, item_name);
        add(pos, string(item_name).append(" = ").append(item_value).append("\n"));
        return 1;
    }
    return 0; //�˾������Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_update(const char *group_name, const char *item_name, const char item_value)
{
    ostringstream os;
    os << item_value;
    return item_update(group_name, item_name, os.str());
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_update(const char *group_name, const char *item_name)
{
    return item_update(group_name, item_name, "");
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��ȡֵ�ɹ�����1�����򷵻�0
  ˵    ����
***************************************************************************/
int CFT::item_get_value(const char *group_name, const char *item_name, int &item_value)
{
    string tmpstr;
    int f = item_get_value(group_name, item_name, tmpstr);
    istringstream is(tmpstr);
    return (f && !(is >> item_value).fail());
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_get_value(const char *group_name, const char *item_name, double &item_value)
{
    string tmpstr;
    int f = item_get_value(group_name, item_name, tmpstr);
    istringstream is(tmpstr);
    return (f && !(is >> item_value).fail());
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
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
        fp.clear();     //��Ҫ
        return 0;
    }

    return item_analyze(strline, tmpstr_n, item_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_get_value(const char *group_name, const char *item_name, char &item_value)
{
    string tmpstr;
    int f = item_get_value(group_name, item_name, tmpstr);
    istringstream is(tmpstr);
    return (f && !(is >> item_value).fail());
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CFT::item_get_value(const char *group_name, const char *item_name)
{
    string tmpstr;
    int f = item_get_value(group_name, item_name, tmpstr);
    return f;
}











/* �����ǹ��ߺ��� */

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
    while (tmp < sz && strline[tmp] != '#' && !(strline[tmp] == ' ' || strline[tmp] == '\t' || strline[tmp] == '\r'))     // # ���к���Ķ���ע��
        value += strline[tmp++];

    return true;
}




/* ������ class CFT �� protected �ĺ��� */


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
                cout << "δ֪�Ĵ򿪷�ʽ [" << opt << "]" << endl;
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
    sz_old = int(fp.tellg());    //��ȡ�ļ���С
    fp.seekp(0, ios::beg);

    char buf;
    for (int i = pos_start, j = pos_start + size; ; i++, j++) {
        fp.seekp(j, ios::beg);
        fp.read(&buf, 1);
        if (buf == 0x1A || !fp.good())      //ע��һ�� 0x1A
            break;
#if DEBUG_CFG_HEHEPIG
        cout << hex << int(buf) << endl;
#endif
        fp.seekp(i, ios::beg);
        fp.write(&buf, 1);
    }

    file_resize(sz_old - size);   //�޸Ĵ�С���Ѻ���������Ķ���ȥ��
    reopen_opt(open_opt);

    to_win();   //��Ҫ!
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
        to_win();   //��Ҫ
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

    to_win();   //��Ҫ
}



//���Ҹ��飬���ض������֮��õ��� tell() ֵ
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

    fp.clear();     //��Ҫ!!!!!!!!

    if (flag) { //˵����ƥ�䵽���һ����
        fp.seekg(0, ios::end);
        int ret = (int)fp.tellg();
        fp.seekg(0, ios::beg);
        return ret;
    }

    return FAIL;
}




int CFT::item_find(const char *group_name, const char *item_name, int mode, int start_pos)
{
    if (mode != 0 && mode != 1) {   //δ֪��ģʽ
        return FAIL;
    }

    int pos_group_begin = group_find(group_name, 1, start_pos);  //ƥ�����ĵ�һ��

    if (pos_group_begin == FAIL) {
        return FAIL;
    }

    int pos_cur = pos_group_begin;    //��ǰ��

    while (pos_group_begin != FAIL) {   //�Է���������

        fp.seekp(pos_group_begin, ios::beg);

        string strline;
        string tmp_item_name, tmp_item_value;

        while (getline(fp, strline, '\n').good() && !is_a_group_name(strline)) {    //������ǰ������������
            int kkkk, ffff;
            if ((kkkk = item_analyze(strline, tmp_item_name, tmp_item_value)) && (ffff = tmp_item_name == item_name)) {   //�����
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
        fp.clear();             //��Ҫ!!!
        pos_group_begin = group_find(group_name, pos_cur);
    }

    fp.clear(); //��Ҫ!!
    return FAIL;    //����˵�����飬��û�ҵ���
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

    fp.clear();         //��Ҫ!!!
    return 1;
}