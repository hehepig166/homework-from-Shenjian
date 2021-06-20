/* 朱泽凯 2051995 济美 */
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <Windows.h>
#include <iomanip>

#include "../include/cmd_console_tools.h"
#include "../include/hehepigMenu.h"
#include "../include/CODE_GB2312.h"
#include "../include/hehepigIO.h"
#include "90-02-b4-pro.h"
#include "../include/cfg_file_tools.h"
using namespace std;

//#define RGB(x,y,z) (((x)<<4)|((y)<<2)|((z)))

const char *CFG_FILE = "led.cfg";

void test()
{
    char tmpstr[20];
    bool f[16][16];
    while (1) {
        puts("\n输入一个全角字符，或输入0退出");
        cin >> tmpstr;
        if (tmpstr[0] == '0')
            break;
        //show_pixels(tmpstr);
        to_image(f, tmpstr, 0);
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                putchar(' ');
                putchar(f[i][j] ? '#' : ' ');
            }
            puts("");
        }
    }
}


const char *const texts[] = {
        "罗小黑战记周更啦！罗小黑战记周更啦！罗小黑战记周更啦！罗小黑战记周更啦！罗小黑战记周更啦！罗小黑战记周更啦！",
        "到处闲逛警谭乔，\n路遇违章把话聊。\n警光询问要去哪？\n成华大道二仙桥。",
        "老年痴呆的牛爷爷说：“你说你图啥呢？”",
        "朋友们好啊，我是混元形意太极门掌门人马保国。\n刚才有个朋友问我马老师发生什么事了，我说怎么回事，给我发了一几张截图。\n我一看，嗷，原来是昨天，有两个年轻人，三十多岁，一个体重九十多公斤，一个体重八十多公斤。",
        "伞兵一号卢本伟准备就绪！！！\n",
        "你们可能不知道只用二十万赢到五百七十八万是什么概念，我们一般只会用两个字来形容这种人：“赌怪”！",
        "给阿姨倒杯茶好吧，阿姨给你倒一杯卡布奇诺。给阿姨倒一杯卡布奇诺！开始你的炸弹秀，炸他炸他。漂亮！十七张牌你能秒我？你能秒杀我？！你今天能十七张牌把卢本伟秒了，我！当！场！就把这个电脑屏幕吃掉！！！",
        "火必舍，出必不，出必被，撞必穿，坐必减，跳必挂，亲必是，癌必早，车必对，墓必宝，狗必不，车必绿，必不断，汤多菜，蹲不排，蛋必加，保必生，难必平，稀必纸，癌必治，练不坏，没必八，拉必有，投必断，吃隔医，土必全，便不血，H转阴，黄不卡，阴必超",
        "群友早起卷绩点，\n而你窝着卷被子。\n你怎么能睡得着觉！",
        "局部坏死的反义词是啥？\n整个好活。"
};

static void modify(const char *(&text), int &is_fan, int &num_row, int &num_col, unsigned  int &bgcolor, unsigned int &fgcolor, int &size)
{
    static char static_text[105];
    puts("\nmodify");
    int opr;
    const char *modify_hint[] = {
        "修改显示文本内容",
        "切换简繁",
        "修改屏幕显示行列数",
        "调整模式 1（*点阵）文字颜色",
        "调整模式 2（hdc）文字颜色",
        "修改字体大小",
        "修改模式2里小点的直径",
        "调整模式 2 (hdc) 背景颜色",
        "返回菜单界面",

        "一些内置文本",
        "自己输入文本"
    };

    while (1) {
        opr = Menu_char(9, "12345678c", modify_hint);

        if (opr == '1') {
            opr = Menu_char(2, "12", &modify_hint[8]);
            if (opr == '1') {
                char tmp[100];
                sprintf(tmp, "输入内置文本索引（%d-%d）\n> ", 0, sizeof(texts) / sizeof(char *) - 1);
                text = texts[GetInt(tmp, 0, sizeof(texts) / sizeof(char *) - 1)];
            }
            else if (opr == '2') {
                puts("输入要显示的文本（只能包含连续的一串中文字符，回车结束，不可超过50个字）\n> ");
                cin >> static_text;
                text = static_text;
            }
            continue;
        }
        else if (opr == '2') {
            is_fan ^= 1;
            cout << "知道了，现在切换为 " << ((is_fan & 1) ? "繁" : "简") << endl;
            continue;
        }
        else if (opr == '3') {
            cout << "当前行列信息：[行，列] = [" << num_row << "，" << num_col << "]" << endl;
            num_row = GetInt("行（1..10）\n> ", 1, 10);
            num_col = GetInt("列（1..10）\n> ", 1, 10);
            continue;
        }
        else if (opr == '4') {
            cout << "当前颜色编号 " << (fgcolor >> 24) << endl;
            fgcolor &= 0xffffff;
            fgcolor |= GetInt("输入颜色编号（1..14）\n> ", 1, 14) << 24;
            continue;
        }
        else if (opr == '5') {
            cout << "当前颜色RGB " << ((fgcolor & 0xff0000) >> 16) << ' ' << ((fgcolor & 0xff00) >> 8) << ' ' << (fgcolor & 0xff) << endl;
            fgcolor &= (~0xffffff);
            fgcolor |= (GetInt("R > ", 0, 255)) << 16;
            fgcolor |= (GetInt("G > ", 0, 255)) << 8;
            fgcolor |= (GetInt("B > ", 0, 255));
            continue;
        }
        else if (opr == '6') {
            cout << "当前字体大小 " << (size & 0xff) << endl;
            size &= (~0xff);
            size |= GetInt("输入字体大小（1..16）\n> ", 1, 16);
            continue;
        }
        else if (opr == '7') {
            cout << "当前直径 " << ((size & 0xff00) >> 8) << endl;
            size &= (~0xff00);
            size |= GetInt("输入直径（1..50）\n> ", 1, 50) << 8;
            continue;
        }
        else if (opr == '8') {
            cout << "当前颜色RGB " << ((bgcolor & 0xff0000) >> 16) << ' ' << ((bgcolor & 0xff00) >> 8) << ' ' << (bgcolor & 0xff) << endl;
            bgcolor &= (~0xffffff);
            bgcolor |= (GetInt("R > ", 0, 255)) << 16;
            bgcolor |= (GetInt("G > ", 0, 255)) << 8;
            bgcolor |= (GetInt("B > ", 0, 255));
        }
        else if (opr == 'c') {
            break;
        }
    }
}






static void read_from_file(const char *(&text), int &is_fan, int &num_row, int &num_col, unsigned  int &bgcolor, unsigned int &fgcolor, int &size)
{
    CFT cft(CFG_FILE, 0);

    if (!cft.is_open()) {
        cout << "无法打开配置文件 " << CFG_FILE << " ，将使用默认值，按回车继续" << endl;
        GetKey('\r');
        return;
    }

    int tmp_int;
    string tmp_string;

    puts("读取全局设置组");

    if (cft.item_get_value("全局设置", "行数", tmp_int)) {
        num_row = tmp_int;
        cout << "行数 " << num_row << endl;
    }
    if (cft.item_get_value("全局设置", "列数", tmp_int)) {
        num_col = tmp_int;
        cout << "列数 " << num_col << endl;
    }
    if (cft.item_get_value("全局设置", "背景色", tmp_string)) {
        istringstream is(tmp_string);
        is >> hex >> tmp_int;
        if (tmp_int >= 0 && tmp_int <= 0xFFFFFF) {
            bgcolor = tmp_int;
            cout << "背景色 " << bgcolor << endl;
        }
    }
    if (cft.item_get_value("全局设置", "简繁", tmp_string) && (tmp_string == "简" || tmp_string == "繁")) {
        is_fan = tmp_string == "繁";
        cout << "简繁 " << tmp_string << endl;
    }

    cft.close();

    puts("读取结束，回车继续");
    GetKey('\r');
}


static void write_to_file(const char *(&text), int &is_fan, int &num_row, int &num_col, unsigned  int &bgcolor, unsigned int &fgcolor, int &size)
{
    CFT cft(CFG_FILE, 1);

    if (!cft.is_open()) {
        return;
    }

    puts("修改配置文件");

    cft.group_add("全局设置");

    if (cft.item_update("全局设置", "行数", num_row)) {
        puts("修改全局行数成功");
    }
    if (cft.item_update("全局设置", "列数", num_col)) {
        puts("修改全局列数成功");
    }
    ostringstream os;
    os << hex << setw(6) << setfill('0') << bgcolor << setfill(' ');
    if (cft.item_update("全局设置", "背景色", os.str())) {
        puts("修改全局背景色成功");
    }
    if (cft.item_update("全局设置", "简繁", is_fan ? "繁" : "简")) {
        puts("修改全局简繁成功");
    }

    cft.close();

    puts("修改结束，回车继续");
    GetKey('\r', '\r');
}



int main()
{
    const char *text = texts[6];
    //size:     0xff 存size，0xff00存直径
    //color:    0xffffff 存RGB，0xff000000 存cct_setcolor
    int is_fan = 1, num_row = 4, num_col = 10, size = (5 << 8) | 6;
    unsigned int bgcolor = 0, fgcolor = (COLOR_PINK << 24) | 0xE066FF;

    read_from_file(text, is_fan, num_row, num_col, bgcolor, fgcolor, size);


    cct_setconsoleborder(120, 30, 120, 9001);
    cct_setfontsize("新宋体", 16);
    cct_setcolor();
    cct_cls();

    const char *hint[] = {
        "模拟LED信息显示-用*表示每个点",
        "模拟LED信息显示-用hdc_point画每个点",
        "模拟LED信息显示-变换颜色",
        "模拟LED信息显示-向内扩散",
        "模拟LED信息显示-溶解",
        "模拟LED信息显示-不限长文本循环播放（换行符会失效）",
        "测试用",
        "修改屏幕状态",
        "退出"
    };

    int done = 0;

    while (!done) {
        cct_setcolor();
        cct_cls();
        int opr = Menu_char(9, "1234560cz", hint);

        switch (opr) {
            case('0'):
                test();
                break;
            case ('1'):
                print_basic(text, is_fan, num_row, num_col, bgcolor >> 24, fgcolor >> 24, size, 0);
                break;
            case ('2'):
                print_basic(text, is_fan, num_row, num_col, bgcolor & 0xffffff, fgcolor & 0xffffff, size, 1);
                break;
            case('3'):
                print_colorful(text, is_fan, num_row, num_col, size);
                break;
            case('4'):
                print_zoom(text, is_fan, num_row, num_col, bgcolor & 0xffffff, fgcolor & 0xffffff, size);
                break;
            case('5'):
                print_dissolve(text, is_fan, num_row, num_col, bgcolor & 0xffffff, fgcolor & 0xffffff, size);
                break;
            case('6'):
                print_inf(text, is_fan, num_row, num_col, bgcolor & 0xffffff, fgcolor & 0xffffff, size);
                break;
            case('c'):
                modify(text, is_fan, num_row, num_col, bgcolor, fgcolor, size);
                break;
            case('z'):
                int update_cft = GetInt("是否要更新配置文件？(0不要，1要)", 0, 1);
                if (update_cft) {
                    write_to_file(text, is_fan, num_row, num_col, bgcolor, fgcolor, size);
                }

                done = 1;
                break;
        }
    }



    return 0;
}