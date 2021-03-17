#include "../include/hehepigDraw.h"
#include "../include/hehepigIO.h"

void testDigit() {

    int n, m;
    
    n = GetInt("行[5,10] >", 5, 10);
    m = GetInt("列[5,10] >", 5, 10);

    hehepig_block_map A(n, m);
    A.DigitalLogError("模式1测试");
    A.DigitalPrint();

    A.DigitalLogError("模式2测试");
    A.DigitalPrint(1);
}

int main() {
    
    testDigit();
}