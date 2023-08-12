#include <intrins.h>
void delay(int du)		//@25MHz
{
	unsigned char data i, j, k;
    for(k = 0; k < du; k++) {
        _nop_();
        i = 5;
        j = 9;
        do
        {
            while (--j);
        } while (--i);
    }
}
