#include "../include/hehepigDraw.h"
#include "../include/hehepigIO.h"

void testDigit() {

    int n, m;
    
    n = GetInt("��[5,10] >", 5, 10);
    m = GetInt("��[5,10] >", 5, 10);

    hehepig_block_map A(n, m);
    A.DigitalLogError("ģʽ1����");
    A.DigitalPrint();

    A.DigitalLogError("ģʽ2����");
    A.DigitalPrint(1);
}

int main() {
    
    testDigit();
}