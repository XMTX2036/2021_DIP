#include "bmp.h"
BYTE overflow(int n)
{
    if(n > 255) n |= 0x7ff;
    if(n < 0) n &= 0;
    return n;
}