/* ���� 2051995 ���� */
#include "./90-02-b1-star_play.h"
#include "../include/cmd_console_tools.h"
#include "../include/hehepigIO.h"
#include "../include/hehepigDraw.h"
#include "../include/hehepigMenu.h"
#include <ctime>
#include <iostream>
using namespace std;

#define MaxN 10
#define ColNum 5
#define RAN(x,y) (rand()%((y)-(x)+1)+(x))

const int HBMCOL[] = { COLOR_WHITE, COLOR_PINK, COLOR_GREEN, COLOR_YELLOW, COLOR_RED, COLOR_CYAN };

/// <summary>
/// �Ը����ĵ�Ϊ�����ֵһ������ͨ��
/// </summary>
/// <param name="x, y">���λ��</param>
/// <param name="Row, Col">��ͼ��С</param>
/// <param name="Map">��ͼֵ����</param>
/// <param name="vis">�ṩ�����ڼ�¼�ľ�����ͨ�ĸ�ֵΪ1������ͨ��Ϊ0��</param>
/// <param name="cnt">���ڼ���</param>
void FindConnected(int x, int y, int Row, int Col, int Map[][MaxN], int vis[][MaxN], int* cnt, int sides = 4) {
    static int fx[8][2] = { {0,1},{1,0},{0,-1},{-1,0},{-1,-1},{-1,1},{1,-1},{1,1} };
    vis[x][y] = 1;
    (*cnt)++;

    for (int F = 0; F < sides; F++) {
        int xx = x + fx[F][0];
        int yy = y + fx[F][1];
        if (xx >= 0 && xx < Row && yy >= 0 && yy < Col && !vis[xx][yy] && Map[xx][yy] == Map[x][y])
            FindConnected(xx, yy, Row, Col, Map, vis, cnt, sides);
    }
}


/// <summary>
/// �ں�һ�� int ���󣬸��ֺ����Ըþ������
/// </summary>
class Play {
public:
    int Map[MaxN][MaxN];
    int Row;
    int Col;

    int vis[MaxN][MaxN];    // ���������õ�����
    int ConCnt;             // ���������ĸ���

    int OldScore;           // ��һ�β�������ܵ÷�
    int Score;              // �ܵ÷�
    int Num;                // ��ʣ��������

    /// <summary>
    /// �������У��½���ͼ
    /// </summary>
    void Init() {
        //srand(time(0));

        Row = GetInt("�У�3-10�� > ", 3, MaxN);
        cout << "  !������" << Row << endl;
        Col = GetInt("�У�3-10�� > ", 3, MaxN);
        cout << "  !������" << Col << endl;
        for (int i = 0; i < Row; i++)
            for (int j = 0; j < Col; j++) {
                vis[i][j] = 0;
                Map[i][j] = RAN(1, ColNum);
            }

        OldScore = 0;
        Score = 0;
        Num = 0;
    }

    /// <summary>
    /// ������� (x,y) ��������ͬ�ģ����ظ����������������õ㣩
    /// </summary>
    int Find(int x, int y) {

        if (Map[x][y] == 0)
            return 0;

        for (int i = 0; i < Row; i++)
            for (int j = 0; j < Col; j++)
                vis[i][j] = 0;
        ConCnt = 0;    //-1��Ϊ�������õ��Ĳ������Լ�

        FindConnected(x, y, Row, Col, Map, vis, &ConCnt);
        return ConCnt;
    }

    /// <summary>
    /// ������һ�� FindConnected �ó��� vis �е��������ǣ�A�ж�Ӧ��ֵ��Ϊ0��
    /// </summary>
    void Confirm() {
        for (int i = 0; i < Row; i++)
            for (int j = 0; j < Col; j++)
                if (vis[i][j])
                    Map[i][j] = vis[i][j] = 0;

        Num -= ConCnt;
        OldScore = Score;
        Score += (ConCnt * ConCnt * 5);
        
    }

    /// <summary>
    /// ����A�����µ�����λ
    /// </summary>
    void Down() {
        for (int j = 0, cur = Row - 1; j < Col; j++, cur = Row - 1) {
            for (int i = Row - 1; i >= 0; i--)
                if (Map[i][j])
                    Map[cur--][j] = Map[i][j];
            while (cur >= 0)
                Map[cur--][j] = 0;
        }
    }

    /// <summary>
    /// ����A������Ų����λ
    /// </summary>
    void Left() {
        int tmp[MaxN] = { 0 };
        int i, j, jj, cnt, tcnt = 0;
        for (j = 0, cnt = 0; j < Col; j++) {
            for (i = tcnt = 0; i < Row; i++)
                tcnt += (Map[i][j] != 0);
            tmp[j] = cnt;
            cnt += (tcnt == 0);
        }
        for (j = 0, jj = j - tmp[j]; j < Col; j++, jj = j - tmp[j])
            for (i = 0; i < Row; i++)
                Map[i][jj] = Map[i][j];

        while (jj < Col) {
            for (i = 0; i < Row; i++)
                Map[i][jj] = 0;
        }

    }

    /// <summary>
    /// ����Ƿ��޷��ƶ�
    /// </summary>
    bool CheckFail() {
        for (int i = 0; i < Row; i++)
            for (int j = 0; j < Col; j++)
                if (Find(i, j)>1)
                    return false;


        OldScore = Score;
        Score += Num > 10 ? 0 : (10 - Num) * 10;

        return true;
    }


    /// <summary>
    /// ����ǰ������Ϣ���µ� HBM ��
    /// </summary>
    /// <param name="HBM"></param>
    void Upgrade(hehepig_block_map* HBM) {
        for (int i = 0; i < Row; i++)
            for (int j = 0; j < Col; j++) {
                HBM->A[i][j].Val = Map[i][j];
                HBM->A[i][j].BgColor = HBMCOL[Map[i][j]];
                HBM->A[i][j].FtColor = (Map[i][j] == 0 || vis[i][j]) ? COLOR_WHITE : COLOR_BLACK;
            }
    }
};


///================================================
/// DigitalPlay
/// 
///================================================

void DigitalPlay::Init() {
    static Play _P;
    P = &_P;
    P->Init();
    Row = P->Row;
    Col = P->Col;

    HBM = hehepig_block_map(Row, Col);
    P->Upgrade(&HBM);

    HBM.DigitalLogError("\nǰ���飺\n");
    HBM.DigitalPrint();

}

void DigitalPlay::GetXY(int& x, int& y) {
    HBM.DigitalLogError("\n��������[����b2] > ");
    while (1) {
        x = GetKey();
        if (isalpha(x) && (x = (toupper(x) - 'A')) < Row)
            break;
    }
    putchar(x + 'A');   //����
    y = GetKey('0', '0' + Col - 1, 1) - '0';
}


/*****
A       "�������ҳ����������ʶ",
B       "���������һ���������ֲ�����ʾ��",
C       "���������һ�أ��ֲ�����ʾ��",
*****/
void DigitalPlay::Begin(int Mode) {

    Init();

    int nxtX, nxtY;

    while (1) {
        while (1) {
            GetXY(nxtX, nxtY);
            if (P->Find(nxtX, nxtY)>1) {
                HBM.DigitalLogError("\n�˴���Ч���������������������ɫ�ֱ�ʾ��Ц�����ǣ�\n");
                P->Upgrade(&HBM);
                HBM.DigitalPrint();
                break;
            }
            else {
                HBM.DigitalLogError("\n��Ч�����˴����ǲ�����\n");
            }
        }

        if (Mode == 'A') {  //=======================================================A
            HBM.DigitalLogError("\n��������q���ز˵�\n");
            GetKey('q', 'q');
            return;
        }

        HBM.DigitalLogError("\n�Ƿ��������ǣ�\n");
        const char* tmpstr[] = { "ȷ��ѡ�����","����ѡ�����" ,"������Ϸ" };
        int opr = Menu_char(3, "YNQ", tmpstr);
        opr = toupper(opr);
        putchar(opr);
        puts("");
        if (opr == 'N')
            continue;
        else if (opr == 'Q') {
            return;
        }
        else if (opr == 'Y') {
            // ȷ��ɾ��
            P->Confirm();
            P->Upgrade(&HBM);
            HBM.DigitalLogError("\nɾ����\n");
            HBM.DigitalPrint();
            HBM.DigitalLogError("\n���س������������¿�Ų��\n");
            GetKey('\r', '\r');

            // ���¿�Ų
            P->Down();
            P->Upgrade(&HBM);

            for (int i = 0; i < Row; i++) {
                for (int j = 0; j < Col; j++)
                    cout << P->Map[i][j] << " ";
                puts("");
            }

            HBM.DigitalPrint();
            HBM.DigitalLogError("\n���س���������������Ų��\n");
            GetKey('\r', '\r');

            // ������Ų
            P->Left();
            P->Upgrade(&HBM);
            HBM.DigitalPrint();
        }

        if (Mode == 'B') {  //=======================================================B
            HBM.DigitalLogError("\n��������q���ز˵�\n");
            GetKey('q', 'q');
            return;
        }

        if (P->CheckFail()) {
            cct_setcolor(0, COLOR_RED);
            cout << "�������ܵ÷֣�" << P->Score << " = " << P->OldScore << " + " << P->Score - P->OldScore << endl;
            HBM.DigitalLogError("\n��q���ز˵�\n");
            cct_setcolor();
            GetKey('q', 'q');
            return;
        }
        else {
            cout << "Ŀǰ�÷֣�" << P->Score << " = "<<P->OldScore <<" + " <<P->Score-P->OldScore <<endl;
        }
    }

}
