/* ���� 2051995 ���� */

#include "90-02-b2.h"
#include "../include/hehepigIO.h"
#include "../include/cmd_console_tools.h"
#include <iostream>
#include <Windows.h>
using namespace std;

void beginHint()
{
    puts("�ϳ�ʮ˵��");
    puts("");
    puts("============================================");
    puts("֧�ּ���������\n");
    puts("��������в���������ʹ�÷������س������������в���\n");
    puts("���̲��������˸��߽磬��ѭ���ˣ����ڿ��ٵ��߽磩\n");
    puts("ѡ�񣨻س��������������ĳһ��������Ժϳɣ���Ѻϳ����������ʾ");
    puts("���ٴ�ѡ����Ǹ��������ڵĿ飬�Ὣ�������������������ѡ���λ�úϳ�һ����1���������ٴ�ѡ�����������Ŀ飬����Ϊ����ѡ��ϳ�������һ��ѡ�����Ϸ�\n");
    puts("��������ÿ�κϳ����÷֣������ɵ���*�����������С");
    puts("============================================\n");
    puts("��Ϸ�а� q �����˳�\n");

    cout << "��q�˳�������������..." << endl;
    int opr = GetKey();
    if (opr == 'q' || opr == 'Q') {
        exit(0);
    }

}

int main()
{

    beginHint();

    HCS_GAME HG;

    int r, c, n;
    int opr, oprRow, oprCol;

    while (1) {

        oprRow = oprCol = 0;
        cct_cls();
        r = GetInt("��������[5..15]\n >", 4, 15);
        c = GetInt("��������[5..15]\n >", 4, 15);
        n = GetInt("����ϳ�Ŀ����[5..10]\n >", 5, 10);

        HG.reset(r, c, n);

        while (1) {
            opr = HG.getOpr(oprRow, oprCol);

            if (opr == -1 || opr == '\r') {     //���������س�
                
                if (HG.isSelected(oprRow, oprCol)) {    //ȷ�ϣ�����
                    int tmp = HG.getVal(oprRow, oprCol) +1 ;    //��Ҫ��ӵ�����
                    int cnt;

                    cnt = HG.confirm();   //����

                    HG.addScore(tmp * cnt);

                    for (int i = 0; i < 5; i++) {       //��������
                        HG.setVal(oprRow, oprCol, 0);
                        Sleep(50);
                        HG.setVal(oprRow, oprCol, tmp);
                        Sleep(50);
                    }
                    HG.setVal(oprRow, oprCol, tmp);     //�ϳ�

                    HG.down();  //�½�����
                }
                else {  //ѡ��
                    HG.select(oprRow, oprCol);
                }
            }
            else if (opr == key_left) {
                HG.focusSwitch(oprRow, oprCol, 0);
                oprCol = max(oprCol - 1, 0);
                HG.focusSwitch(oprRow, oprCol, 1);
            }
            else if (opr == key_right) {
                HG.focusSwitch(oprRow, oprCol, 0);
                oprCol = min(oprCol + 1, c - 1);
                HG.focusSwitch(oprRow, oprCol, 1);
            }
            else if (opr == key_up) {
                HG.focusSwitch(oprRow, oprCol, 0);
                oprRow = max(oprRow - 1, 0);
                HG.focusSwitch(oprRow, oprCol, 1);
            }
            else if (opr == key_down) {
                HG.focusSwitch(oprRow, oprCol, 0);
                oprRow = min(oprRow + 1, r - 1);
                HG.focusSwitch(oprRow, oprCol, 1);
            }
            else if (opr == 'q' || opr == 'Q') {    //�˳�
                cct_cls();
                break;
            }

            if (HG.checkWin()) {
                break;
            }
        }

        cout << "��q�˳����������¾�" << endl;
        opr = GetKey();
        if (opr == 'q' || opr == 'Q') {
            break;
        }

    }
}