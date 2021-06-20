/* ���� 2051995 ���� */

#ifndef __HEHEPIG_CFG_FILE_TOOLS__
#define __HEHEPIG_CFG_FILE_TOOLS__

#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum class ITEM_TYPE {
    Int,		//Ϊ�����int��ͻ����������ĸ��д��ʽ
    Double,
    String,
    Character,
    Null
};

#define OPEN_OPT_RDONLY		0	//��ֻ����ʽ�򿪣��򲻿��򷵻�ʧ�ܣ�
#define OPEN_OPT_RDWR		1	//�Զ�д��ʽ�򿪣��򲻿��ļ��򴴽���

#define FAIL -1

bool is_a_group_name(const string str);


#define DEBUG_CFG_HEHEPIG 0

/* ---------------------------------------------------------------
     �������������Ҫ�Ķ���
     constȫ�ֱ���/staticȫ�ֱ���(����)/define/class/struct/enum/enum class/union��
   ---------------------------------------------------------------- */


bool group_name_analyze(const string &strline, string &name);

bool item_analyze(const string &strline, string &name, string &value);


/* ---------------------------------------------------------------
     class CFT�Ķ��壬���в��ֲ�Ҫ��������protected����������Ҫ�Ķ���
   ---------------------------------------------------------------- */
class CFT {
protected:
    string filename;
    fstream fp;

    int file_resize(int newsize);

    /* ---------------------------------------------------------------
         �������������Ҫ�Ķ��壨�ڲ��ã�
       ---------------------------------------------------------------- */
    bool is_open_flag;
    int open_opt;

    bool reopen_opt(int opt); //�ɹ����� true

    //ɾ������ pos_start ��ʼ����� size �ֽڵĶ���
    //�����ˣ���ɾ������ɾΪֹ
    void del(int pos_start, int size);      //ɾ������ȫ�������Ϊ�˱�֤������ to_win

    //�� pos_start �����㣩������� str
    //����� pos_start �ĺ�����
    void add(int pos_start, string str);    //�������ȫ������������ڽ�β����Ϊ�˱�֤������ to_win

    //�� start_pos ��ʼ���Ҹ��飬���� mode ���ض����Ӧ�� tell() ֵ��Ĭ�� mode = 0��
    //0: [group_name] ��һ��֮ǰ��seek֮�������һ�о��� [] ��һ�У�
    //1: [group_name] ��һ��֮�󣨶��� [] ��һ�е�seek��
    //2: [group_name] ��һ�����seek֮�������һ�о�����һ����� []�������Ѿ������һ�飬�򷵻����
    //��û�ҵ��򷵻� FAIL (-1)
    int group_find(const char *group_name, int mode = 0, int start_pos = 0);

    //�� start_pos ��ʼ���Ҹ���ĸ������ mode ���ض����Ӧ�� tell() ֵ��Ĭ�� mode = 0��
    //0: [group_name] ��һ��֮ǰ��seek֮�������һ�о��� [] ��һ�У�
    //1: [group_name] ��һ��֮�󣨶��� [] ��һ�е�seek��
    //��û�ҵ��򷵻� FAIL (-1)
    int item_find(const char *group_name, const char *item_name, int mode = 0, int start_pos = 0);

    //ת�� win ��ʽ��0A -> 0D 0A��
    //�ɹ���� win ��ʽ�򷵻� 1�����򷵻� 0
    //������Ч�������ˡ���ÿ�����޸ĵĲ���������һ��to_win
    int to_win(int start_pos = 0);

public:
    CFT();							//�޲ι��죬��filenameΪ""����
    CFT(const char *cfgname, int opt = OPEN_OPT_RDONLY);	//���ι��죬����ͬopen
    ~CFT();							//����

    void open(const char *cfgname, int opt = OPEN_OPT_RDONLY);
    bool is_open();	//����򿪳ɹ����򷵻�true�����򷵻�false
    void close();

    /* ��������ɾ�� */
    int group_add(const char *group_name);
    int group_del(const char *group_name);

    /* �����ӣ����������� */
    int item_add(const char *group_name, const char *item_name, const int item_value);
    int item_add(const char *group_name, const char *item_name, const double item_value);
    int item_add(const char *group_name, const char *item_name, const char *item_value);
    int item_add(const char *group_name, const char *item_name, const string &item_value);
    int item_add(const char *group_name, const char *item_name, const char item_value);
    int item_add(const char *group_name, const char *item_name);	//NULL

    /* ��ɾ�� */
    int item_del(const char *group_name, const char *item_name);

    /* ����£����������� */
    int item_update(const char *group_name, const char *item_name, const int item_value);
    int item_update(const char *group_name, const char *item_name, const double item_value);
    int item_update(const char *group_name, const char *item_name, const char *item_value);
    int item_update(const char *group_name, const char *item_name, const string &item_value);
    int item_update(const char *group_name, const char *item_name, const char item_value);
    int item_update(const char *group_name, const char *item_name);	//NULL

    /* ȡĳ���ֵ������������ */
    int item_get_value(const char *group_name, const char *item_name, int &item_value);
    int item_get_value(const char *group_name, const char *item_name, double &item_value);
    int item_get_value(const char *group_name, const char *item_name, char *item_value);
    int item_get_value(const char *group_name, const char *item_name, string &item_value);
    int item_get_value(const char *group_name, const char *item_name, char &item_value);
    int item_get_value(const char *group_name, const char *item_name);

    /* ---------------------------------------------------------------
         ���������������Ҫ�Ķ��壨��������ֵ�ֻ���������
       ---------------------------------------------------------------- */
};

#endif