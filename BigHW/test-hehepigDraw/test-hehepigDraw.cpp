#include "../include/hehepigDraw.h"
#include "../include/hehepigIO.h"
#include "../include/cmd_console_tools.h"
#include <iostream>
using namespace std;

void testDigit() {

    int n, m;
    
    n = GetInt("––[5,10] >", 5, 10);
    m = GetInt("¡–[5,10] >", 5, 10);

    hehepig_block_map A(n, m);
    A.DigitalLogError("≤‚ ‘\n");
    A.DigitalPrint();
}

void testGraphic1() {
    hehepig_block_graph G;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 6; j++)
            G.GotoPrint(i, j, "∏Ú");

    G.GotoPrint(4, 0, "π˛", 0, 7, 6);

    puts("");
}



void testGraphic() {
    hehepig_block_graph G;

    G.Init(2, 6, 1, 7, 5, 2);
    int aaaa=getchar();
    G.Init(3, 5, 1, 5, 0, 0);
    aaaa=getchar();
    G.ModifyBlock(2, 0, hehepig_block(1, COLOR_YELLOW));
    aaaa=getchar();

    G.Init(6, 8, 1, 3, 2, 10);
    aaaa=getchar();
    G.ModifyBlock(2, 0, hehepig_block(1, COLOR_GREEN));



    int lx=-1, ly=-1;
    while (1) {
        int xx, yy, ss, tt, k1, k2;
        int Row = 6;
        int Col = 8;
        cct_gotoxy(0, 0);
        tt = G.ReadKeyMouse(xx, yy, ss, k1, k2);
        printf("%d (%c %d) %d [%d %d]                                     ",tt,xx+'A',yy,ss,k1,k2);

        if (tt == CCT_MOUSE_EVENT) {
            if (xx < 0 || yy < 0) {
                xx = lx;
                yy = ly;
            }
            if (ss == 2) {
                break;
            }
        }
        else if (ss==CCT_KEYBOARD_EVENT){
            if (k1 == 224) {
                switch (k2) {
                    case 72:
                        xx--;
                        break;
                    case 75:
                        yy--;
                        break;
                    case 77:
                        yy++;
                        break;
                    case 80:
                        xx++;
                        break;
                }
                xx = min(xx, Row - 1);
                yy = min(yy, Col - 1);
                xx = max(xx, 0);
                yy = max(yy, 0);
            }
        }

        if (lx != xx || ly != yy) {
            G.ModifyBlock(xx, yy, hehepig_block(0, COLOR_CYAN));
            if (lx >= 0 && ly >= 0)
                G.ModifyBlock(lx, ly, COLOR_WHITE);
            lx = xx;
            ly = yy;
        }

    }


}


void testGraphic2() {
    static hehepig_block_map M;
    M.GraphicalInit(4, 6, 1, 6);
    M.A[1][3] = hehepig_block(1, COLOR_GREEN);
    M.GraphicalUpgrade();
    int xx, yy, sz;
    M.GetXY(xx, yy);

    sz = GetInt("new BlockSize:", 1, 7);
    M.ChangeBlockSize(sz);
    M.GetXY(xx, yy);
    M.A[xx][yy] = hehepig_block(1, COLOR_RED);
    M.GraphicalUpgrade();

    M.ChangeFrameMode(2);
    M.GetXY(xx, yy);
    M.A[xx][yy] = hehepig_block(1, COLOR_YELLOW);
    M.GraphicalUpgrade();

}

int main() {
    //cct_setconsoleborder(100, 45);

    testGraphic();
    while (1)
        testGraphic2();
    testDigit();

    //cct_setconsoleborder(120, 25);
}